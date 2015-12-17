#include "CircleCollider.h"

inline bool CircleCollider::checkSpecificCollision(const ICollider * self) const {
	self->checkCollision(this);
}

inline bool CircleCollider::checkCollision(const RectangleCollider * other) const {
	//Colision between rectangle and circle
}

inline bool CircleCollider::checkCollision(const CircleCollider * other) const {
	return this->position.distanceTo(other->position) <= (this->radius + other->radius);
}
