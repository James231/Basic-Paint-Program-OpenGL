/*
	Tool_Circ.h
	Implements the Circle drawing tool
*/

#pragma once


/*
	Handles mouse press events passed onto the Circ tool

	@param button - Mouse button pressed
	@param state - State of mouse event (down or up)
	@param x - The x coordinate of the mouse when pressed
	@param y - The y coordinate of the mouse when pressed
	@return Has the tool handled the event?
*/
bool Tool_Circ::Pressed(int button, int state, int x, int y) {
	if (currentCanvas.checkInside(x, y)) {
		// convert mouse position into canvas coordinates
		int cx = (x - currentCanvas.xOffset) / currentCanvas.zoom;
		int cy = (y - currentCanvas.yOffset) / currentCanvas.zoom;
		// remember the start mouse position if this is start of a drag
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) && !isMouseDown) {
			isMouseDown = true;
			startMouseX = cx;
			startMouseY = cy;
			return true;
		}
		// draw the circle if this is the end of a drag
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP) && isMouseDown) {
			if ((startMouseX == cx) && (startMouseY == cy)) {
				// if the mouse hasn't moved, just colour one pixel
				currentCanvas.SetPixelColour(cx, cy, selectedColour);
			}
			else {
				// get the rect coordinates to put the circle in
				int minX = std::min(cx, startMouseX);
				int maxX = std::max(cx, startMouseX);
				int minY = std::min(cy, startMouseY);
				int maxY = std::max(cy, startMouseY);
				// work out the radii and center coords
				float radH = (maxX - minX) / 2;
				float radV = (maxY - minY) / 2;
				float centX = (maxX + minX) / 2;
				float centY = (maxY + minY) / 2;
				// go through each pixel in the bounding rect, colour it if it lies within ellipse
				for (int px = 0; px <= maxX - minX; px++) {
					for (int py = 0; py <= maxY - minY; py++) {
						if ((std::pow(((px+minX) - centX) / radH, 2) + std::pow(((py+minY) - centY) / radV, 2)) < 1) {
							currentCanvas.SetPixelColour(minX + px, minY + py, selectedColour);
						 }
					}
				}
			}
			isMouseDown = false;
			return true;
		}
	}
	return false;
}


/*
	Should this tool take priority for receiving mouse events

	@param button - Mouse button pressed
	@param state - State of mouse event (down or up)
	@param x - The x coordinate of the mouse when pressed
	@param y - The y coordinate of the mouse when pressed
	@return Should this tool take priority for receiving mouse events
*/
bool Tool_Circ::BlockMousePress(int button, int state, int x, int y) {
	// if during a drag, this tool should take the mouse events first
	if (isMouseDown) {
		if (currentCanvas.checkInside(x, y)) {
			return true;
		}
	}
	isMouseDown = false;
	return false;
}