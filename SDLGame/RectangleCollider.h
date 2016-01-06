#pragma once
#ifndef RECTANGLE_COLLIDER_H
#define RECTANGLE_COLLIDER_H

#include "Collider.h"
#include "Globals.h"
#include "SDL\SDL_rect.h"
class FloorCollider;
class CircleCollider;

class RectangleCollider : public Collider {
public:
	RectangleCollider(fPoint& position, iPoint& rectangle, float rotation, TypeCollider type);
	virtual ~RectangleCollider(){}

	virtual void paintCollider() const;

	bool checkCollision(const Collider* r) const {
		return r->checkSpecificCollision(this);
	}
	bool checkSpecificCollision(const Collider* self) const;
	bool checkCollision(const RectangleCollider* other) const;
	bool checkCollision(const CircleCollider* other) const;
	bool checkCollision(const FloorCollider* other) const {return false;};
	Collider* clone();

	SDL_Rect getGlobalRectangle() const;

	float rotation;
private:
	iPoint rect;
	
};


#endif // !RECTANGLE_COLLIDER_H
