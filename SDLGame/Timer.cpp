#include "Timer.h"
#include "SDL\SDL.h"


Timer::Timer() {
	startTime = 0;
	pauseTime = 0;

	paused = false;
	stopped = false;
}


Timer::~Timer() {}

void Timer::start() {
	//Start the timer
	stopped = false;

	//Unpause the timer
	paused = false;

	//Get the current clock time
	startTime = SDL_GetTicks();
	pauseTime = 0;
}

void Timer::pause() {
	//If the timer is running and isn't already paused
	if (!stopped && !paused) {
		//Pause the timer
		paused = true;

		//Calculate the paused ticks
		pauseTime = SDL_GetTicks() - startTime;
		startTime = 0;
	}
}

void Timer::unpause() {
	//If the timer is running and paused
	if (!stopped && paused) {
		//Unpause the timer
		paused = false;

		//Reset the starting ticks
		startTime = SDL_GetTicks() - pauseTime;

		//Reset the paused ticks
		pauseTime = 0;
	}
}

void Timer::stop() {
	//Stop the timer
	stopped = true;

	//Unpause the timer
	paused = false;

	//Clear tick variables
	startTime = 0;
	pauseTime = 0;
}

bool Timer::isPaused() const {
	return paused;
}

bool Timer::isStopped() const {
	return stopped;
}

unsigned int Timer::value() const {
	//The actual timer time
	unsigned int time = 0;

	//If the timer is running
	if (!stopped) {
		//If the timer is paused
		if (paused) {
			//Return the number of ticks when the timer was paused
			time = pauseTime;
		} else {
			//Return the current time minus the start time
			time = SDL_GetTicks() - startTime;
		}
	}

	return time;
}

float Timer::miliToSeconds(unsigned int miliseconds) {
	return static_cast<float>(miliseconds / 1000.0);
}
