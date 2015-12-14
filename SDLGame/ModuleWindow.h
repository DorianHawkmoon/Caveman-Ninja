#pragma once
#ifndef MODULE_WINDOW_H
#define MODULE_WINDOW_H

#include "Module.h"
#include "SDL\SDL.h"

class ModuleWindow : public Module {
public:
	ModuleWindow();
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();

	// Called before quitting
	bool CleanUp();

public:
	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;
};

#endif // !MODULE_WINDOW_H
