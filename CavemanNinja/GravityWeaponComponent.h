#pragma once
#ifndef GRAVITY_WEAPON_COMPONENT_H
#define GRAVITY_WEAPON_COMPONENT_H

#include "IComponent.h"
#include "Timer.h"
class MotionComponent;
class CollisionComponent;

class GravityWeaponComponent : public IComponent {
public:
	GravityWeaponComponent(std::string nameComponent);
	virtual ~GravityWeaponComponent();

	virtual IComponent * makeClone() const;
	virtual bool start();
	virtual update_status update();

public:
	float gravity;
	float maxVelocity;

private:
	MotionComponent* motion;
	CollisionComponent* collision;
	Timer* timer;
};


#endif // !GRAVITY_WEAPON_COMPONENT_H
