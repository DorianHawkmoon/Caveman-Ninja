#pragma once
#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include "IComponent.h"
#include "Collider.h"
#include <string>

class CollisionComponent : public IComponent {
public:
	CollisionComponent(const std::string& name, Collider *collider);
	virtual ~CollisionComponent();

	virtual IComponent* makeClone() const;

	virtual bool start();
	virtual update_status preUpdate();
	virtual update_status update();
	virtual update_status postUpdate();
	virtual bool cleanUp();

	const Collider* getCollider() const {
		return collider;
	}

private:
	Collider* collider;

	bool cleaned;
};

#endif // !COLLISION_COMPONENT_H

