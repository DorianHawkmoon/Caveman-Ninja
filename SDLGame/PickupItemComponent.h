#pragma once
#ifndef PICKUP_ITEM_COMPONENT_H
#define PICKUP_ITEM_COMPONENT_H

#include "IComponent.h"
class Collider;
#include <string>
class LifeComponent;

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
};

#endif // !PICKUP_ITEM_COMPONENT_H

