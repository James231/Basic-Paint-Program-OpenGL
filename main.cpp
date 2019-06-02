/*
	main.cpp
	Entry point for 2D Drawing Tool
*/


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stddef.h>
#include <iostream>
#include <math.h>

// For each static class we import from a header file, we need to redefine its variables here

#include "Colour.h"
Colour selectedColour = { 1.0f, 0.0f, 0.0f };
bool canvasAssigned = false;
#include "File Management.h"
#include "Button.h"
#include "Fonts.h"
#include "Pointer.h"
#include "Cover.h"
bool Cover::show = false;
#include "Canvas.h"
#include "Colour Palette.h"
const Colour ColourPalette::colours[] = {
	{ 1.0f, 0.0f, 0.0f },		// red
	{ 0.0f, 1.0f, 0.0f },		// green
	{ 0.0f, 0.0f, 1.0f },		// blue
	{ 0.0f, 0.0f, 0.0f },		// black
	{ 1.0f, 1.0f, 1.0f }		// white
};
int ColourPalette::selectedIndex = 0;
int ColourPalette::palette_x_pos = 750;
#include "Alert Dialogue.h"
bool AlertDialogue::show = false;
Button AlertDialogue::okButton;
std::string AlertDialogue::message = "";
#include "Yes No Dialogue.h"
bool YesNoDialogue::show = false;
Button YesNoDialogue::yesButton;
Button YesNoDialogue::noButton;
std::string YesNoDialogue::message = "";
Callback YesNoDialogue::yesCallback;
#include "Open File Dialogue.h"
bool OpenFileDialogue::show = false;
Button OpenFileDialogue::cancelButton;
std::vector<Button> OpenFileDialogue::fileButtons;
std::vector<Button> OpenFileDialogue::crossButtons;
std::string OpenFileDialogue::deletionPendingFileName = "";
#include "Save File Dialogue.h"
bool SaveFileDialogue::show = false;
Button SaveFileDialogue::cancelButton;
Button SaveFileDialogue::saveButton;
std::string SaveFileDialogue::fileName = "";
bool SaveFileDialogue::showTooLongText = false;
#include "Top Menu Bar Callbacks.h"
#include "Top Menu Bar.h"
std::vector<Button> TopMenuBar::buttons;
#include "Toolbar.h"
int Toolbar::selectedButton = 0;
Button Toolbar::penButton;
Button Toolbar::moveButton;
Button Toolbar::fillButton;
Button Toolbar::rectButton;
Button Toolbar::circleButton;
#include "Tool_Pen.h"
bool Tool_Pen::isMouseDown = false;
int Tool_Pen::mouseLastX = 0;
int Tool_Pen::mouseLastY = 0;
#include "Tool_Fill.h"
#include "Tool_Rect.h"
bool Tool_Rect::isMouseDown = false;
int Tool_Rect::startMouseX = 0;
int Tool_Rect::startMouseY = 0;
#include "Tool_Circ.h"
bool Tool_Circ::isMouseDown = false;
int Tool_Circ::startMouseX = 0;
int Tool_Circ::startMouseY = 0;
#include "Tool_Move.h"
int Tool_Move::flickerFrameCount;
bool Tool_Move::flickerColor;
bool Tool_Move::isMouseDown;
int Tool_Move::startMouseX;
int Tool_Move::startMouseY;
int Tool_Move::endMouseX;
int Tool_Move::endMouseY;
bool Tool_Move::isDisplaying;




/*
	OpenGL display function
*/
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();

		// Rescale to "pixel" scale - position (x, y) is x pixels along, y pixels up
		// Allows user to resize window without stretching UI elements
		glScalef((double)(800) / (double)(glutGet(GLUT_WINDOW_WIDTH)), (double)(600) / (double)(glutGet(GLUT_WINDOW_HEIGHT)), 1.0f);

		// Draw the canvas and any overlays from tools in use
		if (canvasAssigned) {
			currentCanvas.Draw(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			ToolEvents::Display(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		}

		// Draw the colour palette
		ColourPalette::Display(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

		// Draw the toolbar on left hand side
		Toolbar::Display(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

		// Draw the top menu bar buttons (new, open, save, etc)
		TopMenuBar::Display(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

		// Draw the dark semi-transparent cover if necessary
		Cover::Display(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

		// Draw File Dialgues if necessary
		OpenFileDialogue::Display(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		SaveFileDialogue::Display(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		AlertDialogue::Display(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		YesNoDialogue::Display(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

		// Draw mouse pointer last (so it appears above everything else)
		Display_Pointer();

	glPopMatrix();
	glutSwapBuffers();
}


/*
	Handles mouse click events

	@param button - Mouse button pressed
	@param state - State of mouse event (down or up)
	@param x - The x coordinate of the mouse when pressed
	@param y - The y coordinate of the mouse when pressed
*/
void mouse_click(int button, int state, int x, int y)
{
	// If we are currently using a tool which wants all mouse events first, give it the mouse events
	// Otherwise pass the mouse event onto each UI element in turn until it gets handled, in the order of depth
	if ((!canvasAssigned) || (!ToolEvents::BlockMousePress(button, state, x, y))) {
		// First pass the event onto the Dialogues
		if (AlertDialogue::Pressed(button, state, x, y)) {
			return;
		}
		if (YesNoDialogue::Pressed(button, state, x, y)) {
			return;
		}
		if (OpenFileDialogue::Pressed(button, state, x, y)) {
			return;
		}
		if (SaveFileDialogue::Pressed(button, state, x, y)) {
			return;
		}
		// If not handled, maybe the Cover will block it
		if (Cover::Pressed(button, state, x, y)) {
			return;
		}
		// If not handled pass it onto buttons/toolbars
		if (TopMenuBar::Pressed(button, state, x, y)) {
			return;
		}
		if (Toolbar::Pressed(button, state, x, y)) {
			return;
		}
		if (ColourPalette::Pressed(button, state, x, y)) {
			return;
		}
	}
	// If it hasn't been handled, pass it on to the selected tool if we have a canvas
	if (canvasAssigned) {
		if (ToolEvents::Pressed(button, state, x, y)) {
			return;
		}
	}
}


/*
	Handles all mouse movement events

	@param x - The new x coordinate of the mouse
	@param y - The new y coordinate of the mouse
*/
void mouse_motion(int x, int y)
{
	// Remember new cursor position for the Pointer
	cursorX = x;
	cursorY = y;

	// Pass the mouse move event onto Dialogues
	if (AlertDialogue::Hover(x, y)) {
		return;
	}
	if (YesNoDialogue::Hover(x, y)) {
		return;
	}
	if (OpenFileDialogue::Hover(x, y)) {
		return;
	}
	if (SaveFileDialogue::Hover(x, y)) {
		return;
	}

	// If not handled, maybe the Cover will block it
	if (Cover::Hover(x, y)) {
		return;
	}

	// If not handled pass it onto buttons/toolbars
	if (Toolbar::Hover(x, y)) {
		return;
	}
	if (TopMenuBar::Hover(x, y)) {
		return;
	}

	// If it hasn't been handled, pass it on to the selected tool if we have a canvas
	if (canvasAssigned) {
		if (ToolEvents::Hover(x, y)) {
			return;
		}
	}
}


/*
	Handles standard keyboard events

	@param key - The key that was pressed
	@param x - The x position of the mouse
	@param y - The y position of the mouse
*/
void keyboard(unsigned char key, int x, int y)
{
	// Save File Dialogue should steal the input events if active
	// But Yes/No Dialogue can appear above it so should block inputs if it is being displayed
	if (YesNoDialogue::show) {
		return;
	}
	// Now pass on to Save File Dialogue
	if (SaveFileDialogue::KeyboardPressed(key, x, y)) {
		return;
	}

	// otherwise check for quitting or zoom
	switch (key)
	{
		case 'q': exit(1); break;
		case 's':
			// zoom in
			if (canvasAssigned) {
				currentCanvas.zoom++;
			}
			break;
		case 'S':
			// zoom out
			if (canvasAssigned) {
				if (currentCanvas.zoom > 1) {
					currentCanvas.zoom--;
				}
			}
			break;
	}
}


/*
	Handles special keyboard events

	@param key - The key that was pressed
	@param x - The x position of the mouse
	@param y - The y position of the mouse
*/
void special(int key, int x, int y)
{
	if (!Cover::show) {

		// Pass onto selected tool (Move tool uses arrow keys)
		if (canvasAssigned) {
			if (ToolEvents::SpecialKey(key, x, y)) {
				return;
			}
		}

		// If not handled check arrow keys for panning camera
		switch (key)
		{
			case GLUT_KEY_LEFT: if (canvasAssigned) { currentCanvas.xOffset -= 6; } break;
			case GLUT_KEY_RIGHT: if (canvasAssigned) { currentCanvas.xOffset += 6; } break;
			case GLUT_KEY_UP: if (canvasAssigned) { currentCanvas.yOffset -= 6; } break;
			case GLUT_KEY_DOWN: if (canvasAssigned) { currentCanvas.yOffset += 6; } break;
		}
	}
}


/*
	Called to intialize all classes
*/
void init()
{
	// Initialize classes where needed
	TopMenuBar::Init();
	Toolbar::Init();
	OpenFileDialogue::Init();
	SaveFileDialogue::Init();
	AlertDialogue::Init();
	YesNoDialogue::Init();

	// Enable transparency (e.g. black semi-transparent cover over screen appears with dialogues)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set initial scale for window coordinates
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}


/*
	idle function
*/
void idle() {
	// force a redraw
	// so we get through as many frames as possible (needed for things like blinking of Move tool)
	glutPostRedisplay();
}


/*
	Main entry point of application
*/
int main(int argc, char* argv[])
{
	// create window with title and fixed start size
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutCreateWindow("2D Drawing Tool");

	// define the display function
	glutDisplayFunc(display);

	// handlers for keyboard input
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);

	// define mouse pressed event handler
	glutMouseFunc(mouse_click);

	// define mouse movement event handlers
	glutPassiveMotionFunc(mouse_motion); // << when mouse is not being pressed
	glutMotionFunc(mouse_motion); // << when mouse is being pressed

	// define idle and init
	glutIdleFunc(idle);

	// initialize everything
	init();

	// start first render cycle
	glutMainLoop();
	return 0;
}
