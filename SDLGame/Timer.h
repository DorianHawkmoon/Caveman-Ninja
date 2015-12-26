#pragma once
#ifndef TIMER_H
#define TIMER_H
class Timer {
public:
	Timer();
	~Timer();

	void start();
	void pause();
	void unpause();
	void stop();

	bool isPaused();
	bool isStopped();

	unsigned int value();

private:
	bool paused;
	bool stopped;
	unsigned int startTime;
	unsigned int pauseTime;
};

#endif // TIMER_H