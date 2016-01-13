#pragma once
#ifndef DAMAGE_COMPONENT_H
#define DAMAGE_COMPONENT_H

#include "IComponent.h"
#include <string>
class Collider;

class DamageComponent : public IComponent {
public:
	DamageComponent(const std::string& name, int normalDamage, int specialDamage) : IComponent(name), normalDamage(normalDamage), specialDamage(specialDamage), strong(false) {}
	virtual ~DamageComponent();

	virtual IComponent* makeClone() const;

	int getDamage() const {
		return (strong) ? specialDamage : normalDamage;
	}

	bool strong;
private:
	int normalDamage;
	int specialDamage;
};

#endif // !DAMAGE_COMPONENT_H

