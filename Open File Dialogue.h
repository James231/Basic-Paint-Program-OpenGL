/*
	Open File Dialogue.h
	This adds the Open File Dialogue listing the list of the users saved files allowing them to be opened or deleted
*/

#pragma once

#include <fstream>
#include <vector>
#include "Canvas.h"

class OpenFileDialogue {

public:

	static bool show;
	static Button cancelButton;

	// The buttons for opening a file (at most 8)
	static std::vector<Button> fileButtons;
	// The buttons for deleting a file (at most 8)
	static std::vector<Button> crossButtons;

	// If we are deleting a file, this stores the file to delete while we are waiting for the user to confirm their action
	static std::string deletionPendingFileName;


	/*
		Opens the Open File Dialogue
	*/
	static void Show() {
		// Get the list of files to display
		std::vector<std::string> files = FileManagement::GetList();
		for (int i = 0; i < fileButtons.size(); i++) {
			if (i < files.size()) {
				// display the first 10 files
				fileButtons[i].text = files[i].c_str();
				fileButtons[i].Show();
				crossButtons[i].Show();
			}
			else {
				// If more files than buttons, hide the unneeded buttons
				fileButtons[i].Hide();
				crossButtons[i].Hide();
			}
		}
		Cover::show = true;
		show = true;
		cancelButton.Show();
	}


	/*
		Hides the Open File Dialogue
	*/
	static void Hide() {
		Cover::show = false;
		show = false;
		cancelButton.Hide();
		// Hide all the buttons
		for (int i = 0; i < fileButtons.size(); i++) {
			fileButtons[i].Hide();
			crossButtons[i].Hide();
		}
	}


	/*
		Callback invoked when the Cancel button is pressed
	*/
	static void CancelPressed(Button button) {
		Hide();
	}


	/*
		Callback invoked when a file is selected
	*/
	static void SelectPressed(Button button) {
		std::string content = FileManagement::ReadFile(button.text);
		Canvas newCanvas = Canvas::Deserialize(content);
		currentCanvas = newCanvas;
		canvasAssigned = true;
		currentCanvas.fileName = button.text;
		Hide();
	}


	/*
		Callback invoked when the 'X' button is pressed for a file listed
	*/
	static void DeletePressed(Button button) {
		// get the fileName from this button
		std::string fileName = "";
		for (int i = 0; i < fileButtons.size(); i++) {
			if (crossButtons[i].y_pos == button.y_pos) {
				fileName = fileButtons[i].text;
			}
		}
		// Ask the user if they are sure they want to delete the file
		YesNoDialogue::Show("Are you sure you want to delete " + fileName + ".dti?", DeleteConfirmedCallback);
		deletionPendingFileName = fileName; // << remember the file name so we can access it in the Callback function below
	}


	/*
		Callback invoked when the user confirms they are sure they want to delete a file
	*/
	static void DeleteConfirmedCallback(Button button) {
		// Delete the file and display the updated list of files
		FileManagement::DeleteFile(deletionPendingFileName);
		Show();
	}


	/*
		Initializes the Open File Dialogue
	*/
	static void Init() {
		show = false;
		// create the cancel button
		cancelButton = Button::Create(0, 500, 100, 40, (char *)"Cancel", CancelPressed, false);
		// create the 8 buttons and delete buttons
		for (int i = 0; i < 8; i++) {
			Button newButton = Button::Create(0, 160 + (i * 40), 300, 40, (char *)"asdf", SelectPressed, false);
			fileButtons.push_back(newButton);
			Button newCrossButton = Button::Create(0, 160 + (i * 40), 30, 40, (char *)"X", DeletePressed, false);
			crossButtons.push_back(newCrossButton);
		}
	}


	/*
		Displays the open file dialogue

		@param window_width - the width of the window
		@param window_height - the height of the window
	*/
	static void Display(int window_width, int window_height) {
		if (show) {
			// Display the 2 lines of centered text
			char* text = (char *)"Select a file below to open it:";
			display_text(text, (window_width / 2) - (get_text_width(text) / 2), window_height - 100);
			char* textTwo = (char *)"(these are the .dti files in the application folder)";
			display_text_small(textTwo, (window_width / 2) - (get_text_width_small(textTwo) / 2), window_height - 140);
			// Display the centered cancel button
			cancelButton.HorizontallyCenter(window_width);
			cancelButton.Display(window_width, window_height);
			// Display the list of open and delete buttons
			for (int i = 0; i < fileButtons.size(); i++) {
				fileButtons[i].HorizontallyCenter(window_width);
				fileButtons[i].Display(window_width, window_height);
				crossButtons[i].x_pos = fileButtons[i].x_pos + fileButtons[i].width;
				crossButtons[i].Display(window_width, window_height);
			}
		}
	}


	/*
		Handles button pressed events for Open File Dialogue

		@param button - Mouse button pressed
		@param state - State of mouse event (down or up)
		@param x - The x coordinate of the mouse when pressed
		@param y - The y coordinate of the mouse when pressed
		@return Has the Open File Dialogue handled the event?
	*/
	static bool Pressed(int button, int state, int x, int y) {
		if (show) {
			// Go through all the buttons and check if they handle the event
			if (cancelButton.Pressed(button, state, x, y)) {
				return true;
			}
			for (int i = 0; i < fileButtons.size(); i++) {
				if (fileButtons[i].Pressed(button, state, x, y)) {
					return true;
				}
				if (crossButtons[i].Pressed(button, state, x, y)) {
					return true;
				}
			}
		}
		return false;
	}


	/*
		Detects if the Open File Dialogue should be in the "hover" state

		@param x - Mouse position x coordinate
		@param y - Mouse position y coordinate
		@return True if the button handles the hovering event
	*/
	static bool Hover(int x, int y) {
		bool output = false;
		if (show) {
			// Go through all buttons and check if they handle the event
			if (cancelButton.Hover(x, y)) {
				output = true;
			}
			for (int i = 0; i < fileButtons.size(); i++) {
				if (fileButtons[i].Hover(x, y)) {
					output = true;
				}
				if (crossButtons[i].Hover(x, y)) {
					output = true;
				}
			}
		}
		return output;
	}
};
