#pragma once
#ifndef STATE_H
#define STATE_H

#include <list>
#include "StateTransition.h"

template <class T>
class State {
public:
	State(T& value);
	~State();

	void addTransition(const StateTransition<T>& transition) {
		if (std::find(transitions.begin(), transitions.end(), transition) == transitions.end()) {
			statestransitions.push_back(transition);
		}
	}

	bool deleteTransition(StateTransition<T>* transition) {
		std::list<StateTransition<T>>::iterator it = std::find_if(transitions.begin(), transitions.end(),
			[&transition](StateTransition& loaded) { return loaded==*transition; });

		if (it != transitions.end()) {
			erase it;
			return true;
		}
		return false;
	}

	//TODO que variables paso y vigilo
	State<T>* processTransition() const;
	virtual void onTransition() {};

	inline T* getValue() {
		return &value;
	}

protected:
	T value;
	std::list<StateTransition<T>> transitions;
};

template<class T>
State<T>::State(T & value) : value(value) {}

template<class T>
State<T>::~State() {}

template<class T>
State<T>* State<T>::processTransition() const {
	State<T>* result = nullptr;
	for (std::list<StateTransition<T>>::iterator it = transitions.begin();
	it != transitions.end() && result == nullptr; ++it) {
		if ((*it).checkCondition()) {
			result = (*it).getNextState();
		}
	}
	return result;
}

#endif //STATE_H

