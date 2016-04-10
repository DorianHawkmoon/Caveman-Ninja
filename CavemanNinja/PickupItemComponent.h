#pragma once
#ifndef PICKUP_ITEM_COMPONENT_H
#define PICKUP_ITEM_COMPONENT_H

#include "IComponent.h"
#include <string>
class Collider;
class LifeComponent;
class ScoreComponent;

class PickupItemComponent :	public IComponent {
public:
	PickupItemComponent(const std::string& name);
	virtual ~PickupItemComponent();

	virtual bool start();
	virtual void onCollisionEnter(const Collider* self, const Collider* another);
	virtual bool cleanUp();

	IComponent* makeClone() const;

private:
	LifeComponent* life;
	ScoreComponent* score;
	unsigned int eatSound;
};

#endif // !PICKUP_ITEM_COMPONENT_H

