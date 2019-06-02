/*
	Colour Palette.h
	This file implements the colour palette on the right hand side
*/

#pragma once

#include "Canvas.h"

class ColourPalette {

public:

	// Array of colours in the palette
	static const Colour colours[];

	// index of selected colour in above array
	static int selectedIndex;

	// x position of the palette, based on window width
	static int palette_x_pos;


	/*
		Draws the colour palette on the screen
		This is called from inside the "display" function

		@param window_width - Width of the window
		@param window_height - Height of the window
	*/
	static void Display(int window_width, int window_height) {
		palette_x_pos = window_width - 50;
		// draw the button for each colour (these are not from the Button class! they are implemented here)
		glBegin(GL_QUADS);
		int xPos = palette_x_pos;
		int yPos = 50;
		for (int i = 0; i < 5; i++) {
			// Draw border for the colour
			//		different coloured border depending on whether it is selected or not
			if (i == selectedIndex) {
				glColor3f(0.6f, 0.6f, 0.6f);
			}
			else {
				glColor3f(0.2f, 0.2f, 0.2f);
			}
			glVertex2f(xPos, window_height - yPos);
			glVertex2f(xPos + 50, window_height - yPos);
			glVertex2f(xPos + 50, window_height - (yPos + 50));
			glVertex2f(xPos, window_height - (yPos + 50));
			// Now draw the colour box itself
			glColor3f(colours[i].r, colours[i].g, colours[i].b);
			glVertex2f(xPos + 2, window_height - (yPos + 2));
			glVertex2f(xPos + 46, window_height - (yPos + 2));
			glVertex2f(xPos + 46, window_height - (yPos + 46));
			glVertex2f(xPos + 2, window_height - (yPos + 46));
			yPos += 49;
		}
		glEnd();
	}


	/*
		Checks if (x,y) coordinate is within a given rect

		@param x - x coordinate of point to test
		@param y - y coordinate of point to test
		@param x_pos - x coordinate of the rect (top left)
		@param y_pos - y coordinate of the rect (top left)
		@param width - width of the rect
		@param height - height of the rect
		@return True if the point lies within the rect
	*/
	static bool checkInside(int x, int y, int x_pos, int y_pos, int width, int height) {
		if (x >= x_pos) {
			if (y >= y_pos) {
				if (x < x_pos + width) {
					if (y < y_pos + height) {
						return true;
					}
				}
			}
		}
		return false;
	}


	/*
		Handles mouse pressed events for the colour palette

		@param button - Mouse button pressed
		@param state - State of mouse event (down or up)
		@param x - The x coordinate of the mouse when pressed
		@param y - The y coordinate of the mouse when pressed
		@return Has the colour palette handled the event?
	*/
	static bool Pressed(int button, int state, int x, int y) {
		if (button == GLUT_LEFT_BUTTON) {
			if (state == GLUT_UP) {
				int xPos = palette_x_pos;
				int yPos = 50;
				for (int i = 0; i < 5; i++) {
					// Check if the mouse is inside each colour rect
					if (checkInside(x, y, xPos, yPos, 50, 50)) {
						// select the colour
						selectedIndex = i;
						selectedColour = colours[i];
						return true;
					}
					yPos += 49;
				}
			}
		}
		// block the event from continuing if the mouse is over the colour palette
		return checkInside(x, y, palette_x_pos, 50, 50, 245);
	}

};