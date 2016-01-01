#include "ModuleTimer.h"

ModuleTimer::ModuleTimer(): Module(), fpsTimer(), deltaTime(0), fps(0), frames(0) {}


ModuleTimer::~ModuleTimer() {}

bool ModuleTimer::start() {
	fpsTimer.start();
	return true;
}

update_status ModuleTimer::preUpdate() {
	++frames;
	//get the delta time (time of the last frame)
	deltaTime = fpsTimer.value();
	fpsTimer.start(); //reset counter
	//calculate the fps
	float seconds = fpsTimer.miliToSeconds(deltaTime);
	fps = static_cast<int>(frames / seconds);

	return UPDATE_CONTINUE;
}

bool ModuleTimer::cleanUp() {
	fpsTimer.stop();
	deltaTime = 0;
	fps = 0;
	frames = 0;
	return true;
}
