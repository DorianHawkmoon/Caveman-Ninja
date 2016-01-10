#pragma once
#ifndef GRAVITY_H
#define GRAVITY_H

#include "IComponent.h"
#include <string>
#include "Entity.h"
#include "Application.h"
#include "ModuleTimer.h"
class MotionComponent;
class Collider;

class GravityComponent : public IComponent {
public:
	GravityComponent(std::string nameComponent, Collider *collider);
	virtual ~GravityComponent();

	IComponent* makeClone() const;

	bool start();
	update_status preUpdate();
	update_status update();
	update_status postUpdate();

	virtual void onCollisionEnter(const Collider* self, const Collider* another);
	virtual void onCollisionExit(const Collider* self, const Collider* another) {};
	virtual void onCollisionStay(const Collider* self, const Collider* another);;

	bool isFalling();

	bool cleanUp();

public:
	float gravity;
	float maxVelocity;

private:
	MotionComponent* motion;
	//CollisionComponent* collision;
	Collider* collision;
	Collider* gravityCollider;

	bool cleaned;
};

#endif // !GRAVITY_H

