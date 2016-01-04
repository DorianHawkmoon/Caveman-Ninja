#include "CircleCollider.h"
#include "RectangleCollider.h"

CircleCollider::~CircleCollider() {}

Collider * CircleCollider::clone() {
	CircleCollider* result = new CircleCollider(position, radius, type);
	return Collider::clone(result);
}

bool CircleCollider::checkSpecificCollision(const Collider * self) const {
	return self->checkCollision(this);
}

bool CircleCollider::checkCollision(const RectangleCollider * other) const {
	return other->checkCollision(this);
}

bool CircleCollider::checkCollision(const CircleCollider * other) const {
	return this->position.distanceTo(other->position) <= (this->radius + other->radius);
}