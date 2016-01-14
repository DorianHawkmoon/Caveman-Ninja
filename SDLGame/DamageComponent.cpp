#include "DamageComponent.h"
#include "Collider.h"
#include "CollisionComponent.h"
#include "Entity.h"
#include "LifeComponent.h"

DamageComponent::DamageComponent(const std::string & name, int normalDamage, int specialDamage) : IComponent(name), normalDamage(normalDamage), specialDamage(specialDamage), strong(false) {}

DamageComponent::~DamageComponent() {}

IComponent * DamageComponent::makeClone() const {
	DamageComponent* result = new DamageComponent(getID(), normalDamage, specialDamage);
	result->strong = strong;
	return result;
}

int DamageComponent::getDamage() const {
	return (strong) ? specialDamage : normalDamage;
}
