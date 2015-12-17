#include "IComponent.h"

IComponent::IComponent(const std::string id) : componentID(id) {}

IComponent::~IComponent() {}

inline bool IComponent::isEqual(const IComponent * another) {
	return componentID.compare(another->getID()) == 0;
}
