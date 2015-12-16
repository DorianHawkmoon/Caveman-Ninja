#pragma once
#ifndef MODULE_H
#define MODULE_H

#include "Globals.h"

class Application;

class Module {
public:

	Module(bool active = true) : active(active) {

	}

	inline bool enable() {
		if (active == false) {
			return active = start();
		}

		return true;
	}

	inline bool disable() {
		if (active == true) {
			return active = !cleanUp();
		}

		return true;
	}

	inline bool isEnabled() {
		return active;
	}

	inline virtual bool init() {
		return true;
	}

	inline virtual bool start() {
		return true;
	}

	inline virtual update_status preUpdate() {
		return UPDATE_CONTINUE;
	}

	inline virtual update_status update() {
		return UPDATE_CONTINUE;
	}

	inline virtual update_status postUpdate() {
		return UPDATE_CONTINUE;
	}

	inline virtual bool cleanUp() {
		return true;
	}

private:
	bool active;
};

#endif // !MODULE_H
