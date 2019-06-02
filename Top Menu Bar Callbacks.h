/*
	Top Menu Bar Calllbacks.h
	This file adds callbacks to the top menu bar buttons. These are functions which are called when the "New", "Open", "Save" and "Save As" buttons are pressed
*/

#pragma once

#include "Button.h"
#include "Cover.h"
#include "Open File Dialogue.h"
#include "Save File Dialogue.h"


/*
	"New" pressed then "Yes" pressed in "are you sure" dialogue

	@param button - The button that was pressed
*/
void NewConfirmedCallback(Button button) {
	canvasAssigned = true;
	currentCanvas = NewCanvas(500, 500, 100, 100);
}


/*
	"New" pressed

	@param button - The button that was pressed
*/
void NewButtonPressed(Button button) {
	if (canvasAssigned) {
		YesNoDialogue::Show("You will loose any unsaved changes. Continue?", NewConfirmedCallback);
	}
	else {
		canvasAssigned = true;
		currentCanvas = NewCanvas(500, 500, 100, 100);
	}
}


/*
	"Open" pressed

	@param button - The button that was pressed
*/
void OpenButtonPressed(Button button) {
	OpenFileDialogue::Show();
}


/*
	"Save" pressed

	@param button - The button that was pressed
*/
void SaveButtonPressed(Button button) {
	// If there is no current canvas, show an error
	if (!canvasAssigned) {
		AlertDialogue::Alert("No canvas has been created. Create one before saving.");
		return;
	}
	// If the current canvas is new (without a file path), go to Save File Dialogue (just like Save As)
	if (currentCanvas.fileName == "") {
		SaveFileDialogue::Reset();
		SaveFileDialogue::Show();
		return;
	}
	// If it is new (with a file path) save it to the path
	FileManagement::WriteFile(currentCanvas.fileName, currentCanvas.Serialize());
	AlertDialogue::Alert("Saved to " + currentCanvas.fileName + ".dti");
}

/*
	"SaveAs" pressed

	@param button - The button that was pressed
*/
void SaveAsButtonPressed(Button button) {
	if (!canvasAssigned) {
		AlertDialogue::Alert("No canvas has been created. Create one before saving.");
		return;
	}
	SaveFileDialogue::Reset();
	SaveFileDialogue::Show();
}
