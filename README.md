# Basic Paint Program OpenGL

## Introduction
This is a basic 2D paint program (like MS paint) written in C++ and OpenGL for university coursework. Tested on Windows and Linux. 

### Features:  
1. Draw with pen tool
2. Move blocks of pixels around
3. Fill in areas
4. Select from palette of colours
5. Draw rectangles and circles/elipses
6. Save and load your files
7. Basic menus
8. Some custom dialogues: alert messages, yes/no dialogues, open/save file, etc

### Important
1. The canvas is stored as a 2D array of pixels (each pixel containing 3 floats) in system memory. It is not compressed. This means saving, loading and colour filling features may take a few seconds for your system to process them since they are moving around and modifying large amounts of data.
2. No resizing objects or transparency support
3. I was limited to OpenGL and C++ standard librarys so no native UI, not even Open/Save File Dialogues, etc.
4. Files are saved as .dti files in the application directory. They are just text files containing the colour array. No compression applied to files.
5. No clipboard or undo/redo
6. Move tool is very basic. Select area by drawing square, use arrow keys to move. It will delete anything in it's path while moving it.
7. I am no artist. It doesn't look great.

## Building
Includes a Makefile for linux.
```
make -f Makefile.linux main
./main
```

## Liscense
Use it however you like, for whatever you like.