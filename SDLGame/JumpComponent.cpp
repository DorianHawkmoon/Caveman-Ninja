#include "JumpComponent.h"
#include "Application.h"
#include "ModuleTimer.h"
#include "GravityComponent.h"
#include "MotionComponent.h"
#include "Entity.h"

JumpComponent::JumpComponent(const std::string & nameComponent) : IComponent(nameComponent), speed(1), doubleSpeed(1), cleaned(true) {}

bool JumpComponent::start() {
	if (cleaned) {
		jumpAccelerated = false;
		//take the state from the controller
		jump = &parent->controller.stateJump;
		cleaned = false;
	}
	return true;
}

update_status JumpComponent::preUpdate() {
	if (toClean) {
		cleanUp();
		toClean = false;
	}
	return UPDATE_CONTINUE;
}

update_status JumpComponent::update() {
	float finalSpeed = speed;
	switch (*jump) {
		case DOUBLE_JUMP:
			finalSpeed = doubleSpeed;
		case JUMP:
		{
			GravityComponent* gravity = static_cast<GravityComponent*>(parent->getComponent("gravity"));
			MotionComponent* motion = static_cast<MotionComponent*>(parent->getComponent("motion"));
			if (!jumpAccelerated) {
				jumpAccelerated=true;
				if (motion != nullptr) {
					motion->velocity.y = 0;
					motion->velocity.y -= 1 * finalSpeed;
				}
			} else if (motion != nullptr && motion->velocity.y >= 0){
					*jump = TypeJump::FALL;
					jumpAccelerated = false;
			}
			
		}
			break;

		case JUMP_DOWN:
		{
			MotionComponent* motion = static_cast<MotionComponent*>(parent->getComponent("motion"));
			if (!jumpAccelerated) {
				jumpAccelerated = true;
				if (motion != nullptr) {
					motion->velocity.y += 1;
				}
			}
		}
			break;
		case FALL:
			jumpAccelerated = false;
		case NONE:
			break;
	}

	return UPDATE_CONTINUE;
}

bool JumpComponent::cleanUp() {
	if (!cleaned) {
		jumpAccelerated = false;
		cleaned = true;
	}
	return true;
}

IComponent * JumpComponent::makeClone() const {
	return nullptr;
}
