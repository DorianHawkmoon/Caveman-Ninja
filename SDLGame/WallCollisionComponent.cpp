#include "WallCollisionComponent.h"

WallCollisionComponent::WallCollisionComponent(const std::string & name) :IComponent(name) {}

IComponent * WallCollisionComponent::makeClone() const {
	return new WallCollisionComponent(getID());
}

void WallCollisionComponent::onCollisionEnter(const Collider * self, const Collider * another) {
	// Primero, detecta si la colisión es con una pared
	if (another->type != TypeCollider::WALL) {
		return;
	}

	Transform anotherTrans = another->getGlobalTransform();
	Transform thisTrans = self->getGlobalTransform();

	fPoint offset = {(anotherTrans.position.x < 0) ? anotherTrans.position.x : 0,
		(anotherTrans.position.y < 0) ? anotherTrans.position.x : 0};
	offset.x = (thisTrans.position.x<0) ? thisTrans.position.x : offset.x;
	offset.y = (thisTrans.position.y<0) ? thisTrans.position.y : offset.y;
	offset.x *= -1;
	offset.y *= -1;

	fPoint wallRelativeDirection = (anotherTrans.position + offset) - (thisTrans.position + offset);
	MotionComponent* motion = static_cast<MotionComponent*>(parent->getComponent("motion"));

	bool onX = true;
	if (wallRelativeDirection.x * motion->velocity.x <= 0) {	// Si dan menor que 0, tienen signo distinto, luego son direcciones opuestas
		onX = false;
	}
	// quieto bicho!
	if (onX) {
		motion->velocity.x = 0;
		int stepDirection = (wallRelativeDirection.x > 0) ? -1 : 1;
		//put the entity over the collision again
		int count = 0;
		do {
			parent->transform->position.x += stepDirection;
		} while (self->checkCollision(another) && ++count < 100);
	}
}

void WallCollisionComponent::onCollisionStay(Collider * self, Collider * another) {
	onCollisionEnter(self, another);
}
