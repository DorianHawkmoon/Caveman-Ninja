#include "ModuleInput.h"
#include "Globals.h"
#include <algorithm>



ModuleInput::ModuleInput(bool started): Module(started) {
	//keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
}

// Destructor
ModuleInput::~ModuleInput() {
	LOG("Quiting SDL input event system");
	//if (keyboard != nullptr){
		//delete keyboard; 
		//keyboard = nullptr; 
	//}

	SDL_QuitSubSystem(SDL_INIT_EVENTS);
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

bool ModuleInput::start() {
	return true;
}

// Called every draw update
update_status ModuleInput::preUpdate() {
	//TODO: improve the input system
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	memset(windowEvents, false, WE_COUNT * sizeof(bool));

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

	SDL_Event events;
	while (SDL_PollEvent(&events)) {
		switch (events.type) {
			case SDL_QUIT:
				return UPDATE_STOP;
			break;
			case SDL_WINDOWEVENT:
				switch (events.window.event) {
					//case SDL_WINDOWEVENT_LEAVE:
					case SDL_WINDOWEVENT_HIDDEN:
					case SDL_WINDOWEVENT_MINIMIZED:
					//case SDL_WINDOWEVENT_FOCUS_LOST:
						windowEvents[WE_HIDE] = true;
						break;

						//case SDL_WINDOWEVENT_ENTER:
					case SDL_WINDOWEVENT_SHOWN:
					case SDL_WINDOWEVENT_FOCUS_GAINED:
					case SDL_WINDOWEVENT_MAXIMIZED:
					case SDL_WINDOWEVENT_RESTORED:
						windowEvents[WE_SHOW] = true;
						break;

					case SDL_WINDOWEVENT_MOVED:
					case SDL_WINDOWEVENT_RESIZED:
					case SDL_WINDOWEVENT_SIZE_CHANGED:
					//case SDL_WINDOWEVENT_FOCUS_LOST:
						windowEvents[WE_PAUSE] = true;

				}
				break;

		}
	}

	if (getKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		return UPDATE_STOP;
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::cleanUp() {
	LOG("Disabling SDL input event subsystem");
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	return true;
}

bool ModuleInput::singleKey(const SDL_Scancode & key, const std::list<SDL_Scancode>& exceptions) const {
	bool result = getKey(key) != KEY_IDLE;
	for (int i = 0; i < MAX_KEYS && result; ++i) {
		if (i != key) {
			std::list<SDL_Scancode>::const_iterator it = std::find(exceptions.begin(), exceptions.end(), i);
			if (it == exceptions.end()) {
				result &= !(getKey(i));
			}
		}
	}
	return result;
}


bool ModuleInput::keyActive(const SDL_Scancode & key) const {
	return getKey(key) != KEY_IDLE;
}

bool ModuleInput::singleCombination(const std::list<SDL_Scancode>& keys, const std::list<SDL_Scancode>& exceptions)const  {
	bool result = combinationActive(keys);
	for (int i = 0; i < MAX_KEYS && result; ++i) {
		bool parcial = !(getKey(i)); // !pressed
		if (!parcial) { //means is pressed
			//look if is one of the selected keys to be pressed
			std::list<SDL_Scancode>::const_iterator it = std::find(keys.begin(), keys.end(), i);
			if (it == keys.end()) {
				//pressed without being one of be pressed! exception?
				std::list<SDL_Scancode>::const_iterator it2 = std::find(exceptions.begin(), exceptions.end(), *it);
				if (it2 == exceptions.end()) {
					//not an exception
					result = false;
				}
				
			}
		}
	}
	return result;
}

bool ModuleInput::combinationActive(const std::list<SDL_Scancode>& keys) const {
	bool result = true;
	for (std::list<SDL_Scancode>::const_iterator it = keys.begin(); it != keys.end() && result; ++it) {
		result &= keyActive(*it);
	}
	return result;
}
