#pragma once
#ifndef MODULE_TIMER_H
#define MODULE_TIMER_H

#include "Module.h"
#include "Timer.h"

class ModuleTimer :	public Module {
public:
	ModuleTimer();
	virtual ~ModuleTimer();

	bool start();

	update_status preUpdate();
	bool cleanUp();

	int getDeltaFrame() const;
	int getFPS() const;

	void pause();
	void unpause();

	void resetFrames();

private:
	Timer fpsTimer;
	unsigned int deltaTime;
	unsigned int fps;
	unsigned int frames;
};

#endif // !MODULE_TIMER_H

