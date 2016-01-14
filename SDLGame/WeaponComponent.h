#pragma once
#ifndef WEAPON_COMPONENT_H
#define WEAPON_COMPONENT_H

#include "IComponent.h"
#include "MotionComponent.h"
#include "CollisionComponent.h"
#include "Timer.h"
#include "Application.h"
#include "PlayerWeapon.h"
#include "ModuleScene.h"
class Entity;

class WeaponComponent : public IComponent {
public:
	WeaponComponent(const std::string& name, int maxThrowable = -1, unsigned int delay = 0);
	virtual ~WeaponComponent();

	virtual bool start();
	virtual update_status preUpdate();

	virtual IComponent* makeClone() const;

	bool limitReached() const;
	void throwWeapon();

private:
	Timer time;
	int maxThrowable;
	unsigned int delay;
	std::list<Entity*> weapons;

	unsigned int soundEffect;
};


#endif // !WEAPON_COMPONENT_H
