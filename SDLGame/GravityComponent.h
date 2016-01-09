#pragma once
#ifndef GRAVITY_H
#define GRAVITY_H

#include "IComponent.h"
#include <string>
#include "Entity.h"
#include "Application.h"
#include "ModuleTimer.h"
class MotionComponent;
class CollisionComponent;

class GravityComponent : public IComponent {
public:
	GravityComponent(std::string nameComponent) : IComponent(nameComponent), gravity(0) {}
	virtual ~GravityComponent() {}

	IComponent* makeClone();

	bool start();
	update_status update();
	update_status postUpdate();

	virtual void onCollisionEnter(const Collider* self, const Collider* another);
	virtual void onCollisionExit(const Collider* self, const Collider* another) {};
	virtual void onCollisionStay(const Collider* self, const Collider* another) {
		onCollisionEnter(self, another);
	};

	bool isFalling();

public:
	float gravity;
	float maxVelocity;

private:
	MotionComponent* motion;
	CollisionComponent* collision;
	Collider* gravityCollider;
};

#endif // !GRAVITY_H

