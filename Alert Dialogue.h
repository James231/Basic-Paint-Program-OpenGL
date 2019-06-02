/*
	Alert Dialogue.h
	This adds a Alert Dialogue which displays a custom message, and "Ok" button.
*/

#pragma once

class AlertDialogue {

public:

	static bool show;
	static Button okButton;
	static std::string message;


	/*
		Displays the Alert dialogue with custom message text

		@param m - The message to display
	*/
	static void Alert(std::string m) {
		message = m;
		show = true;
		// Draw the ok button and enable the black semi-transparent cover
		okButton.Show();
		Cover::show = true;
	}


	/*
		Hides the Alert Dialogue
	*/
	static void Hide() {
		show = false;
		// Hide the ok button and disable the black semi-transparent cover
		okButton.Hide();
		Cover::show = false;
	}

	
	/*
		Callback function when the Ok button is pressed. Hides the Alert Dialogue

		@param button - The button that was pressed
	*/
	static void OkPressed(Button button) {
		Hide();
	}


	/*
		Initialize the Alert Dialogue
	*/
	static void Init() {
		// Creat the Ok button and open the dialogue for a welcome screen
		okButton = Button::Create(0, 140, 100, 40, (char *)"Ok", OkPressed, true);
		Alert("Welcome to this 2D Drawing Tool. Press \"New\" to get started.");
	}


	/*
		Displays the Alert Dialogue

		@param window_width - Width of the window
		@param window_height - Height of the window
	*/
	static void Display(int window_width, int window_height) {
		if (show) {
			// Display the text aligned to the center of the screen
			display_text(message, (window_width / 2) - (get_text_width(message) / 2), window_height - 100);
			// Align the ok button to the center of the screen and display it
			okButton.HorizontallyCenter(window_width);
			okButton.Display(window_width, window_height);
		}
	}


	/*
		Handles button pressed events for alert dialogue

		@param button - Mouse button pressed
		@param state - State of mouse event (down or up)
		@param x - The x coordinate of the mouse when pressed
		@param y - The y coordinate of the mouse when pressed
		@return Has the alert dialogue handled the event?
	*/
	static bool Pressed(int button, int state, int x, int y) {
		if (show) {
			// pass event onto ok button
			if (okButton.Pressed(button, state, x, y)) {
				return true;
			}
		}
		return false;
	}


	/*
		Handles mouse move events for alert dialogue

		@param x - The new x coordinate of the mouse
		@param y - The new y coordinate of the mouse
		@return Has the alert dialogue handled the event?
	*/
	static bool Hover(int x, int y) {
		bool output = false;
		if (show) {
			if (okButton.Hover(x, y)) {
				output = true;
			}
		}
		return output;
	}

};