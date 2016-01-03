#pragma once
#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include "IComponent.h"
#include "Collider.h"
#include <string>
#include "CollisionListener.h"

class CollisionComponent : public IComponent, CollisionListener {
public:
	CollisionComponent(const std::string& name, Collider *collider);
	~CollisionComponent();

	virtual IComponent* makeClone();

	virtual bool start();
	virtual update_status update();
	virtual update_status postUpdate();

private:
	Collider* collider;
};

#endif // !COLLISION_COMPONENT_H

