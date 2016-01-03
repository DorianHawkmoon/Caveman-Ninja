#pragma once
#ifndef MODULE_H
#define MODULE_H

#include "Globals.h"
#include "IUpdater.h"
class Application;

class Module : public IUpdater {
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

	inline bool isEnabled() {
		return active;
	}

	inline virtual bool init() {
		return true;
	}

	

private:
	bool active;
};

#endif // !MODULE_H
