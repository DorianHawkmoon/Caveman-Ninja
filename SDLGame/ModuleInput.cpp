#include "ModuleInput.h"
#include "Globals.h"
#include "SDL\SDL.h"

#include "Application.h"
#include "ModuleAudio.h"

ModuleInput::ModuleInput() {}

// Destructor
ModuleInput::~ModuleInput() {
	if (keyboard != nullptr) {
		delete keyboard;
	}
}

// Called before render is available
bool ModuleInput::Init() {
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0) {
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::Update() {
	SDL_PumpEvents();

	keyboard = SDL_GetKeyboardState(nullptr);

	if (keyboard[SDL_SCANCODE_ESCAPE]) {
		return UPDATE_STOP;
	}

	if (keyboard[SDL_SCANCODE_F]) {
		//TODO en otro sitio!!
		Mix_PlayChannel(-1, App->audio->LoadEffect("playerLaser.wav"), 0);
	}

	SDL_Event Events;
	while (SDL_PollEvent(&Events)) {
		if (Events.type == SDL_QUIT)
			return UPDATE_STOP;
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp() {
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}