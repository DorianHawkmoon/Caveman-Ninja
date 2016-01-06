#pragma once
#ifndef GRAVITY_H
#define GRAVITY_H

#include "IComponent.h"
#include "MotionComponent.h"
#include <string>
#include "Entity.h"
#include "Application.h"
#include "ModuleTimer.h"

class Gravity :	public IComponent {
public:
	Gravity(std::string nameComponent) : IComponent(nameComponent), gravity(0) {}
	virtual ~Gravity() {}

	IComponent* makeClone();

	update_status update();

	virtual void onCollisionEnter(Collider* one, Collider* another);
	virtual void onCollisionExit(Collider* one, Collider* another) {};
	virtual void onCollisionStay(Collider* one, Collider* another) {};

	bool isFalling();

public:
	float gravity;
	float maxVelocity;
};

#endif // !GRAVITY_H
