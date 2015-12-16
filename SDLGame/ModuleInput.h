#pragma once

#ifndef MODULE_INPUT_H
#define MODULE_INPUT_H

#include "Module.h"
typedef unsigned __int8 Uint8;

#define MAX_KEYS 300

enum KeyState {
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module {
public:
	ModuleInput(bool started = true);
	virtual ~ModuleInput();

	// Check key states (includes mouse and joy buttons)
	inline KeyState GetKey(int id) const {
		return keyboard[id];
	}

	bool init();
	bool start();
	update_status preUpdate();
	bool cleanUp();

private:
	KeyState *keyboard;
};

#endif // !MODULE_INPUT_H
