#pragma once
#ifndef JUMP_COMPONENT_H
#define JUMP_COMPONENT_H

#include "IComponent.h"
#include <string>
#include "MotionComponent.h"

class JumpComponent : public IComponent {
public:
	JumpComponent(std::string nameComponent) : IComponent(nameComponent), speed(1), doubleSpeed(1),cleaned(true) {};
	virtual ~JumpComponent() {};

	bool start();
	update_status preUpdate();
	update_status update();

	bool cleanUp();

	IComponent* makeClone() const;

public:
	float speed;
	float doubleSpeed;

private:
	TypeJump* jump;
	bool jumpAccelerated;
	bool cleaned;
};

#endif // !JUMP_COMPONENT_H

