#pragma once
#ifndef JUMP_COMPONENT_H
#define JUMP_COMPONENT_H

#include "IComponent.h"
#include <string>
#include "MotionComponent.h"

class JumpComponent : public IComponent {
public:
	JumpComponent(std::string nameComponent, int height, int doubleHeight) : IComponent(nameComponent), height(height), doubleHeight(doubleHeight) {};
	~JumpComponent() {};

	bool start();
	
	update_status update();

	bool cleanUp() { 
		jumping = 0;
		return true;
	};

	IComponent* makeClone() {
		return nullptr;
	};

public:
	float speed;

private:
	JumpType* jump;
	int height; //normal jump
	int doubleHeight; //double jump

	int jumping; //height jumped
};

#endif // !JUMP_COMPONENT_H

