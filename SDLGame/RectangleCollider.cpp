#include "RectangleCollider.h"

inline RectangleCollider::RectangleCollider(fPoint position, SDL_Rect rectangle, TypeCollider type) : ICollider(position, nullptr, type), rect(rectangle) {}

inline void RectangleCollider::paintCollider() const {
	iPoint pos(position.x, position.y);
	SDL_Color color;
	color.b = 255;
	color.r = 255;
	color.g = 255;
	color.a = 255;
	App->renderer->paintRectangle(color, pos, rect);
}

inline bool RectangleCollider::checkSpecificCollision(const ICollider * self) const {
	self->checkCollision(this);
}

inline bool RectangleCollider::checkCollision(const RectangleCollider * other) const {
	return SDL_HasIntersection(&rect, &(other->rect))==SDL_TRUE;
}

inline bool RectangleCollider::checkCollision(const CircleCollider * other) const {
	//TODO colision entre rect y circulo
}
