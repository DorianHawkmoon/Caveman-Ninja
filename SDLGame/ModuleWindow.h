#pragma once
#ifndef MODULE_WINDOW_H
#define MODULE_WINDOW_H

#include "Module.h"
#include "SDL/SDL.h"

class ModuleWindow : public Module {
public:
	ModuleWindow(bool started = true);
	virtual ~ModuleWindow();

	// Called before quitting
	bool init();

	// Called before quitting
	bool cleanUp();

public:
	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;
};

#endif // !MODULE_WINDOW_H
