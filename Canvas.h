/*
	Canvas.h
	This file implements a Canvas (or "image") which stores an array of pixels (rbg colour values) which can be read/written and displayed
*/

#pragma once

#include <string>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <cmath>

class Canvas {

	public:
		int width, height;
		int xOffset, yOffset; // << offsets change for panning
		std::string saveFilePath;
		Colour** pixels; // << 2D array of pixel colours
		float zoom;

		std::string fileName;


		/*
			Sets values for the canvas

			@param w - width of the canvas
			@param h - height of the canvas
			@param xo - x offset of the canvas
			@param yo - y offset of the canvas
		*/
		void set_values(int w, int h, int xo, int yo) {
			fileName = "";
			width = w;
			height = h;
			xOffset = xo;
			yOffset = yo;
			zoom = 3.0f;
			// fill in grid of pixels with white colour
			struct Colour white = { 1.0f, 1.0f, 1.0f };
			pixels = new Colour*[w];
			for (int x = 0; x < w; ++x) {
				pixels[x] = new Colour[h*5];
				for (int y = 0; y < h; y++) {
					pixels[x][y] = white;
				}
			}
		}


		/*
			Returns the pixel colour at the vertex

			@param x - x coordinate to get the colour from
			@param y - y coordinate to get the colour from
		*/
		Colour GetPixelColour(int x, int y) {
			return pixels[x][y];
		}


		/*
			Assigns a colour to the position

			@param x - x coordinate to set the colour of
			@param y - y coordinate to set the colour of
			@param r - red colour
			@param g - green colour
			@param b - blue colour
		*/
		void SetPixelColour(int x, int y, float r, float g, float b) {
			struct Colour newColor = { r, g, b };
			pixels[x][y] = newColor;
		}


		/*
			Assigns a colour to the position

			@param x - x coordinate to set the colour of
			@param y - y coordinate to set the colour of
			@param c - the colour to assign
		*/
		void SetPixelColour(int x, int y, Colour c) {
			pixels[x][y] = c;
		}


		/*
			Convert float to string

			@param value - the float to convert
			@return The float as a string
		*/
		std::string to_string(float value)
		{
		  std::ostringstream os ;
		  os << value ;
		  return os.str() ;
		}


		/*
			Convert int to string

			@param value - The int to convert
			@return The int as a string
		*/
		std::string to_string(int value)
		{
			std::ostringstream os;
			os << value;
			return os.str();
		}


		/*
			Expresses canvas in string format

			@return The string representing the canvas
		*/
		std::string Serialize() {
			// output width and height on first 2 lines
			std::string output = to_string(width) + "\n" + to_string(height);
			// then list r, g, b values for all pixels
			for (int x = 0; x < width; x++) {
				for (int y = 0; y < height; y++) {
					output += "\n" + to_string(pixels[x][y].r);
					output += "\n" + to_string(pixels[x][y].g);
					output += "\n" + to_string(pixels[x][y].b);
				}
			}
			return output;
		}


		/*
			Creates a canvas from a string

			@param data - Canvas in string format
			@return New canvas from the string
		*/
		static Canvas Deserialize(std::string data) {
			std::string curLine = "";

			// c++ (pre c++ 11) has no string split function :(

			// first get width and height from first 2 lines
			int width = -1;
			int height = -1;
			int i = 0;
			while ((width == -1) || (height == -1)) {
				// goes through characters and puts them together to give a complete line
				char c = data[i];
				if (c == '\n') {
					if (width == -1) {
						width = std::atoi(curLine.c_str());
						curLine = "";
						i++;
						continue;
					}
					if (height == -1) {
						height = std::atoi(curLine.c_str());
						curLine = "";
						i++;
						break;
					}
				}
				else {
					curLine += c;
				}
				i++;
				if (i > 10000) {
					Canvas* newCanvas = new Canvas;
					newCanvas->set_values(500, 500, 100, 100);
					return *newCanvas;
				}
			}

			// now create the Canvas using width and height
			Canvas* canvasRef = new Canvas;
			canvasRef->set_values(width, height, 100, 100);
			Canvas canvas = *canvasRef;

			// continue going through string to fill in the colours
			curLine = "";
			int pixelNum = 0;
			int coloursDoneInPixel = 0;
			float r;
			float g;
			float b;
			for (int j = i; j < data.size(); j++) {
				char c = data[j];
				if (c == '\n') {
					if (coloursDoneInPixel == 0) {
						r = std::atof(curLine.c_str());
						coloursDoneInPixel++;
					}
					else {
						if (coloursDoneInPixel == 1) {
							g = std::atof(curLine.c_str());
							coloursDoneInPixel++;
						}
						else {
							b = std::atof(curLine.c_str());
							int xIndex = std::floor((double)pixelNum / (double)height);
							int yIndex = pixelNum % height;
							canvas.SetPixelColour(xIndex, yIndex, r, g, b);
							coloursDoneInPixel = 0;
							pixelNum++;
						}
					}
					curLine = "";
				}
				else {
					curLine += c;
				}
			}
			return canvas;
		}


		/*
			Draws the canvas on the screen
			This is called from inside the "display" function

			@param window_width - Width of the window
			@param window_height - Height of the window
		*/
		void Draw(int window_width, int window_height) {
			glBegin(GL_QUADS);
			for (int x = 0; x < width; x++) {
				for (int y = 0; y < height; y++) {
					// for each pixel draw a quad - size of quad is equal to the zoom
					glColor3f(pixels[x][y].r, pixels[x][y].g, pixels[x][y].b);
					glVertex2f((x*zoom) + xOffset, window_height - ((y*zoom) + yOffset));
					glVertex2f(((x+1)*zoom) + xOffset, window_height - ((y*zoom) + yOffset));
					glVertex2f(((x+1)*zoom) + xOffset, window_height - (((y+1)*zoom) + yOffset));
					glVertex2f((x*zoom) + xOffset, window_height - (((y+1)*zoom) + yOffset));
				}
			}
			glEnd();
		}


		/*
			Check if a point (x,y) is inside the button or not

			@param x - x coordinate of the point to check
			@param y - y coordinate of the point to check
			@return True if point lies inside the button
		*/
		bool checkInside(int x, int y) {
			if (x >= xOffset) {
				if (y >= yOffset) {
					if (x < xOffset + (width * zoom)) {
						if (y < yOffset + (height * zoom)) {
							return true;
						}
					}
				}
			}
			return false;
		}


		/*
			Rounds float to nearest int

			@param num - float to round
			@return The rounded integer
		*/
		int round(float num)
		{
		  return std::ceil(num - 0.5);
		}

};


/*
	Creates a new canvas with given width, height and offsets

	@param w - width of the canvas
	@param h - height of the canvas
	@param xOffset - x coordinate of canvas in window
	@param yOffset - y coordinate of canvas in window
	@return The new canvas
*/
Canvas NewCanvas(int w, int h, int xOffset, int yOffset) {
	Canvas* newCanvas = new Canvas;
	newCanvas->set_values(w, h, xOffset, yOffset);
	return *newCanvas;
}

// The canvas which is currently being displayed
static Canvas currentCanvas;
