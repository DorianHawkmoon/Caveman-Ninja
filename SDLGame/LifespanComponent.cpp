#include "LifespanComponent.h"


#include "Entity.h"

LifespanComponent::~LifespanComponent() {}

bool LifespanComponent::start() {
	time.start();
	return true;
}

update_status LifespanComponent::update() {
	if (time.value() > lifespan) {
		parent->destroy();
	}
	return UPDATE_CONTINUE;
}

IComponent * LifespanComponent::makeClone() const {
	return new LifespanComponent(getID(), lifespan);
}
