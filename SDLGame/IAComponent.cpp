#include "IAComponent.h"

IAComponent::IAComponent(const std::string & name) : IComponent(name) {
	ticks = 0;
	ticked = false;

	functionUpdate = doNothing();
	functionPreUpdate = doNothing();
	functionPostUpdate = doNothing();
}

bool IAComponent::start() {
	ticksPassed = 0;
	return true;
}

update_status IAComponent::preUpdate() {
	ticked = false;
	ticksPassed += App->timer->getDeltaFrame();
	if (ticksPassed > ticks) {
		ticksPassed -= ticks;
		ticked = true;
	}
	functionPreUpdate(this->parent, ticked);

	return UPDATE_CONTINUE;
}

update_status IAComponent::update() {
	functionUpdate(this->parent, ticked);
	return UPDATE_CONTINUE;
}

update_status IAComponent::postUpdate() {
	functionPostUpdate(this->parent, ticked);
	return UPDATE_CONTINUE;
}

IComponent * IAComponent::makeClone() {
	IAComponent* ia = new IAComponent(getID());
	ia->ticks = ticks;
	ia->functionUpdate = functionUpdate;
	ia->functionPreUpdate = functionPreUpdate;
	ia->functionPostUpdate = functionPostUpdate;

	return ia;
}

void IAComponent::clearFunction(std::function<void(Entity*, bool)>& function) {
	function = doNothing();
}
