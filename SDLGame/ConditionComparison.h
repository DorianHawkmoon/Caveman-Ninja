#pragma once
#ifndef CONDITION_COMPARISON_H
#define CONDITION_COMPARISON_H

#include "Condition.h"
enum Compare {
	EQUAL,
	GREATER,
	LESS,
	GREATER_EQUAL,
	LESS_EQUAL
};

template <class T>
class ConditionComparison : public Condition {
public:
	ConditionComparison(const T* watch, const T& trigger, Compare comparison=EQUAL) : watch(watch), trigger(trigger), comparison(comparison) {}
	virtual ~ConditionComparison() {};

	bool check() const {
		bool result;
		switch (comparison) {
			case EQUAL:
				result =*watch == trigger;
				break;
			case GREATER:
				result = *watch > trigger;
				break;
			case LESS:
				result = *watch < trigger;
				break;
			case GREATER_EQUAL:
				result = *watch >= trigger;
				break;
			case LESS_EQUAL:
				result = *watch <= trigger;
				break;
		}
		return result;
	}

	Condition* clone() const {
		return new ConditionComparison(watch, trigger, comparison);
	}

private:
	const Compare comparison;
	const T* watch; //not owned
	const T trigger;
};

#endif // CONDITION_COMPARISON_H