/*
	Tool_Pen.h
	Implements the Pen tool
*/

#pragma once


/*
	Handles mouse press events passed onto the Pen tool

	@param button - Mouse button pressed
	@param state - State of mouse event (down or up)
	@param x - The x coordinate of the mouse when pressed
	@param y - The y coordinate of the mouse when pressed
	@return Has the tool handled the event?
*/
bool Tool_Pen::Pressed(int button, int state, int x, int y) {
	if (currentCanvas.checkInside(x, y)) {
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) && !isMouseDown) {
			std::cout << "start drag" << std::endl;
			isMouseDown = true;
			mouseLastX = (x - currentCanvas.xOffset) / currentCanvas.zoom;
			mouseLastY = (y - currentCanvas.yOffset) / currentCanvas.zoom;
			return true;
		}
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP) && isMouseDown) {
			std::cout << "end drag" << std::endl;
			isMouseDown = false;
			return true;
		}
	}
	return false;
}


/*
	Handles mouse movement events passed to the tool

	@param x - Mouse position x coordinate
	@param y - Mouse position y coordinate
	@return True if the tool handles the hovering event
*/
bool Tool_Pen::Hover(int x, int y) {

	if (isMouseDown) {
		if (currentCanvas.checkInside(x, y)) {
			// we want to colour all pixels on the line between the last and current mouse positions

			// First convert current mouse position to canvas coordinates
			int canvasCoordX = (x - currentCanvas.xOffset) / currentCanvas.zoom;
			int canvasCoordY = (y - currentCanvas.yOffset) / currentCanvas.zoom;

			// If the mouse hasn't moved, colour the single pixel at its position
			if ((canvasCoordX == mouseLastX) && (canvasCoordY == mouseLastY)) {
				currentCanvas.SetPixelColour(canvasCoordX, canvasCoordY, selectedColour);
			}
			else {

				int minX = std::min(canvasCoordX, mouseLastX);
				int maxX = std::max(canvasCoordX, mouseLastX);
				int minY = std::min(canvasCoordY, mouseLastY);
				int maxY = std::max(canvasCoordY, mouseLastY);

				// Find a unit vector moving along the line from last mouse position to current mouse position
				double length = std::sqrt(std::pow(maxX - minX, 2) + std::pow(maxY - minY, 2));
				double moveX = (canvasCoordX - mouseLastX) / length;
				double moveY = (canvasCoordY - mouseLastY) / length;

				// we start at current mouse position
				double curX = mouseLastX;
				double curY = mouseLastY;

				// keep moving in the direction of the unit vector and colouring in pixels until outside range
				while ((curX <= maxX) && (curY <= maxY) && (curX >= minX) && (curY >= minY)) {
					currentCanvas.SetPixelColour(round(curX), round(curY), selectedColour);
					curX += moveX;
					curY += moveY;
				}

			}

			// remember the mouse position for next time
			mouseLastX = canvasCoordX;
			mouseLastY = canvasCoordY;

		}
		return true;
	}
	return currentCanvas.checkInside(x, y);
}


/*
	Should this tool take priority for receiving mouse events

	@param button - Mouse button pressed
	@param state - State of mouse event (down or up)
	@param x - The x coordinate of the mouse when pressed
	@param y - The y coordinate of the mouse when pressed
	@return Should this tool take priority for receiving mouse events
*/
bool Tool_Pen::BlockMousePress(int button, int state, int x, int y) {
	if (isMouseDown) {
		// Should take mouse events if the mouse is already down and over the canvas
		if (currentCanvas.checkInside(x, y)) {
			return true;
		}
	}
	isMouseDown = false;
	return false;
}