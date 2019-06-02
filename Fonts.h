/*
	Fonts.h
	Implements drawing of text using a stroke font

	This code is origionally from the labs
*/

#pragma once

#include <cstring>

/*
	Displays text on the screen using a stroke font

	@param text - The text to display
*/
void draw_text(const char* text) {
	size_t len = strlen(text);
	for (size_t i = 0; i<len; i++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
}


/*
	Displays text on the screen using a stroke font

	@param text - The text to display
*/
void draw_text(std::string text) {
	size_t len = text.size();
	for (size_t i = 0; i<len; i++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
}


/*
	Displays text on the screen at a specific position using a stroke font

	@param text - The text to display
	@param x - X coordinate to draw the text
	@param y - Y coordinate to draw the text
*/
void display_text(const char* text, float x, float y) {
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f); // white colour
	// translate to the right place, and scale to a standard size
	glTranslatef(x, y, 0.0f);
	glScalef(0.22f, 0.22f, 1.0f);
	draw_text(text);
	glPopMatrix();
}


/*
	Displays text on the screen at a specific position using a stroke font

	@param text - The text to display
	@param x - X coordinate to draw the text
	@param y - Y coordinate to draw the text
*/
void display_text(std::string text, float x, float y) {
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f); // white colour
	// translate to the right place, and scale to a standard size
	glTranslatef(x, y, 0.0f);
	glScalef(0.22f, 0.22f, 1.0f);
	draw_text(text);
	glPopMatrix();
}

/*
	Displays text on the screen at a specific position using a stroke font with small font size

	@param text - The text to display
	@param x - X coordinate to draw the text
	@param y - Y coordinate to draw the text
*/
void display_text_small(const char* text, float x, float y) {
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f); // white colour
	glTranslatef(x, y, 0.0f);
	// translate to the right place, and scale to a small size
	glScalef(0.17f, 0.17f, 1.0f);
	draw_text(text);
	glPopMatrix();
}


/*
	Calculates the width of the text at standard font size

	@param text - The text to caluclate the width of
	@return The width of the text when displayed
*/
float get_text_width(const char* text) {
	size_t len = strlen(text);
	int total_width = 0;
	for (size_t i = 0; i<len; i++) {
		total_width += glutStrokeWidth(GLUT_STROKE_ROMAN, text[i]);
	}
	return total_width * 0.22f;
}


/*
	Calculates the width of the text at standard font size

	@param text - The text to caluclate the width of
	@return The width of the text when displayed
*/
float get_text_width(std::string text) {
	size_t len = text.size();
	int total_width = 0;
	for (size_t i = 0; i<len; i++) {
		total_width += glutStrokeWidth(GLUT_STROKE_ROMAN, text[i]);
	}
	return total_width * 0.22f;
}


/*
	Calculates the width of the text at a small font size

	@param text - The text to caluclate the width of
	@return The width of the text when displayed
*/
float get_text_width_small(const char* text) {
	size_t len = strlen(text);
	int total_width = 0;
	for (size_t i = 0; i<len; i++) {
		total_width += glutStrokeWidth(GLUT_STROKE_ROMAN, text[i]);
	}
	return total_width * 0.17f;
}
