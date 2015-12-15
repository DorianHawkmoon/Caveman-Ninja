#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum update_status {
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};


// Configuration -----------
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SCREEN_SIZE 2

#define FULLSCREEN false
#define BORDERLESS false
#define RESIZABLE true
#define FULLSCREEN_DESKTOP false
#define VSYNC true

#define TITLE "Super Awesome Game"

#endif // !GLOBALS_H