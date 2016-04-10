#pragma once
#ifndef CONDITION_CALLBACK_H
#define CONDITION_CALLBACK_H

#include "Condition.h"
#include <functional>

class ConditionCallback : public Condition {
public:
	ConditionCallback(const std::function<bool()>& functionCheck)  {
		this->functionCheck = functionCheck;
	}
	virtual ~ConditionCallback() {};

	bool check() const {
		return functionCheck();
	}

	Condition* clone() const{
		return new ConditionCallback(functionCheck);
	}

private:
	/**
	* variable donde guardar la función a ejecutar
	*/
	std::function<bool()> functionCheck;
};

#endif // CONDITION_CALLBACK_H