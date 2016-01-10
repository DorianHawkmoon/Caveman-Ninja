#include "DamageComponent.h"
#include "Collider.h"
#include "CollisionComponent.h"
#include "Entity.h"
#include "LifeComponent.h"

DamageComponent::~DamageComponent() {}

void DamageComponent::onCollisionEnter(const Collider * self, const Collider * another) {
	//check its me
	CollisionComponent* collider = static_cast<CollisionComponent*>(parent->getComponent("collider"));
	if (self != collider->getCollider()) {
		return;
	}

	//get the other's life and do normal damage
	if (another->parent == nullptr) {
		return;
	}
	LifeComponent* life=static_cast<LifeComponent*>(another->parent->getComponent("life"));
	if (life != nullptr) {
		life->modifyActualLife(-normalDamage);
		LOG("player damage!");
	}

	//TODO check myself and see if i do normal or special damage
}

IComponent * DamageComponent::makeClone() {
	DamageComponent* result = new DamageComponent(getID());
	result->normalDamage = normalDamage;
	result->specialDamage = specialDamage;
	return result;
}
