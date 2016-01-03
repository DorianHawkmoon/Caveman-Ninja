#include "MotionComponent.h"
#include "Application.h"
#include "ModuleTimer.h"


MotionComponent::MotionComponent(std::string nameComponent): IComponent(nameComponent) {
	velocity.x = 0;
	velocity.y = 0;
}


MotionComponent::~MotionComponent() {}


update_status MotionComponent::update() {
	float seconds = App->timer->getDeltaFrame()/1000.0f;
    parent->transform.position.x += velocity.x * seconds;
	parent->transform.position.y += velocity.y * seconds;
	return UPDATE_CONTINUE;
}

IComponent * MotionComponent::makeClone() {
	std::string name = getID();
	MotionComponent* result = new MotionComponent(name);
	result->velocity = velocity;
	return result;
}
