#pragma once
#ifndef STATE_MACHINE_H
#define	STATE_MACHINE_H

#include <list>
#include "StateTransition.h"
template <class T>
class State;

/**
* Máquina de estados finitos
*/
template <class T>
class StateMachine {
public:
	/**
	* Constructor con el número de estados totales de la máquina
	* @param numberStates numero de estados que tiene
	*/
	StateMachine<T>(State<T>* initialState);
	virtual ~StateMachine();

	/**
	* devuelve el estado actual de la maquina
	* @return estado actual de la maquina
	*/
	inline State<T>* getState() { return state; }
	void proccessState();
	void addState(State<T>* state);

private:
	std::list<State<T> *> states;
	/**
	* estado actual de la maquina
	*/
	State<T>* state;
};

template<class T>
StateMachine<T>::StateMachine(State<T> * initialState) : state(initialState) {
	states.push_back(state);
}

template<class T>
StateMachine<T>::~StateMachine() {

}

template<class T>
void StateMachine<T>::proccessState() {
	State<T>* result = nullptr;

	for (std::list<const State*>::iterator it = states.begin; it != states.end() && result == nullptr; ++it) {
		result = (*it)->process();
	}

	if (result != nullptr) {
		state = result;
		state->onTransition();
	}
}

template<class T>
void StateMachine<T>::addState(State<T> * state) {
	if (std::find(states.begin(), states.end(), state) == state.end()) {
		states.push_back(state);
	}
}
#endif	/* STATE_MACHINE_H */
