#include "IAComponent.h"
#include "Application.h"
#include "ModuleTimer.h"

IAComponent::IAComponent(const std::string & name) : IComponent(name) {
	ticks = 0;
	ticked = false;
}

IAComponent::~IAComponent() {

}

bool IAComponent::start() {
	ticksPassed = ticks;
	return true;
}

update_status IAComponent::preUpdate() {
	ticked = true;
	ticksPassed += App->timer->getDeltaFrame();
	if (ticksPassed > ticks) {
		ticksPassed -= ticks;
		ticked = true;
	}
	delegatedPreUpdate();

	return UPDATE_CONTINUE;
}


bool IAComponent::cleanUp() {
	return false;
}

IComponent * IAComponent::makeClone() const {
	IAComponent* ia = new IAComponent(getID());
	ia->ticks = ticks;

	return ia;
}
