#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

#if false
#define DEBUG
#endif

void log(const char file[], int line, const char* format, ...);

// Configuration -----------
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 256
#define SCREEN_SIZE 3
#define FPS 60

#define FULLSCREEN false
#define BORDERLESS false
#define RESIZABLE true
#define FULLSCREEN_DESKTOP false
#define VSYNC true

#define TITLE "Caveman Ninja"

#endif // !GLOBALS_H