/*
	Tool_Fill.h
	Implements the Fill tool
*/

#pragma once

#include <vector>

// struct for storing coordinates
struct Tuple {
	int x;
	int y;
};


/*
	Fill algorithm from a position

	@param startColour - The colour we should be replacing
	@param cx - x coordinate of pixel to fill from
	@param cy - y coordinate of pixel to fill from
*/
void Tool_Fill::Fill(Colour startColour, int cx, int cy) {
	// algorithm expands from point filling an area
	// vec stores outer pixels for the next iteration
	std::vector<Tuple> vec;
	std::vector<Tuple> newvec;
	Tuple startCoord = { cx, cy };
	vec.push_back(startCoord);
	while (vec.size() > 0) {
		for (int i = 0; i < vec.size(); i++) {
			// change pixel colour for pixels on outside of the fill area
			Colour colourAtPixel = currentCanvas.GetPixelColour(vec[i].x, vec[i].y);
			if ((colourAtPixel.r == startColour.r) && (colourAtPixel.g == startColour.g) && (colourAtPixel.b == startColour.b)) {
				currentCanvas.SetPixelColour(vec[i].x, vec[i].y, selectedColour);
				// add neighbours to the outside vector for the next iteration
				if (vec[i].x < currentCanvas.width - 1) {
					Tuple newCoord = { vec[i].x + 1, vec[i].y };
					newvec.push_back(newCoord);
				}
				if (vec[i].x > 0) {
					Tuple newCoord = { vec[i].x - 1, vec[i].y };
					newvec.push_back(newCoord);
				}
				if (vec[i].y < currentCanvas.height - 1) {
					Tuple newCoord = { vec[i].x, vec[i].y + 1 };
					newvec.push_back(newCoord);
				}
				if (vec[i].y > 0) {
					Tuple newCoord = { vec[i].x, vec[i].y - 1 };
					newvec.push_back(newCoord);
				}
			}

		}
		vec.empty();
		vec = newvec;
		std::vector<Tuple> b;
		newvec = b;
	}
}


/*
	Handles mouse press events passed onto the Fill tool

	@param button - Mouse button pressed
	@param state - State of mouse event (down or up)
	@param x - The x coordinate of the mouse when pressed
	@param y - The y coordinate of the mouse when pressed
	@return Has the tool handled the event?
*/
bool Tool_Fill::Pressed(int button, int state, int x, int y) {
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		int canvasCoordX = (x - currentCanvas.xOffset) / currentCanvas.zoom;
		int canvasCoordY = (y - currentCanvas.yOffset) / currentCanvas.zoom;
		Colour colourWherePressed = currentCanvas.GetPixelColour(canvasCoordX, canvasCoordY);
		if ((colourWherePressed.r != selectedColour.r) || (colourWherePressed.g != selectedColour.g) || (colourWherePressed.b != selectedColour.b)) {
			Tool_Fill::Fill(colourWherePressed, canvasCoordX, canvasCoordY);
			return true;
		}
	}
	return false;
}