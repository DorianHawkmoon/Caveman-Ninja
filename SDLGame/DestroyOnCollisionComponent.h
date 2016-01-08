#pragma once
#ifndef DESTROY_ON_COLLISION_COMPONENT_H
#define DESTROY_ON_COLLISION_COMPONENT_H

#include "IComponent.h"
#include <string>

class DestroyOnCollisionComponent : public IComponent {
public:
	DestroyOnCollisionComponent(const std::string& name) : IComponent(name) {}
	virtual ~DestroyOnCollisionComponent();

	IComponent* makeClone() {
		return new DestroyOnCollisionComponent(getID());
	}
	void onCollisionEnter(const Collider* self, const Collider* another);
};

#endif // !DESTROY_ON_COLLISION_COMPONENT_H

