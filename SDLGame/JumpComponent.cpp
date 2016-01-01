#include "JumpComponent.h"

bool JumpComponent::start() {
	jumping = 0;
	//take the state from the controller
	jump = &parent->controller.stateJump;
	return true;
}

update_status JumpComponent::update() {
	switch (*jump) {
		case JUMP:
			if (jumping < height) {
				jumping++;
				MotionComponent* motion = static_cast<MotionComponent*>(parent->getComponent("motion"));
				if (motion != nullptr) {
					motion->velocity.y -= 1 * speed;
				}
			} else {
				*jump = JumpType::FALL;
				jumping = 0;
			}

			break;

			//TODO DOUBLE JUMP
			//TODO JUMP DOWN
		case DOUBLE_JUMP:
		case JUMP_DOWN:
		case NONE:
			break;
	}

	return UPDATE_CONTINUE;
}
