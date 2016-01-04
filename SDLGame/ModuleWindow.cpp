#include "ModuleWindow.h"
#include "Globals.h"

ModuleWindow::ModuleWindow(bool started):Module(started) {}

// Destructor
ModuleWindow::~ModuleWindow() {
	if (screen_surface != nullptr) {
		//TODO error deleting
		//delete screen_surface;
	}

	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if (window != nullptr) {
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
}

// Called before render is available
bool ModuleWindow::init() {
	LOG("init SDL window & surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else {
		//Create window
		int width = SCREEN_WIDTH* SCREEN_SIZE;
		int height = SCREEN_HEIGHT* SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_SHOWN;

		if (FULLSCREEN == true) {
			flags |= SDL_WINDOW_FULLSCREEN;
		}
		if (FULLSCREEN_DESKTOP == true) {
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (BORDERLESS == true) {
			flags |= SDL_WINDOW_BORDERLESS;
		}
		if (RESIZABLE == true) {
			flags |= SDL_WINDOW_RESIZABLE;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if (window == NULL) {
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else {
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::cleanUp() {
	LOG("CleanUp windows");
	return true;
}

