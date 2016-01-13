#pragma once
#ifndef GRAVITY_H
#define GRAVITY_H

#include "IComponent.h"
#include <string>
#include "Entity.h"
#include "Application.h"
#include "ModuleTimer.h"
class MotionComponent;
class LifeComponent;
class Collider;

class GravityComponent : public IComponent {
public:
	GravityComponent(const std::string& nameComponent, Collider *collider);
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
	LifeComponent* life;
	Collider* collision;
	Collider* gravityCollider;
	Timer fallTime;

	bool cleaned;

protected:
	virtual void loadSound() {
		
	}
	virtual void playSound() {
		
	}
};

#endif // !GRAVITY_H

