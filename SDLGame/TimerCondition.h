#pragma once
#ifndef TIMER_CONDITION_H
#define TIMER_CONDITION_H

#include "Condition.h"
#include "Timer.h"
#include "ModuleTimer.h"
#include "Application.h"

class TimerCondition : public Condition {
public:
	TimerCondition(unsigned int milliseconds) : milliseconds(milliseconds), timer() {
		timer=App->timer->getTimer();
	}
	virtual ~TimerCondition() {
		App->timer->deleteTimer(timer);
	}

	bool start() { 
		timer->start();
		return true;
	}
	bool check() const {
		return timer->value() >= milliseconds;
	}

	bool cleanUp() { 
		timer->stop();
		return true; 
	}

	virtual Condition* clone() const {
		return new TimerCondition(milliseconds);
	}

private:
	unsigned int milliseconds;
	Timer* timer;
};


#endif // !TIMER_CONDITION_H
