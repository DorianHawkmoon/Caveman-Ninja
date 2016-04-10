#include "Trigger.h"

#include "Entity.h"
#include "RectangleCollider.h"
#include "CollisionComponent.h"
#include "DestroyOnCollisionComponent.h"
#include "OnCollisionTrigger.h"

Entity * Trigger::makeTrigger(const fPoint& position, const iPoint& size, const std::function<void()>& functionCheck) {
	Entity* result = new Entity();

	RectangleCollider* rectangle = new RectangleCollider(position, size, 0, TypeCollider::TRIGGER);
	CollisionComponent* collider = new CollisionComponent("collider", rectangle);
	result->addComponent(collider);

	OnCollisionTrigger* trigger = new OnCollisionTrigger("trigger", functionCheck);
	result->addComponent(trigger);


	DestroyOnCollisionComponent* destroy = new DestroyOnCollisionComponent("destroyOnCollision");
	result->addComponent(destroy);

	return result;
}
