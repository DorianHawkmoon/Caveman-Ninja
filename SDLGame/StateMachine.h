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

#include "State.h"

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

	for (std::list<State<T>*>::iterator it = states.begin(); it != states.end() && result == nullptr; ++it) {
		result = (*it)->processTransition();
	}

	if (result != nullptr) {
		state = result;
		state->onTransition();
	}
}

template<class T>
void StateMachine<T>::addState(State<T> * state) {
	if (std::find(states.begin(), states.end(), state) == states.end()) {
		states.push_back(state);
	}
}
#endif	/* STATE_MACHINE_H */
