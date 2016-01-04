#include "Gravity.h"

IComponent * Gravity::makeClone() {
	Gravity* result = new Gravity(this->getID());
	result->gravity = gravity;
	return result;
}

update_status Gravity::update() {
	MotionComponent* motion = static_cast<MotionComponent*>(parent->getComponent("motion"));
	if (!isFalling()) {
		parent->controller.stateJump = JumpType::NONE;
		if (motion != nullptr && motion->velocity.y > 0) {
			motion->velocity.y = 0;
		}
	} else {
		if (motion != nullptr) {
			motion->velocity.y += 1.0f * gravity *(App->timer->getDeltaFrame() / 1000.0f);
		}
	}
	return UPDATE_CONTINUE;
}

bool Gravity::isFalling() const {
	//TODO need to check if has ground (collision) TODO
	return parent->transform.position.y < (256 - 60);
}
