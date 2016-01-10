#pragma once
#ifndef DAMAGE_COMPONENT_H
#define DAMAGE_COMPONENT_H

#include "IComponent.h"
#include <string>
class Collider;

class DamageComponent : public IComponent {
public:
	DamageComponent(const std::string& name) : IComponent(name), normalDamage(0), specialDamage(0) {}
	virtual ~DamageComponent();

	void onCollisionEnter(const Collider * self, const Collider * another);
	IComponent* makeClone();

	int normalDamage;
	int specialDamage;
};

#endif // !DAMAGE_COMPONENT_H

