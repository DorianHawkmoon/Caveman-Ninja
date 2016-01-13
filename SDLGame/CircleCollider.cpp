#include "CircleCollider.h"
#include "RectangleCollider.h"
#include "LineCollider.h"
#include "Transform.h"

CircleCollider::CircleCollider(fPoint position, float radius, TypeCollider type) : Collider(position, type), radius(radius) {

}

CircleCollider::~CircleCollider() {}

void CircleCollider::paintCollider() const {
	//get global
	Transform global = getGlobalTransform();

	// Determina el color y opacidad del dibujo
	SDL_Color renderColor;
	renderColor.r = 0;
	renderColor.g = 179;
	renderColor.b = 0;
	renderColor.a = 100;

	App->renderer->paintCircle(renderColor, global.position, radius);
}

Collider * CircleCollider::clone() const {
	CircleCollider* result = new CircleCollider(position, radius, type);
	return Collider::clone(result);
}

fPoint CircleCollider::getGlobalPoint() const {
	fPoint global = position;
	if (parentTransform != nullptr) {
		global += parentTransform->getGlobalTransform().position;
	}
	return global;
}

bool CircleCollider::checkSpecificCollision(const Collider * self) const {
	return self->checkCollision(this);
}

bool CircleCollider::checkCollision(const RectangleCollider * other) const {
	return other->checkCollision(this);
}

bool CircleCollider::checkCollision(const CircleCollider * other) const {
	fPoint globalOther = other->getGlobalPoint();
	fPoint global = getGlobalPoint();
	return global.distanceTo(globalOther) <= (this->radius + other->radius);
}

bool CircleCollider::checkCollision(const LineCollider * other) const { return other->checkCollision(this); }
