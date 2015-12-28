#pragma once
#ifndef TRANSITION_H
#define TRANSITION_H
/**
* Estructura de transición para la máquina de estados finitos
*/
struct Transition {
	Transition(int state, int entry, int newState) :
		state(state), entry(entry), newState(newState) {};

	/**
	* Estado previo
	*/
	int state;
	/**
	* Entrada
	*/
	int entry;
	/**
	* Nuevo estado
	*/
	int newState;
};
#endif // TRANSITION_H