#pragma once
#ifndef WEAPON_COMPONENT_H
#define WEAPON_COMPONENT_H

#include "IComponent.h"
#include "MotionComponent.h"
#include "CollisionComponent.h"
#include "Timer.h"
#include "Application.h"
#include "PlayerWeapon.h"
#include "Entity.h"
#include "ModuleScene.h"

class WeaponComponent : public IComponent {
public:
	WeaponComponent(const std::string& name, int maxThrowable = -1, unsigned int delay = 0) :IComponent(name), maxThrowable(maxThrowable), delay(delay), weapons() {}
	virtual ~WeaponComponent();

	bool start();

	virtual update_status preUpdate();

	virtual update_status update() {
		return UPDATE_CONTINUE;
	}

	IComponent* makeClone();

	bool limitReached() const;

	void throwWeapon();

private:
	Timer time;
	int maxThrowable;
	unsigned int delay;
	std::list<Entity*> weapons;
};


#endif // !WEAPON_COMPONENT_H
