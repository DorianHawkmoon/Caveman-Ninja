#include "MotionComponent.h"
#include "Application.h"
#include "ModuleTimer.h"


MotionComponent::MotionComponent(const std::string& nameComponent): IComponent(nameComponent), velocity(0,0), speed(0) {
}


MotionComponent::~MotionComponent() {}


update_status MotionComponent::update() {
	float seconds = App->timer->getDeltaFrame()/1000.0f;
    parent->transform->position.x += velocity.x * seconds;
	parent->transform->position.y += velocity.y * seconds;
	return UPDATE_CONTINUE;
}

IComponent * MotionComponent::makeClone() const {
	std::string name = getID();
	MotionComponent* result = new MotionComponent(name);
	result->velocity = velocity;
	return result;
}
