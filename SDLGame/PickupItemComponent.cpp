#include "PickupItemComponent.h"
#include "Entity.h"
#include "Collider.h"
#include "Items.h"
#include "DataItemComponent.h"
#include "LifeComponent.h"

PickupItemComponent::PickupItemComponent(const std::string & name) :IComponent(name) {}

PickupItemComponent::~PickupItemComponent() {}

bool PickupItemComponent::start() {
	bool result=true;
	life = static_cast<LifeComponent*>(parent->getComponent("life"));
	return result;
}

void PickupItemComponent::onCollisionEnter(const Collider * self, const Collider * another) {
	//check if is the item
	if (another->type != TypeCollider::ITEM) {
		return;
	}

	//get the data of the entity
	DataItemComponent* data = static_cast<DataItemComponent*>(another->parent->getComponent("data"));
	if (data == nullptr) {
		return;
	}

	switch (data->type) {
		case TypeItem::SMALL_FOOD:
		case TypeItem::MEDIUM_FOOD:
		case TypeItem::BIG_FOOD:
			life->modifyActualLife(data->life);
			break;

		case TypeItem::WEAPON:
			
			break;
	}
}

bool PickupItemComponent::cleanUp() {
	life = nullptr;
	return true;
}

IComponent * PickupItemComponent::makeClone() const {
	return new PickupItemComponent(getID());
}
