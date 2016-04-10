#pragma once
#ifndef TRIGGER_H
#define TRIGGER_H

#include "Point.h"
#include <functional>
class Entity;

struct Trigger {
	//give the position and size of the trigger and the function that will be triggered
	static Entity* makeTrigger(const fPoint& position, const iPoint& size, const std::function<void()>& functionCheck);
};

#endif // !TRIGGER_H

