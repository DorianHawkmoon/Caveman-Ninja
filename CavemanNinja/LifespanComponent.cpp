#include "LifespanComponent.h"
#include "Entity.h"
#include "ModuleTimer.h"
#include "Application.h"
#include "Timer.h"

inline LifespanComponent::LifespanComponent(const std::string & name, unsigned int lifespan) : IComponent(name), lifespan(lifespan) {
	time = App->timer->getTimer();
}

LifespanComponent::~LifespanComponent() {
	App->timer->deleteTimer(time);
}

bool LifespanComponent::start() {
	time->start();
	return true;
}

update_status LifespanComponent::update() {
	if (time->value() > lifespan) {
		parent->destroy();
	}
	return UPDATE_CONTINUE;
}

IComponent * LifespanComponent::makeClone() const {
	return new LifespanComponent(getID(), lifespan);
}
