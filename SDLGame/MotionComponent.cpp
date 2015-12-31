#include "MotionComponent.h"



MotionComponent::MotionComponent(std::string nameComponent): IComponent(nameComponent) {
	velocity.x = 0;
	velocity.y = 0;
}


MotionComponent::~MotionComponent() {}


update_status MotionComponent::update() {
	parent->transform.position += velocity;
	return UPDATE_CONTINUE;
}
