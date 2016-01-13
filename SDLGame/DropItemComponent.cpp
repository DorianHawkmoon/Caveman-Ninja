#include "DropItemComponent.h"
#include "Entity.h"

DropItemComponent::DropItemComponent(const std::string & name, const Entity * item) : IComponent(name), item(item) {}

DropItemComponent::~DropItemComponent() {
	if (item != nullptr) {
		delete item;
	}
}

IComponent * DropItemComponent::makeClone() const {
	Entity* cloned = (item != nullptr) ? item->clone() : nullptr;
	return new DropItemComponent(getID(), item->clone());
}
