#include "CircleCollider.h"
#include "RectangleCollider.h"
#include "Transform.h"

CircleCollider::~CircleCollider() {}

void CircleCollider::paintCollider() const {
	// Determina el color y opacidad del dibujo
	SDL_Color renderColor;
	renderColor.r = 0;
	renderColor.g = 179;
	renderColor.b = 0;
	renderColor.a = 128;

	//const SDL_Texture* texture, const SDL_Color& color, const iPoint& position, float radius, float speed = 1.0f
	App->renderer->paintCircle(renderColor, position, radius);
}

Collider * CircleCollider::clone() {
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