/*
	Tool_Rect.h
	Implements the Rect tool
*/

#pragma once


/*
	Handles mouse press events passed onto the Rect tool

	@param button - Mouse button pressed
	@param state - State of mouse event (down or up)
	@param x - The x coordinate of the mouse when pressed
	@param y - The y coordinate of the mouse when pressed
	@return Has the tool handled the event?
*/
bool Tool_Rect::Pressed(int button, int state, int x, int y) {
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
		// draw the rect if this is the end of a drag
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP) && isMouseDown) {
			if ((startMouseX == cx) && (startMouseY == cy)) {
				// if the mouse hasn't moved, just colour one pixel
				currentCanvas.SetPixelColour(cx, cy, selectedColour);
			}
			else {
				// get the rect coordinates
				int minX = std::min(cx, startMouseX);
				int maxX = std::max(cx, startMouseX);
				int minY = std::min(cy, startMouseY);
				int maxY = std::max(cy, startMouseY);
				for (int px = 0; px <= maxX - minX; px++) {
					for (int py = 0; py <= maxY - minY; py++) {
						// fill in the pixels
						currentCanvas.SetPixelColour(minX + px, minY + py, selectedColour);
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
bool Tool_Rect::BlockMousePress(int button, int state, int x, int y) {
	// if during a drag, this tool should take the mouse events first
	if (isMouseDown) {
		if (currentCanvas.checkInside(x, y)) {
			return true;
		}
	}
	isMouseDown = false;
	return false;
}