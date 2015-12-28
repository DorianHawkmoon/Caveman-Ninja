#pragma once
#ifndef CONDITION_EQUAL_H
#define CONDITION_EQUAL_H

#include "Condition.h"
template <class T>
class ConditionEqual : public Condition {
public:
	ConditionEqual(const T* watch, const T& trigger) : watch(watch), trigger(trigger) {}
	~ConditionEqual() {};

	bool check() const {
		return *watch == trigger;
	}

private:
	const T* watch;
	const T trigger;
};

#endif // CONDITION_EQUAL_H