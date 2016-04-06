#include "ModuleTimer.h"
#include <algorithm>

ModuleTimer::ModuleTimer(): Module(), fpsTimer(), deltaTime(0), fps(0), frames(0), temporalTimers() {}


ModuleTimer::~ModuleTimer() {
	for (std::list<Timer*>::iterator it = temporalTimers.begin(); it != temporalTimers.end(); ++it) {
		delete (*it);
	}
}

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

	for (std::list<Timer*>::iterator it = temporalTimers.begin(); it != temporalTimers.end(); ++it) {
		(*it)->stop();
	}

	return true;
}

int ModuleTimer::getDeltaFrame() const {
	return deltaTime;
}

int ModuleTimer::getFPS() const {
	return fps;
}

void ModuleTimer::pause() {
	fpsTimer.pause();
	for (std::list<Timer*>::iterator it = temporalTimers.begin(); it != temporalTimers.end(); ++it) {
		(*it)->pause();
	}
}

void ModuleTimer::unpause() {
	fpsTimer.unpause();
	for (std::list<Timer*>::iterator it = temporalTimers.begin(); it != temporalTimers.end(); ++it) {
		(*it)->unpause();
	}
}

void ModuleTimer::resetFrames() {
	frames = 0;
}

Timer * ModuleTimer::getTimer() {
	Timer* timer = new Timer();
	temporalTimers.push_back(timer);
	return timer;
}

void ModuleTimer::deleteTimer(Timer * timer) {
	//search for the texture
	std::list<Timer*>::iterator it = std::find_if(temporalTimers.begin(), temporalTimers.end(),
		[&timer](Timer* loaded) { return loaded == timer; });

	if (it != temporalTimers.end()) {
		temporalTimers.erase(it);
		delete *it;
	}
}
