#pragma once
#ifndef STATE_H
#define STATE_H

#include <list>
#include "StateTransition.h"

template <class T>
class State {
public:
	State(T& value);
	virtual ~State();

	void addTransition(const StateTransition<T>* transition) {
		if (std::find(transitions.begin(), transitions.end(), transition) == transitions.end()) {
			StateTransition<T>* copy = transition->clone();
			transitions.push_back(copy);
		}
	}

	bool deleteTransition(const StateTransition<T>* transition) {
		if (find(transitions.begin(), transitions.end(), transition) != transitions.end()) {
			erase it;
			return true;
		}
		return false;
	}

	State<T>* processTransition() const;
	void onTransition(State<T>* newState) {
		cleanUp();
		value.cleanUp();
		newState->getValue()->start();
		newState->start();
	};
	virtual bool start();
	virtual bool cleanUp();
	inline T* getValue() {
		return &value;
	}

protected:
	T value;
	std::list<StateTransition<T>*> transitions;
};

template<class T>
State<T>::State(T & value) : value(value) {}

template<class T>
State<T>::~State() {
	for (auto it = transitions.begin(); it != transitions.end(); ++it) {
		delete *it;
	}
}

template<class T>
State<T>* State<T>::processTransition() const {
	State<T>* result = nullptr;
	for (auto it = transitions.begin();	it != transitions.end() && result == nullptr; ++it) {
		if ((*it)->checkCondition()) {
			result = (*it)->getNextState();
		}
	}
	return result;
}

template<class T>
bool State<T>::start() {
	bool result = true;
	for (auto it = transitions.begin(); it != transitions.end() && result; ++it) {
		result = (*it)->start();
	}
	return result;
}

template<class T>
inline bool State<T>::cleanUp() {
	bool result = true;
	for (auto it = transitions.begin(); it != transitions.end() && result; ++it) {
		result = (*it)->cleanUp();
	}
	return result;
}


#endif //STATE_H

