#include "StateMachine.h"

StateMachine::StateMachine(int numberStates) : state(0), transitions(numberStates) {
	for (int i = 0; i<numberStates; ++i) {
		transitions[i] = std::vector<Transition>();
	}
}

void StateMachine::addTransition(Transition &transition) {
	transitions[transition.state].push_back(std::move(transition));
}

int StateMachine::processEntry(int entry) {
	std::vector<Transition> stateTransition = transitions[state];
	unsigned int i = 0;
	bool finish = false;
	while (i<stateTransition.size() && !finish) {
		if (stateTransition[i].entry == entry) {
			finish = true;
			state = stateTransition[i].newState;
		}
		++i;
	}
	return (finish) ? state : -1;
}

StateMachine::~StateMachine() {

}
