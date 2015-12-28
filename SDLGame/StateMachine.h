#pragma once
#ifndef STATE_MACHINE_H
#define	STATE_MACHINE_H

#include <vector>
#include "StateTransition.h"

/**
* Máquina de estados finitos
*/
class StateMachine {
public:
	/**
	* Constructor con el número de estados totales de la máquina
	* @param numberStates numero de estados que tiene
	*/
	StateMachine(int numberStates);
	virtual ~StateMachine();

	/**
	* Añadir transición a la máquina
	* @param transition
	*/
	void addTransition(Transition &transition);

	/**
	* devuelve el estado actual de la maquina
	* @return estado actual de la maquina
	*/
	inline int getState() { return state; }
	/**
	* Procesa la entrada y devuelve el nuevo estado
	* @param entry entrada para la maquina
	* @return nuevo estado de la máquina si hay transición o -1 si no lo hay
	*/
	int processEntry(int entry);

private:
	/**
	* estado actual de la maquina
	*/
	int state;
	/**
	* lista de transiciones
	*/
	std::vector< std::vector<Transition> > transitions;
};

#endif	/* STATE_MACHINE_H */

