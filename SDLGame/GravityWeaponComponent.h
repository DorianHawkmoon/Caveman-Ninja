#pragma once
#ifndef GRAVITY_WEAPON_COMPONENT_H
#define GRAVITY_WEAPON_COMPONENT_H

#include "IComponent.h"
class MotionComponent;
class CollisionComponent;

class GravityWeaponComponent : public IComponent {
public:
	GravityWeaponComponent(std::string nameComponent) : IComponent(nameComponent), gravity(0) {}
	virtual ~GravityWeaponComponent();

	IComponent * makeClone();
	bool start();
	update_status update();

public:
	float gravity;
	float maxVelocity;

private:
	MotionComponent* motion;
	CollisionComponent* collision;

};


#endif // !GRAVITY_WEAPON_COMPONENT_H
