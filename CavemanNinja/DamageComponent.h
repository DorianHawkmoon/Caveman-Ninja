#pragma once
#ifndef DAMAGE_COMPONENT_H
#define DAMAGE_COMPONENT_H

#include "IComponent.h"
#include <string>
class Collider;

class DamageComponent : public IComponent {
public:
	DamageComponent(const std::string& name, int normalDamage, int specialDamage);
	virtual ~DamageComponent();

	virtual IComponent* makeClone() const;

	int getDamage() const;

	bool strong;
private:
	int normalDamage;
	int specialDamage;
};

#endif // !DAMAGE_COMPONENT_H

