#pragma once
#ifndef WALL_COLLISION_COMPONENT_H
#define WALL_COLLISION_COMPONENT_H

#include "IComponent.h"
#include <string>
#include "Collider.h"
#include "Entity.h"
#include "MotionComponent.h"

class WallCollisionComponent : public IComponent {
public:
	WallCollisionComponent(const std::string& name);
	virtual ~WallCollisionComponent() {}

	virtual IComponent* makeClone() const;

	virtual void onCollisionEnter(const Collider* self, const Collider* another);;

	virtual void onCollisionStay(Collider* self, Collider* another);;
};


#endif // !WALL_COLLISION_COMPONENT_H
