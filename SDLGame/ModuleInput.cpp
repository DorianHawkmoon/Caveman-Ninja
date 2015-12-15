#include "ModuleInput.h"
#include "Globals.h"
#include "SDL/SDL.h"

#include "Application.h"
#include "ModuleAudio.h"



ModuleInput::ModuleInput(bool started): Module(started) {
	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
}

// Destructor
ModuleInput::~ModuleInput() {
	if (keyboard != nullptr){
		delete[] keyboard; 
		keyboard = nullptr; 
	}
}

// Called before render is available
bool ModuleInput::init() {
	LOG("init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0) {
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::preUpdate() {
	//TODO: improve the input system
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	//store the whole state of the keyboard
	for (int i = 0; i < MAX_KEYS; ++i) {
		if (keys[i] == 1) {
			if (keyboard[i] == KEY_IDLE) {
				keyboard[i] = KEY_DOWN;
			} else {
				keyboard[i] = KEY_REPEAT;
			}

		} else {
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN) {
				keyboard[i] = KEY_UP;
			} else {
				keyboard[i] = KEY_IDLE;
			}
		}
	}

	if (keyboard[SDL_SCANCODE_ESCAPE]) {
		return UPDATE_STOP;
	}

	if (keyboard[SDL_SCANCODE_F]) {
		//TODO en otro sitio!!
		//Mix_PlayChannel(-1, App->audio->loadEffect("playerLaser.wav"), 0);
	}

	SDL_Event events;
	while (SDL_PollEvent(&events)) {
		switch (events.type) {
			case SDL_QUIT:
				return UPDATE_STOP;
			break;


		}
	}

	if (GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		return UPDATE_STOP;
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::cleanUp() {
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}