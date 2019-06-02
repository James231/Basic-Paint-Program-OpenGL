/*
	Save File Dialogue.h
	This adds the Save File Dialogue where the user can type the name of the file and save it
*/

#pragma once

#include "Canvas.h"
#include <iostream>
#include <fstream>
#include <string>

class SaveFileDialogue {

public:

	static bool show;

	// cancel and save buttons to display
	static Button cancelButton;
	static Button saveButton;

	// the file name which has been typed in
	static std::string fileName;

	// has user tried to enter a file name over 40 chars
	static bool showTooLongText;


	/*
		resets the save file dialogue
	*/
	static void Reset() {
		fileName = "";
		saveButton.Hide();
		showTooLongText = false;
	}


	/*
		start showing the save file dialogue
	*/
	static void Show() {
		Cover::show = true;
		show = true;
		cancelButton.Show();
	}


	/*
		hide the save file dialogue
	*/
	static void Hide() {
		Cover::show = false;
		show = false;
		cancelButton.Hide();
	}


	/*
		Callback invoked when the cancel button is pressed
	*/
	static void CancelPressed(Button button) {
		Hide();
	}


	/*
		Callback invoked when the save button is pressed
	*/
	static void SavePressed(Button button) {
		// check the file name is a valid length
		if ((fileName.size() > 0) && (fileName.size() < 40)) {
			// If the file already exists, ask if the user wants to overwrite it
			if (FileManagement::CheckExists(fileName)) {
				YesNoDialogue::Show("File already exists. Overwrite?", SaveOverwriteCallback);
				return;
			}
			// File doesn't already exists so save it
			FileManagement::WriteFile(fileName, currentCanvas.Serialize());
			currentCanvas.fileName = fileName;
			Hide();
		}
	}


	/*
		Callback when user confirms they want to save, overwriting a file
	*/
	static void SaveOverwriteCallback(Button button) {
		// save the file
		FileManagement::WriteFile(fileName, currentCanvas.Serialize());
		currentCanvas.fileName = fileName;
		Hide();
	}


	/*
		Initializes the open file dialogue
	*/
	static void Init() {
		show = false;
		cancelButton = Button::Create(0, 200, 100, 40, (char *)"Cancel", CancelPressed, false);
		saveButton = Button::Create(0, 200, 75, 40, (char *)"Save", SavePressed, false);
	}


	/*
		Displays the save file dialogue

		@param window_width - the width of the window
		@param window_height - the height of the window
	*/
	static void Display(int window_width, int window_height) {
		if (show) {
			// display text at top
			char* text = (char *)"Type in a file name to save it:";
			display_text(text, (window_width / 2) - (get_text_width(text) / 2), window_height - 100);
			display_text(fileName, (window_width / 2) - (get_text_width(fileName) / 2), window_height - 155);
			// If the user is trying to enter a file name too long, display error message
			if (showTooLongText) {
				char* text2 = (char *)"40 character file name limit";
				display_text_small(text2, (window_width / 2) - (get_text_width_small(text2) / 2), window_height - 184);
			}
			// display the cancel and save buttons (centered then offset from center)
			cancelButton.HorizontallyCenter(window_width);
			cancelButton.x_pos -= 55;
			saveButton.HorizontallyCenter(window_width);
			saveButton.x_pos += 65;
			cancelButton.Display(window_width, window_height);
			saveButton.Display(window_width, window_height);
		}
	}


	/*
		Handles button pressed events for Save File Dialogue

		@param button - Mouse button pressed
		@param state - State of mouse event (down or up)
		@param x - The x coordinate of the mouse when pressed
		@param y - The y coordinate of the mouse when pressed
		@return Has the Save File Dialogue handled the event?
	*/
	static bool Pressed(int button, int state, int x, int y) {
		if (show) {
			// pass the event on to cancel and save buttons
			if (cancelButton.Pressed(button, state, x, y)) {
				return true;
			}
			if (saveButton.Pressed(button, state, x, y)) {
				return true;
			}
		}
		return false;
	}


	/*
		Detects if the Save File Dialogue should be in the "hover" state

		@param x - Mouse position x coordinate
		@param y - Mouse position y coordinate
		@return True if the dialogue handles the hovering event
	*/
	static bool Hover(int x, int y) {
		bool output = false;
		if (show) {
			// pass the event on to cancel and save buttons
			if (cancelButton.Hover(x, y)) {
				output = true;
			}
			if (saveButton.Hover(x, y)) {
				output = true;
			}
		}
		return output;
	}


	/*
		Handles keyboard pressed events

		@param key - The key that was pressed
		@param x - The x position of the mouse
		@param y - The y position of the mosue
		@return True if the event was handled
	*/
	static bool KeyboardPressed(unsigned char key, int x, int y) {
		if (show) {
			// Limit the characters allowed in file names (e.g. ? is not allowed in file names on windows)
			std::string allowedChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_+=[]{}!�$^&()`�,;@' ";
			if (allowedChars.find(key) != std::string::npos) {
				saveButton.Show();
				if (fileName.size() < 40) {
					// add the character to file name
					fileName += key;
				}
				else {
					showTooLongText = true;
				}
				return true;
			}
			if (key == '\b') {
				// backspace key was pressed
				if (fileName.size() > 0) {
					// remove last character form file name
					fileName = fileName.substr(0, fileName.size() - 1);
				}
				if (fileName.size() == 0) {
					saveButton.Hide();
				}
				showTooLongText = false;
				return true;
			}
		}
		return false;
	}

};
