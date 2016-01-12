#include "DamageComponent.h"
#include "Collider.h"
#include "CollisionComponent.h"
#include "Entity.h"
#include "LifeComponent.h"

DamageComponent::~DamageComponent() {}

IComponent * DamageComponent::makeClone() const {
	DamageComponent* result = new DamageComponent(getID(), normalDamage, specialDamage);
	result->strong = strong;
	return result;
}
