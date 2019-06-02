/*
	Pointer.h
	Displays a small white cross at the mouse position
*/

#pragma once

int cursorX = 0;
int cursorY = 0;

/*
	Draws a white cross on the screen on a (-1, 1) scale
*/
void Draw_Cross() {
	glBegin(GL_LINE_LOOP);
	static float pointOne[2] = { -1.0f, 0.0f };
	static float pointTwo[2] = { 1.0f, 0.0f };
	glVertex2fv(pointOne);
	glVertex2fv(pointTwo);
	glEnd();
	glBegin(GL_LINE_LOOP);
	static float pointThree[2] = { 0.0f, 1.0f };
	static float pointFour[2] = { 0.0f, -1.0f };
	glVertex2fv(pointThree);
	glVertex2fv(pointFour);
	glEnd();
}

/*
	Draws a small white cross on the screen
*/
void Display_Pointer() {
	glMatrixMode(GL_MODELVIEW);
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(2.0f);
	glPushMatrix();
	glTranslatef(cursorX, glutGet(GLUT_WINDOW_HEIGHT) - cursorY, 1.0f);
	glScalef(20.0f, 20.0f, 1.0f);
	Draw_Cross();
	glPopMatrix();
}