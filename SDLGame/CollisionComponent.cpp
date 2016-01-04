#include "CollisionComponent.h"
#include "Entity.h"
#include "Application.h"
#include "ModuleCollision.h"

CollisionComponent::CollisionComponent(const std::string & name, Collider * collider) : IComponent(name), collider(collider) {
	collider->addListener(this);
}

CollisionComponent::~CollisionComponent() {
	cleanUp();
	if (collider != nullptr) {
		delete collider;
	}
}

IComponent * CollisionComponent::makeClone() {
	std::string name = getID();
	IComponent* result = new CollisionComponent(name, collider->clone());
	return result;
}

bool CollisionComponent::start() {
	App->collisions->addCollider(collider);
	return true;
}

update_status CollisionComponent::update() {
	if (parent != nullptr) {
		collider->position = parent->transform.position;
	}
	return UPDATE_CONTINUE;
}

update_status CollisionComponent::postUpdate() {
	collider->paintCollider();
	return UPDATE_CONTINUE;
}

bool CollisionComponent::cleanUp() {
	App->collisions->removeCollider(collider);
	return true;
}
