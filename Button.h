/*
	Button.h
	Implements the displaying of a Button and handling of all associated events
*/

#pragma once

#include <iostream>
#include "Fonts.h"
#include <vector>

class Button;

// Define a type representing a button pressed callback function
typedef void(*Callback)(Button button);

// Class defining a button
class Button {

	public:
		// The button pressed callback function
		Button() :callback(NULL) {}
		Callback callback;

		bool display, hovering;
		int x_pos, y_pos, width, height;

		// Text to display
		std::string text;
		

		/*
			Checks if a point (x,y) is inside the button or not

			@param x - The x coordinate of the point
			@param y - The y coordinate of the point
			@return True if the point lies within the button rectangle
		*/
		bool checkInside(int x, int y) {
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
			Detects if the button should be in the "hover" state

			@param x - Mouse position x coordinate
			@param y - Mouse position y coordinate
			@return True if the button handles the hovering event
		*/
		bool Hover(int x, int y) {
			hovering = checkInside(x, y);
			return hovering;
		}


		/*
			Handles mouse pressed events for the button

			@param button - Mouse button pressed
			@param state - State of mouse event (down or up)
			@param x - The x coordinate of the mouse when pressed
			@param y - The y coordinate of the mouse when pressed
			@return Has the button handled the event?
		*/
		bool Pressed(int button, int state, int x, int y) {
			hovering = checkInside(x, y);
			// Check the mouse press was left mouse button up
			if (button == GLUT_LEFT_BUTTON) {
				if (state == GLUT_UP) {
					if (hovering) {
						// The mouse was pressed within the button area so execute the button pressed callback
						if (callback) {
							(*callback)((Button)*this);
						}
						// return "true" as the mouse click event was handled
						return true;
					}
				}
			}
			// button should always block the event (from UI elements underneath it) if the mouse is over the top of it
			return hovering;
		}


		/*
			Draws the button on the screen
			This is called from inside the "display" function

			@param window_width - Width of the window
			@param window_height - Height of the window
		*/
		void Display(int window_width, int window_height) {
			if (display) {
				// button border
				glBegin(GL_QUADS);
				glColor3f(0.8f, 0.8f, 0.8f);
				glVertex2f(x_pos, window_height - y_pos);
				glVertex2f(x_pos + width, window_height - y_pos);
				glVertex2f(x_pos + width, window_height - y_pos - height);
				glVertex2f(x_pos, window_height - y_pos - height);
				glEnd();

				// button interior
				glBegin(GL_QUADS);
				if (hovering) {
					// display a different colour in "hover" state
					glColor3f(0.6f, 0.6f, 0.6f);
				}
				else {
					glColor3f(0.2f, 0.2f, 0.2f);
				}
				glVertex2f(x_pos + 2, window_height - y_pos - 2);
				glVertex2f(x_pos + width - 2, window_height - y_pos - 2);
				glVertex2f(x_pos + width - 2, window_height - y_pos - height + 2);
				glVertex2f(x_pos + 2, window_height - y_pos - height + 2);
				glEnd();

				// Draw the button text - the constant integers add padding
				display_text(text, x_pos + 7, window_height - y_pos - height + 10);
			}
		}


		/*
			Start showing this button in every display call
		*/
		void Show() {
			display = true;
		}


		/*
			Hides this button. Call "Show" to display it again
		*/
		void Hide() {
			display = false;
		}


		/*
			Set the x position of the button to the center of the screen
		*/
		void HorizontallyCenter(int window_width) {
			x_pos = (window_width / 2) - (width / 2);
		}

		
		/*
			Set values of button

			@param x - x position of the button
			@param y - y position of the button
			@param w - width of button
			@param h - height of the button
			@param t - text to display in the button
			@param c - function to call when button pressed
			@param d - display the button?
		*/
		void set_values(int x, int y, int w, int h, char* t, Callback c, bool d) {
			display = true;
			hovering = false;
			x_pos = x;
			y_pos = y;
			width = w;
			height = h;
			text = t;
			callback = c;
			display = d;
		}

		
		/*
			Creates a new button

			@param x - x position of the button
			@param y - y position of the button
			@param w - width of button
			@param h - height of the button
			@param t - text to display in the button
			@param c - function to call when button pressed
			@param d - display the button?
		*/
		static Button Create(int x, int y, int w, int h, char* t, Callback c, bool d) {
			Button* newButton = new Button;
			newButton->set_values(x, y, w, h, t, c, d);
			return *newButton;
		}
};