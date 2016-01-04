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
	StateTransition(State<T>* newState, Condition* condition);
	virtual ~StateTransition();

	inline State<T>* getNextState() const {
		return newState;
	}
	void addCondition(const Condition* condition);
	bool checkCondition() ;

	bool start();
	bool cleanUp();

	StateTransition<T>* clone() const {
		StateTransition<T>* result = new StateTransition<T>(newState);
		for (auto it = conditions.begin(); it != conditions.end(); ++it) {
			result->addCondition(*it);
		}
		return result;
	}

private:
	/**
	* Condiciones
	*/
	std::list<Condition*> conditions; //conditions owned
	/**
	* Nuevo estado
	*/
	State<T>* newState; //i don't own the state
};

#include "Condition.h"

template <class T>
StateTransition<T>::StateTransition(State<T> * newState) : conditions(), newState(newState) {}

template<class T>
StateTransition<T>::StateTransition(State<T>* newState, Condition * condition) : conditions(), newState(newState) {
	addCondition(condition);
}

template <class T>
StateTransition<T>::~StateTransition() {
	for (auto it = conditions.begin(); it != conditions.end(); ++it) {
		delete *it;
	}
	conditions.clear();
}

template <class T>
void StateTransition<T>::addCondition(const Condition * condition) {
	if (condition == nullptr) {
		return;
	}
	Condition* copy = condition->clone(); //owned condition
	if (std::find(conditions.begin(), conditions.end(), copy) == conditions.end()) {
		conditions.push_back(copy);
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

template<class T>
bool StateTransition<T>::start() {
	bool result = true;
	for (std::list<Condition*>::iterator it = conditions.begin(); it != conditions.end() && result; ++it) {
		result = (*it)->start();
	}
	return result;
}

template<class T>
bool StateTransition<T>::cleanUp() {
	bool result = true;
	for (std::list<Condition*>::iterator it = conditions.begin(); it != conditions.end() && result; ++it) {
		result = (*it)->cleanUp();
	}
	return result;
}

#endif // STATE_TRANSITION_H