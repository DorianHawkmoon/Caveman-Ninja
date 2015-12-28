#pragma once
#ifndef CONDITION_CALLBACK_H
#define CONDITION_CALLBACK_H

#include "Condition.h"
#include <functional>

class ConditionCallback : public Condition {
public:
	ConditionCallback(const std::function<bool()>& functionCheck) :functionCheck(functionCheck) {}
	~ConditionCallback() {};

	bool check() const {
		return functionCheck();
	}

private:
	/**
	* variable donde guardar la función a ejecutar
	*/
	const std::function<bool()> functionCheck;
};

#endif // CONDITION_CALLBACK_H