#pragma once
#ifndef JUMP_COMPONENT_H
#define JUMP_COMPONENT_H

#include "IComponent.h"
#include <string>
#include "MotionComponent.h"

class JumpComponent : public IComponent {
public:
	JumpComponent(std::string nameComponent) : IComponent(nameComponent), speed(1), doubleSpeed(1) {};
	~JumpComponent() {};

	bool start();
	
	update_status update();

	bool cleanUp();

	IComponent* makeClone();

public:
	float speed;
	float doubleSpeed;

private:
	JumpType* jump;
	bool jumpAccelerated;
};

#endif // !JUMP_COMPONENT_H

