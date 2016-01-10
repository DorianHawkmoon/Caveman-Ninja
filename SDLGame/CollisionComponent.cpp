#include "CollisionComponent.h"
#include "Entity.h"
#include "Application.h"
#include "ModuleCollision.h"

CollisionComponent::CollisionComponent(const std::string & name, Collider * collider) : IComponent(name), collider(collider), cleaned(true){
	
}

CollisionComponent::~CollisionComponent() {
	cleanUp();
	if (collider != nullptr) {
		delete collider;
	}
}

IComponent * CollisionComponent::makeClone() const {
	std::string name = getID();
	IComponent* result = new CollisionComponent(name, collider->clone());
	return result;
}

bool CollisionComponent::start() {
	if (cleaned) {
		collider->addListener(parent);
		collider->parentTransform = parent->transform;
		collider->parent = parent;
		App->collisions->addCollider(collider);
		cleaned = false;
	}
	return true;
}

update_status CollisionComponent::preUpdate() {
	if (toClean) {
		cleanUp();
		toClean = false;
	}
	return UPDATE_CONTINUE;
}

update_status CollisionComponent::update() {
	return UPDATE_CONTINUE;
}

update_status CollisionComponent::postUpdate() {
	return UPDATE_CONTINUE;
}

bool CollisionComponent::cleanUp() {
	if (!cleaned) {
		App->collisions->removeCollider(collider);
		cleaned = true;
	}
	return true;
}
