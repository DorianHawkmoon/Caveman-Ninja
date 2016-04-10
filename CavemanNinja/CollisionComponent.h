#pragma once
#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include "IComponent.h"
#include <string>
class Collider;

class CollisionComponent : public IComponent {
public:
	CollisionComponent(const std::string& name, Collider *collider);
	virtual ~CollisionComponent();

	virtual IComponent* makeClone() const;

	virtual bool start();
	virtual update_status preUpdate();
	virtual bool cleanUp();

	const Collider* getCollider() const;

private:
	Collider* collider;

	bool cleaned;
};

#endif // !COLLISION_COMPONENT_H

