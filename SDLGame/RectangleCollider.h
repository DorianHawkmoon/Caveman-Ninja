#pragma once
#ifndef RECTANGLE_COLLIDER_H
#define RECTANGLE_COLLIDER_H

#include "Collider.h"
#include "Globals.h"
#include "SDL\SDL_rect.h"
class CircleCollider;

class RectangleCollider : public Collider {
public:
	RectangleCollider(fPoint& position, SDL_Rect& rectangle, TypeCollider type);
	~RectangleCollider(){}

	virtual void paintCollider() const;

	update_status update();

	bool checkCollision(const Collider* r) const {
		return r->checkSpecificCollision(this);
	}
	bool checkSpecificCollision(const Collider* self) const;
	bool checkCollision(const RectangleCollider* other) const;
	bool checkCollision(const CircleCollider* other) const;

	Collider* clone();

private:
	SDL_Rect rect;
};


#endif // !RECTANGLE_COLLIDER_H
