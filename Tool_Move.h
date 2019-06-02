/*
	Tool_Move.h
	Implements the Move tool
*/

#pragma once


/*
	Initializes tool when selected
*/
void Tool_Move::Start() {
	isDisplaying = false;
	isMouseDown = false;
	flickerFrameCount = 20;
	flickerColor = false;
}


/*
	Disables tool when another is selected
*/
void Tool_Move::End() {
	isDisplaying = false;
	isMouseDown = false;
}


/*
	Displays rect around selected pixels

	@param window_width - the width of the window
	@param window_height - the height of the window
*/
void Tool_Move::Display(int window_width, int window_height) {
	if ((!Tool_Move::isDisplaying) || isMouseDown) {
		// display white cover over canvas while user needs to draw rectangle to select pixels
		glBegin(GL_QUADS);
			glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
			glVertex2f(currentCanvas.xOffset, window_height - currentCanvas.yOffset);
			glVertex2f(currentCanvas.xOffset + (currentCanvas.width*currentCanvas.zoom), window_height - currentCanvas.yOffset);
			glVertex2f(currentCanvas.xOffset + (currentCanvas.width*currentCanvas.zoom), window_height - currentCanvas.yOffset - (currentCanvas.height*currentCanvas.zoom));
			glVertex2f(currentCanvas.xOffset, window_height - currentCanvas.yOffset - (currentCanvas.height*currentCanvas.zoom));
		glEnd();
	}
	else {
		// display the blue/red flickering rectangle around selected pixels
		glBegin(GL_LINES);
			if (flickerColor) {
				glColor4f(0.4f, 0.4f, 1.0f, 1.0f);
			}
			else {
				glColor4f(1.0f, 0.4f, 0.4f, 1.0f);
			}
			glVertex2f(currentCanvas.xOffset + (startMouseX*currentCanvas.zoom), window_height - currentCanvas.yOffset - (startMouseY*currentCanvas.zoom));
			glVertex2f(currentCanvas.xOffset + (endMouseX*currentCanvas.zoom), window_height - currentCanvas.yOffset - (startMouseY*currentCanvas.zoom));
			glVertex2f(currentCanvas.xOffset + (endMouseX*currentCanvas.zoom), window_height - currentCanvas.yOffset - (startMouseY*currentCanvas.zoom));
			glVertex2f(currentCanvas.xOffset + (endMouseX*currentCanvas.zoom), window_height - currentCanvas.yOffset - (endMouseY*currentCanvas.zoom));
			glVertex2f(currentCanvas.xOffset + (endMouseX*currentCanvas.zoom), window_height - currentCanvas.yOffset - (endMouseY*currentCanvas.zoom));
			glVertex2f(currentCanvas.xOffset + (startMouseX*currentCanvas.zoom), window_height - currentCanvas.yOffset - (endMouseY*currentCanvas.zoom));
			glVertex2f(currentCanvas.xOffset + (startMouseX*currentCanvas.zoom), window_height - currentCanvas.yOffset - (endMouseY*currentCanvas.zoom));
			glVertex2f(currentCanvas.xOffset + (startMouseX*currentCanvas.zoom), window_height - currentCanvas.yOffset - (startMouseY*currentCanvas.zoom));
		glEnd();
		// change the flickering colour every 20 frames
		flickerFrameCount--;
		if (flickerFrameCount <= 0) {
			flickerFrameCount = 20;
			flickerColor = !flickerColor;
		}
	}
}


/*
	Handles mouse press events passed onto the Move tool

	@param button - Mouse button pressed
	@param state - State of mouse event (down or up)
	@param x - The x coordinate of the mouse when pressed
	@param y - The y coordinate of the mouse when pressed
	@return Has the tool handled the event?
*/
bool Tool_Move::Pressed(int button, int state, int x, int y) {
	if (currentCanvas.checkInside(x, y)) {
		// get mouse position in canvas coordinates
		int cx = (x - currentCanvas.xOffset) / currentCanvas.zoom;
		int cy = (y - currentCanvas.yOffset) / currentCanvas.zoom;
		// remember the drag start position (mouse down) and end position (mouse up)
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) && !isMouseDown) {
			isMouseDown = true;
			startMouseX = cx;
			startMouseY = cy;
			return true;
		}
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP) && isMouseDown) {
			isMouseDown = false;
			endMouseX = cx;
			endMouseY = cy;
			isDisplaying = true;
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
bool Tool_Move::BlockMousePress(int button, int state, int x, int y) {
	if (isMouseDown) {
		// take priority with mouse events when tool is selected and mouse is over canvas
		if (currentCanvas.checkInside(x, y)) {
			return true;
		}
	}
	isMouseDown = false;
	return false;
}


/*
	Handles special key events (arrow keys) for the tool

	@param key - the key that was pressed
	@param x - x position of the mouse
	@param y - y position of the mouse
	@return Has the tool handled the event?
*/
bool Tool_Move::SpecialKey(int key, int x, int y) {
	if (isDisplaying) {
		// get rect coordinates
		int minX = std::min(startMouseX, endMouseX);
		int maxX = std::max(startMouseX, endMouseX);
		int minY = std::min(startMouseY, endMouseY);
		int maxY = std::max(startMouseY, endMouseY);
		switch (key)
		{
			case GLUT_KEY_LEFT:
				// left key pressed
				if (minX > 0) {
					Colour white = { 1.0f, 1.0f, 1.0f };
					for (int x = 0; x < maxX - minX; x++) {
						for (int y = 0; y < maxY - minY; y++) {
							// move all pixels left
							currentCanvas.SetPixelColour(minX + x - 1, minY + y, currentCanvas.GetPixelColour(minX + x, minY + y));
							if (x == maxX - minX - 1) {
								// leave column of white pixels on the right
								currentCanvas.SetPixelColour(minX + x, minY + y, white);
							}
						}
					}
					// rectangle has moved
					startMouseX--;
					endMouseX--;
					return true;
				}
				break;
			// do the same for other arrow keys
			case GLUT_KEY_RIGHT:
				if (maxX < currentCanvas.width) {
					Colour white = { 1.0f, 1.0f, 1.0f };
					for (int x = maxX - minX - 1; x >= 0; x--) {
						for (int y = 0; y < maxY - minY; y++) {
							currentCanvas.SetPixelColour(minX + x + 1, minY + y, currentCanvas.GetPixelColour(minX + x, minY + y));
							if (x == 0) {
								currentCanvas.SetPixelColour(minX + x, minY + y, white);
							}
						}
					}
					startMouseX++;
					endMouseX++;
					return true;
				}
				break;
			case GLUT_KEY_UP:
				if (minY > 0) {
					Colour white = { 1.0f, 1.0f, 1.0f };
					for (int x = 0; x < maxX - minX; x++) {
						for (int y = 0; y < maxY - minY; y++) {
							currentCanvas.SetPixelColour(minX + x, minY + y - 1, currentCanvas.GetPixelColour(minX + x, minY + y));
							if (y == maxY - minY - 1) {
								currentCanvas.SetPixelColour(minX + x, minY + y, white);
							}
						}
					}
					startMouseY--;
					endMouseY--;
					return true;
				}
				break;
			case GLUT_KEY_DOWN:
				if (maxY < currentCanvas.height) {
					Colour white = { 1.0f, 1.0f, 1.0f };
					for (int x = 0; x < maxX - minX; x++) {
						for (int y = maxY - minY - 1; y >= 0; y--) {
							currentCanvas.SetPixelColour(minX + x, minY + y + 1, currentCanvas.GetPixelColour(minX + x, minY + y));
							if (y == 0) {
								currentCanvas.SetPixelColour(minX + x, minY + y, white);
							}
						}
					}
					startMouseY++;
					endMouseY++;
					return true;
				}
				break;
		}
		return true;
	}
	return false;
}
