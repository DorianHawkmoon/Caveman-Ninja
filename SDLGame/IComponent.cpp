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

void IComponent::enable() {
	if (!componentEnabled) {
		componentEnabled = true;
		start();
	}
}

void IComponent::disable() {
	if (componentEnabled) {
		componentEnabled = false;
		toClean = true;
	}
}

bool IComponent::isEnable() {
	return (componentEnabled || toClean);
}
