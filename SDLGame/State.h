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

	void addTransition(StateTransition<T>* transition) {
		if (std::find(transitions.begin(), transitions.end(), transition) == transitions.end()) {
			transitions.push_back(transition);
		}
	}

	bool deleteTransition(StateTransition<T>* transition) {
		//TODO la otra funcion
		std::list<StateTransition<T>>::iterator it = std::find_if(transitions.begin(), transitions.end(),
			[&transition](StateTransition& loaded) { return loaded==transition; });

		if (it != transitions.end()) {
			erase it;
			return true;
		}
		return false;
	}

	State<T>* processTransition() const;
	void onTransition(State<T>* newState) {
		cleanUp();
		//value.cleanUp();
		newState->getValue()->start();
		newState->start();
	};
	bool start();
	bool cleanUp();
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
State<T>::~State() {}

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

#include "Animation.h"
void State<Animation>::onTransition(State<Animation>* newState){
	//LOG("Cambio de estado");
	cleanUp();
	value.cleanUp();
	newState->getValue()->start();
	newState->start();
}

#endif //STATE_H

