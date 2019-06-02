/*
	Toolbar.h
	Implements the toolbar on the left of the screen
*/

#pragma once

// define class structures

// class to pass on events to the selected tool
class ToolEvents {
public:
	static void Start();
	static void End();
	static void Display(int window_width, int window_height);
	static bool Pressed(int button, int state, int x, int y);
	static bool Hover(int x, int y);
	static bool SpecialKey(int key, int x, int y);
	static bool BlockMousePress(int button, int state, int x, int y);
};

// classes for the tools

class Tool_Pen {
public:
	static bool isMouseDown;
	static int mouseLastX;
	static int mouseLastY;
	static bool Pressed(int button, int state, int cx, int cy);
	static bool Hover(int x, int y);
	static bool BlockMousePress(int button, int state, int x, int y);
};

class Tool_Move {
public:
	static bool isMouseDown;
	static int startMouseX;
	static int startMouseY;
	static int endMouseX;
	static int endMouseY;
	static bool isDisplaying;
	static int flickerFrameCount;
	static bool flickerColor;
	static void Start();
	static void End();
	static void Display(int window_width, int window_height);
	static bool Pressed(int button, int state, int cx, int cy);
	static bool BlockMousePress(int button, int state, int x, int y);
	static bool SpecialKey(int key, int x, int y);
};

class Tool_Fill {
public:
	static void Fill(Colour startColour, int x, int y);
	static bool Pressed(int button, int state, int x, int y);
};

class Tool_Rect {
public:
	static bool isMouseDown;
	static int startMouseX;
	static int startMouseY;
	static bool Pressed(int button, int state, int cx, int cy);
	static bool BlockMousePress(int button, int state, int x, int y);
};

class Tool_Circ {
public:
	static bool isMouseDown;
	static int startMouseX;
	static int startMouseY;
	static bool Pressed(int button, int state, int cx, int cy);
	static bool BlockMousePress(int button, int state, int x, int y);
};



/*
	This class implements the toolbar on the left of the window
*/
class Toolbar {

public:
	
	// the index of the selected tool
	static int selectedButton;

	// buttons for each tool in the menu
	static Button penButton;
	static Button moveButton;
	static Button fillButton;
	static Button rectButton;
	static Button circleButton;


	/*
		Callback invoked when a tool button is pressed from the toolbar

		@param button - the button that was pressed
	*/
	static void ToolButtonPressed(Button button) {
		ToolEvents::End();
		if (button.text == "Pen") { selectedButton = 0; }
		if (button.text == "Move") { selectedButton = 1; }
		if (button.text == "Fill") { selectedButton = 2; }
		if (button.text == "Rect") { selectedButton = 3; }
		if (button.text == "Circ") { selectedButton = 4; }
		ToolEvents::Start();
	}


	/*
		Initializes the toolbar
	*/
	static void Init() {
		// start with pen tool selected
		selectedButton = 0;
		// create the buttons for the toolbar
		penButton = Button::Create(0, 100, 78, 40, (char *)"Pen", ToolButtonPressed, true);
		moveButton = Button::Create(0, 140, 78, 40, (char *)"Move", ToolButtonPressed, true);
		fillButton = Button::Create(0, 180, 78, 40, (char *)"Fill", ToolButtonPressed, true);
		rectButton = Button::Create(0, 220, 78, 40, (char *)"Rect", ToolButtonPressed, true);
		circleButton = Button::Create(0, 260, 78, 40, (char *)"Circ", ToolButtonPressed, true);
	}


	/*
		Displays the toolbar

		@param window_width - the width of the window
		@param window_height - the height of the window
	*/
	static void Display(int window_width, int window_height) {
		// draw the buttons
		penButton.Display(window_width, window_height);
		moveButton.Display(window_width, window_height);
		fillButton.Display(window_width, window_height);
		rectButton.Display(window_width, window_height);
		circleButton.Display(window_width, window_height);
		// draw a blue overlay on the selected button
		glColor4f(0.0f, 1.0f, 1.0f, 0.4f);
		glBegin(GL_QUADS);
			int selY = window_height - ( 100 + (selectedButton * 40));
			glVertex2f(0, selY);
			glVertex2f(78, selY);
			glVertex2f(78, selY - 40);
			glVertex2f(0, selY - 40);
		glEnd();
	}


	/*
		Handles mouse press events passing them on to the selected tool

		@param button - Mouse button pressed
		@param state - State of mouse event (down or up)
		@param x - The x coordinate of the mouse when pressed
		@param y - The y coordinate of the mouse when pressed
		@return Has the event been handled?
	*/
	static bool Pressed(int button, int state, int x, int y) {
		if ((selectedButton != 0) && (penButton.Pressed(button, state, x, y))) {
			return true;
		}
		if ((selectedButton != 1) && (moveButton.Pressed(button, state, x, y))) {
			return true;
		}
		if ((selectedButton != 2) && (fillButton.Pressed(button, state, x, y))) {
			return true;
		}
		if ((selectedButton != 3) && (rectButton.Pressed(button, state, x, y))) {
			return true;
		}
		if ((selectedButton != 4) && (circleButton.Pressed(button, state, x, y))) {
			return true;
		}
		return false;
	}


	/*
		Handles mouse movement events passed to the tool

		@param x - Mouse position x coordinate
		@param y - Mouse position y coordinate
		@return True if the event gets handled
	*/
	static bool Hover(int x, int y) {
		bool output = false;
		if (penButton.Hover(x, y)) {
			output = true;
		}
		if (moveButton.Hover(x, y)) {
			output = true;
		}
		if (fillButton.Hover(x, y)) {
			output = true;
		}
		if (rectButton.Hover(x, y)) {
			output = true;
		}
		if (circleButton.Hover(x, y)) {
			output = true;
		}
		return output;
	}

};


/*
	Passes on start event to the selected tool
*/
void ToolEvents::Start() {
	switch (Toolbar::selectedButton) {
		case 1:
			Tool_Move::Start();
			break;
	}
}


/*
	Passes on end event ot the selected tool
*/
void ToolEvents::End() {
	switch (Toolbar::selectedButton) {
		case 1:
			Tool_Move::End();
			break;
	}
}


/*
	Passes display event onto selected tool

	@param window_width - the width of the window
	@param window_height - the height of the window
*/
void ToolEvents::Display(int window_width, int window_height) {
	switch (Toolbar::selectedButton) {
	case 1:
		Tool_Move::Display(window_width, window_height);
		break;
	}
}


/*
	Passes Mouse pressed event onto the selected tool

	@param button - Mouse button pressed
	@param state - State of mouse event (down or up)
	@param x - The x coordinate of the mouse when pressed
	@param y - The y coordinate of the mouse when pressed
	@return Has the tool handled the event?
*/
bool ToolEvents::Pressed(int button, int state, int x, int y) {
	switch (Toolbar::selectedButton) {
	case 0:
		if (Tool_Pen::Pressed(button, state, x, y)) {
			return true;
		}
		break;
	case 1:
		if (Tool_Move::Pressed(button, state, x, y)) {
			return true;
		}
		break;
	case 2:
		if (Tool_Fill::Pressed(button, state, x, y)) {
			return true;
		}
		break;
	case 3:
		if (Tool_Rect::Pressed(button, state, x, y)) {
			return true;
		}
		break;
	case 4:
		if (Tool_Circ::Pressed(button, state, x, y)) {
			return true;
		}
		break;
	}
	return false;
}


/*
	Passes mouse movement events onto the selected tool

	@param x - Mouse position x coordinate
	@param y - Mouse position y coordinate
	@return True if the event gets handled
*/
bool ToolEvents::Hover(int x, int y) {
	switch (Toolbar::selectedButton) {
	case 0:
		if (Tool_Pen::Hover(x, y)) {
			return true;
		}
		break;
	}
	return false;
}


/*
	Passes special key events (arrow keys) on to the selected tool

	@param key - the key that was pressed
	@param x - x position of the mouse
	@param y - y position of the mouse
	@return Has the event been handled?
*/
bool ToolEvents::SpecialKey(int key, int x, int y) {
	switch (Toolbar::selectedButton) {
	case 1:
		if (Tool_Move::SpecialKey(key, x, y)) {
			return true;
		}
		break;
	}
	return false;
}


/*
	Should the selected tool take priority for receiving mouse events

	@param button - Mouse button pressed
	@param state - State of mouse event (down or up)
	@param x - The x coordinate of the mouse when pressed
	@param y - The y coordinate of the mouse when pressed
	@return Should the selected tool take priority for receiving mouse events
*/
bool ToolEvents::BlockMousePress(int button, int state, int x, int y) {
	switch (Toolbar::selectedButton) {
	case 0:
		if (Tool_Pen::BlockMousePress(button, state, x, y)) {
			return true;
		}
		break;
	case 1:
		if (Tool_Move::BlockMousePress(button, state, x, y)) {
			return true;
		}
		break;
	case 3:
		if (Tool_Rect::BlockMousePress(button, state, x, y)) {
			return true;
		}
		break;
	case 4:
		if (Tool_Circ::BlockMousePress(button, state, x, y)) {
			return true;
		}
		break;
	}
	return false;
}