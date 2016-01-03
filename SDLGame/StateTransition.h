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
	~StateTransition();

	inline State<T>* getNextState() const {
		return newState;
	}
	void addCondition(Condition* condition);
	bool checkCondition() ;

	bool start();
	bool cleanUp();

private:
	/**
	* Condiciones
	*/
	std::list<Condition*> conditions; //conditions owned TODO CAN'T DELETED, SHARED POINTERS!!
	/**
	* Nuevo estado
	*/
	State<T>* newState; //i don't own the state
};

#include "Condition.h"

template <class T>
StateTransition<T>::StateTransition(State<T> * newState) : conditions(), newState(newState) {}

template<class T>
inline StateTransition<T>::StateTransition(State<T>* newState, Condition * condition) : conditions(), newState(newState) {
	conditions.push_back(condition);
}

template <class T>
StateTransition<T>::~StateTransition() {
	conditions.clear();
}

template <class T>
void StateTransition<T>::addCondition(Condition * condition) {
	if (condition == nullptr) {
		return;
	}
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