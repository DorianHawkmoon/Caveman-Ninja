#include "DestroyOnCollisionComponent.h"
#include "Collider.h"
#include "CollisionComponent.h"
#include "Entity.h"


DestroyOnCollisionComponent::~DestroyOnCollisionComponent() {}

IComponent * DestroyOnCollisionComponent::makeClone() const {
	return new DestroyOnCollisionComponent(getID());
}

void DestroyOnCollisionComponent::onCollisionEnter(const Collider * self, const Collider * another) {
	//check its me
	CollisionComponent* collider = static_cast<CollisionComponent*>(parent->getComponent("collider"));
	if (self != collider->getCollider()) {
		return;
	}

	//destroy myself
	parent->destroy();
}
