#pragma once
#ifndef GRAVITY_H
#define GRAVITY_H

#include "IComponent.h"
#include "MotionComponent.h"
#include <string>
#include "Entity.h"

class Gravity :	public IComponent {
public:
	Gravity(std::string nameComponent) : IComponent(nameComponent), gravity(0) {}
	~Gravity() {}

	IComponent* makeClone();

	update_status update() {
		if (!isFalling()) {
			parent->controller.stateJump = JumpType::NONE;
		} else {
			MotionComponent* motion = static_cast<MotionComponent*>(parent->getComponent("motion"));
			if (motion != nullptr) {
				motion->velocity.y += 1 * gravity;
			}
		}
		return UPDATE_CONTINUE;
	};
	
	bool isFalling() const {
		//need to check if has ground (collision) TODO
		return parent->transform.position.y < (256 - 30);
	}

public:
	float gravity;
};

#endif // !GRAVITY_H

