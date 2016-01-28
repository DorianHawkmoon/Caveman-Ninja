#pragma once

#ifndef MODULE_INPUT_H
#define MODULE_INPUT_H

#include "Module.h"
#include <cstdint>
//typedef unsigned __int8 Uint8;
#include <list>
#include "SDL/SDL.h"
#define MAX_KEYS 300

enum KeyState {
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

enum EventWindow {
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_PAUSE = 3,
	WE_COUNT
};

class ModuleInput : public Module {
public:
	ModuleInput(bool started = true);
	virtual ~ModuleInput();

	// Check key states (includes mouse and joy buttons)
	inline KeyState getKey(int id) const {
		return keyboard[id];
	}

	// Check for window events last frame
	inline bool getWindowEvent(EventWindow code) const {
		return windowEvents[code];
	}

	bool init();
	bool start();
	update_status preUpdate();
	bool cleanUp();

	bool singleKey(const SDL_Scancode& key, const std::list<SDL_Scancode>& exceptions) const ;
	bool keyActive(const SDL_Scancode& key) const;
	bool singleCombination(const std::list<SDL_Scancode>& keys, const std::list<SDL_Scancode>& exceptions) const;
	bool combinationActive(const std::list<SDL_Scancode>& keys) const;

private:
	KeyState keyboard[MAX_KEYS];
	bool windowEvents[WE_COUNT];
};

#endif // !MODULE_INPUT_H
