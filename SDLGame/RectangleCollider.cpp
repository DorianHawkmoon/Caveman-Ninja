#include "RectangleCollider.h"
#include "Application.h"
#include "ModuleRender.h"
#include "CircleCollider.h"
#include "SDL\SDL_rect.h"
#include "Transform.h"

RectangleCollider::RectangleCollider(fPoint& position, iPoint& rectangle, float rotation, TypeCollider type) : Collider(position, type), rect(rectangle), rotation(rotation) {}

void RectangleCollider::paintCollider() const {
	//get global
	Transform global = getGlobalTransform();
	global.position += position;
	global.rotation += rotation;
	SDL_Color color;
	color.b = 255;
	color.r = 255;
	color.g = 255;
	color.a = 150;

	if (global.rotation == 0) {
		iPoint pos(static_cast<int>(global.position.x), static_cast<int>(global.position.y));

		App->renderer->paintRectangle(color, pos, rect);
	} else {
		//const SDL_Color & color, const Transform & transform, const iPoint & rect, float speed
		App->renderer->paintRectangle(color, global, rect);
	}
}


Collider * RectangleCollider::clone() {
	RectangleCollider* result = new RectangleCollider(position, rect, rotation, type);
	return Collider::clone(result);
}

SDL_Rect RectangleCollider::getGlobalRectangle() const {
	fPoint global = position;
	if (parentTransform != nullptr) {
		global += parentTransform->getGlobalTransform().position;
	}
	
	SDL_Rect result = {static_cast<int>(global.x),
					static_cast<int>(global.y),
					static_cast<int>(rect.x),
					static_cast<int>(rect.y)};
	return result;
}

bool RectangleCollider::checkSpecificCollision(const Collider * self) const {
	return  self->checkCollision(this);
}

bool RectangleCollider::checkCollision(const RectangleCollider * other) const {
	const SDL_Rect rectOther = other->getGlobalRectangle();
	const SDL_Rect rectThis = getGlobalRectangle();
	return SDL_HasIntersection(&rectThis, &rectOther) == SDL_TRUE;
}

bool RectangleCollider::checkCollision(const CircleCollider * other) const {
	fPoint globalCircle = other->getGlobalPoint();

	//Closest point on collision box
	iPoint closestPoint;
	iPoint circle(static_cast<int>(globalCircle.x), 
				static_cast<int>(globalCircle.y));
	SDL_Rect positionRect = getGlobalRectangle();

	//Find closest x offset
	if (circle.x < positionRect.x) {
		closestPoint.x = static_cast<int>(positionRect.x);
	} else if (circle.x > positionRect.x + positionRect.w) { 
		closestPoint.x = static_cast<int>(positionRect.x + positionRect.w);
	} else {
		closestPoint.x = circle.x;
	}

	//Find closest y offset
	if (circle.y < positionRect.y) {
		closestPoint.y = static_cast<int>(positionRect.y);
	} else if (circle.y > positionRect.y + positionRect.h) {
		closestPoint.y = static_cast<int>(positionRect.y + positionRect.h);
	} else {
		closestPoint.y = circle.y;
	}

	//If the closest point is inside the circle
	if (closestPoint.distanceTo(circle) < other->radius) {
		return true;
	}
	//If the shapes have not collided
	return false;
}