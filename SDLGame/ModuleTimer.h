#pragma once
#ifndef MODULE_TIMER_H
#define MODULE_TIMER_H

#include "Module.h"
#include "Timer.h"

class ModuleTimer :	public Module {
public:
	ModuleTimer();
	~ModuleTimer();

	bool start();

	update_status preUpdate();

	bool cleanUp();

	inline int getDeltaFrame() const {
		return deltaTime;
	}
	inline int getFPS() const {
		return fps;
	}

	inline void resetFrames() {
		frames = 0;
	}
private:
	Timer fpsTimer;
	unsigned int deltaTime;
	unsigned int fps;
	unsigned int frames;
};

#endif // !MODULE_TIMER_H

