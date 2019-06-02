/*
	Yes No Dialogue.h
	This file adds a Yes/No Dialogue - a custom message with "Yes" and "No" buttons
			The No button closes the dialogue, the Yes button Invokes a custom callback function
*/

#pragma once

class YesNoDialogue {

public:

	// should the yes/no dialogue be displayed
	static bool show;

	// The yes and no buttons
	static Button yesButton;
	static Button noButton;

	// message to display
	static std::string message;

	// function to call when 'yes' button pressed
	static Callback yesCallback;


	/*
		starts displaying the yes/no dialogue

		@param m - The message to display in the dialogue
		@param callback - The function to invoke when the yes button gets pressed
	*/
	static void Show(std::string m, Callback callback) {
		message = m;
		yesCallback = callback;
		show = true;
		yesButton.Show();
		noButton.Show();
	}


	/*
		stops displaying the yes/no dialogue
	*/
	static void Hide() {
		show = false;
		yesButton.Hide();
		noButton.Hide();
	}


	/*
		Callback invoked when the yes button is pressed

		@param button - the button that was pressed
	*/
	static void YesPressed(Button button) {
		Hide();
		if (yesCallback) {
			// trigger the callback provided for the yes button
			(*yesCallback)(yesButton);
		}
	}


	/*
		Callback invoked when the no button is pressed
	*/
	static void NoPressed(Button button) {
		Hide();
	}


	/*
		Initializes the yes/no dialogue
	*/
	static void Init() {
		// creates the yes and no buttons
		noButton = Button::Create(0, 140, 100, 40, (char *)"No", NoPressed, true);
		yesButton = Button::Create(0, 140, 100, 40, (char *)"Yes", YesPressed, true);
	}


	/*
		Displays the yes/no dialogue

		@param window_width - the width of the window
		@param window_height - the height of the window
	*/
	static void Display(int window_width, int window_height) {
		if (show) {

			// Display another cover (as yes/no dialogues can appear above other dialogues)
			glBegin(GL_QUADS);
				glColor4f(0.0f, 0.0f, 0.0f, 0.85f);
				glVertex2f(0, window_height);
				glVertex2f(0 + window_width, window_height);
				glVertex2f(0 + window_width, 0);
				glVertex2f(0, 0);
			glEnd();

			// display the message
			display_text(message, (window_width / 2) - (get_text_width(message) / 2), window_height - 100);

			// display the buttons, offset from the center
			noButton.HorizontallyCenter(window_width);
			noButton.x_pos -= 55;
			yesButton.HorizontallyCenter(window_width);
			yesButton.x_pos += 55;
			noButton.Display(window_width, window_height);
			yesButton.Display(window_width, window_height);
		}
	}


	/*
		Handles mouse pressed events for the yes/no dialogue

		@param button - Mouse button pressed
		@param state - State of mouse event (down or up)
		@param x - The x coordinate of the mouse when pressed
		@param y - The y coordinate of the mouse when pressed
		@return Has the yes/no dialogue handled the event?
	*/
	static bool Pressed(int button, int state, int x, int y) {
		if (show) {
			// pass the event onto the yes and no buttons
			if (noButton.Pressed(button, state, x, y)) {
				return true;
			}
			if (yesButton.Pressed(button, state, x, y)) {
				return true;
			}
			return true;
		}
		return false;
	}


	/*
		Handles mouse movement events for the yes/no dialogue

		@param x - Mouse position x coordinate
		@param y - Mouse position y coordinate
		@return True if the event gets handled
	*/
	static bool Hover(int x, int y) {
		bool output = false;
		if (show) {
			// pass the event onto the yes and no buttons
			if (noButton.Hover(x, y)) {
				output = true;
			}
			if (yesButton.Hover(x, y)) {
				output = true;
			}
			return true;
		}
		return false;
	}

};
