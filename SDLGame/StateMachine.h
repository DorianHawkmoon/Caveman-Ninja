#pragma once
#ifndef STATE_MACHINE_H
#define	STATE_MACHINE_H

#include <vector>
#include "StateTransition.h"

/**
* M�quina de estados finitos
*/
class StateMachine {
public:
	/**
	* Constructor con el n�mero de estados totales de la m�quina
	* @param numberStates numero de estados que tiene
	*/
	StateMachine(int numberStates);
	virtual ~StateMachine();

	/**
	* A�adir transici�n a la m�quina
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
	* @return nuevo estado de la m�quina si hay transici�n o -1 si no lo hay
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

