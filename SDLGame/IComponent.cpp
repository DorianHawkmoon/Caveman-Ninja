#include "IComponent.h"
#include "Entity.h"

IComponent::IComponent(const std::string id) : componentID(id), componentEnabled(true), toClean(false) {}

IComponent::~IComponent() {
}

void IComponent::setParent(Entity * entity) {
	this->parent = entity;
}

inline bool IComponent::isEqual(const IComponent * another) {
	return componentID.compare(another->getID()) == 0;
}
