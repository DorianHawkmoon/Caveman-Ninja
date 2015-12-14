#pragma once
#ifndef MODULE_H
#define MODULE_H

#include "Globals.h"

class Application;

class Module {
public:

	Module(bool active = true) : active(active) {

	}

	bool enable() {
		if (active == false) {
			return active = start();
		}

		return true;
	}

	bool disable() {
		if (active == true) {
			return active = !cleanUp();
		}

		return true;
	}

	virtual bool init() {
		return true;
	}

	virtual bool start() {
		return true;
	}

	virtual update_status preUpdate() {
		return UPDATE_CONTINUE;
	}

	virtual update_status update() {
		return UPDATE_CONTINUE;
	}

	virtual update_status postUpdate() {
		return UPDATE_CONTINUE;
	}

	virtual bool cleanUp() {
		return true;
	}

private:
	bool active;
};

#endif // !MODULE_H
