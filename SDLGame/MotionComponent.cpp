#include "MotionComponent.h"



MotionComponent::MotionComponent(std::string nameComponent): IComponent(nameComponent) {
	velocity.x = 0;
	velocity.y = 0;
}


MotionComponent::~MotionComponent() {}

update_status MotionComponent::preUpdate() {
	return update_status();
}

update_status MotionComponent::update() {
	parent->transform.position += velocity;
	return update_status();
}

update_status MotionComponent::postUpdate() {
	return update_status();
}
