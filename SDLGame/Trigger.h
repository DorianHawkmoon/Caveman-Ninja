#pragma once
#ifndef TRIGGER_H
#define TRIGGER_H

#include "Point.h"
#include <functional>
class Entity;

struct Trigger {
	static Entity* makeTrigger(const fPoint& position, const iPoint& size, const std::function<void()>& functionCheck);

};

#endif // !TRIGGER_H

