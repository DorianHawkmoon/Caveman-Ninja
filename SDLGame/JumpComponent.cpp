#include "JumpComponent.h"
#include "Application.h"
#include "ModuleTimer.h"

bool JumpComponent::start() {
	jumpAccelerated = false;
	//take the state from the controller
	jump = &parent->controller.stateJump;
	return true;
}

update_status JumpComponent::update() {
	float finalSpeed = speed;
	switch (*jump) {
		case DOUBLE_JUMP:
			finalSpeed = doubleSpeed;
		case JUMP:
		{
			MotionComponent* motion = static_cast<MotionComponent*>(parent->getComponent("motion"));
			if (!jumpAccelerated) {
				jumpAccelerated=true;
				if (motion != nullptr) {
					motion->velocity.y -= 1 * finalSpeed;
				}
			} else {
				if (motion != nullptr && motion->velocity.y >= 0) {
						*jump = JumpType::FALL;
						jumpAccelerated = false;
				
				}
			}
		}
			break;

			//TODO JUMP DOWN
		case JUMP_DOWN:
		{
			MotionComponent* motion = static_cast<MotionComponent*>(parent->getComponent("motion"));
			if (!jumpAccelerated) {
				jumpAccelerated = true;
				if (motion != nullptr) {
					motion->velocity.y += 1 * (finalSpeed/2);
				}
			} else {
				/*if (motion != nullptr && motion->velocity.y >= 0) {
					*jump = JumpType::FALL;
					jumpAccelerated = false;

				}*/
			}
		}
			break;
		case NONE:
			break;
	}

	return UPDATE_CONTINUE;
}

bool JumpComponent::cleanUp() {
	jumpAccelerated = false;
	return true;
}

IComponent * JumpComponent::makeClone() {
	return nullptr;
}
