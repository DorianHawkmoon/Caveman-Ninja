#pragma once
#ifndef STATE_TRANSITION_H
#define STATE_TRANSITION_H

#include <list>
template <class T>
class State;
class Condition;

/**
* Estructura de transición para la máquina de estados finitos
*/
template <class T>
class StateTransition {
public:
	StateTransition(State<T>* newState);
	~StateTransition();

	inline State<T>* getNextState() const {
		return newState;
	}
	void addCondition(Condition* condition);
	bool checkCondition() ;

private:
	/**
	* Condiciones
	*/
	std::list<Condition*> conditions;
	/**
	* Nuevo estado
	*/
	State<T>* newState;
};

#include "Condition.h"

template <class T>
StateTransition<T>::StateTransition(State<T> * newState) : conditions(), newState(newState) {}

template <class T>
StateTransition<T>::~StateTransition() {
	conditions.clear();
}

template <class T>
void StateTransition<T>::addCondition(Condition * condition) {
	if (std::find(conditions.begin(), conditions.end(), condition) == conditions.end()) {
		conditions.push_back(condition);
	}
}

template <class T>
bool StateTransition<T>::checkCondition() {
	bool result = true;
	for (std::list<Condition*>::iterator it = conditions.begin(); it != conditions.end() && result; ++it) {
		result = (*it)->check();
	}
	return result;
}

#endif // STATE_TRANSITION_H