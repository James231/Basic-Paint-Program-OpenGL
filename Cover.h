/*
	Cover.h
	This file handles the "Cover" - the dark semi-transparent layer to cover everything when displaying dialogues
*/

#pragma once

class Cover {

public:

	// Should the cover be displayed
	static bool show;


	/*
		Displays the cover if necessary
	*/
	static void Display(int window_width, int window_height) {
		if (show) {
			glBegin(GL_QUADS);
				// semi transparent black colour
				glColor4f(0.0f, 0.0f, 0.0f, 0.85f);
				// with quad to fill the whole window
				glVertex2f(0, window_height);
				glVertex2f(0 + window_width, window_height);
				glVertex2f(0 + window_width, 0);
				glVertex2f(0, 0);
			glEnd();
		}
	}


	/*
		Handles button pressed events for the cover

		@param button - Mouse button pressed
		@param state - State of mouse event (down or up)
		@param x - The x coordinate of the mouse when pressed
		@param y - The y coordinate of the mouse when pressed
		@return Has the Top Menu Bar handled the event?
	*/
	static bool Pressed(int button, int state, int x, int y) {
		// if showing cover block mouse presses for layers underneath cover
		return show;
	}


	/*
		Handles mouse move events for cover

		@param x - The new x coordinate of the mouse
		@param y - The new y coordinate of the mouse
		@return Has the top menu bar handled the event?
	*/
	static bool Hover(int x, int y) {
		// if showing cover block mouse move events for layers underneath cover
		return show;
	}
};