#pragma once
#ifndef RECTANGLE_COLLIDER_H
#define RECTANGLE_COLLIDER_H

#include "Collider.h"
#include "Globals.h"
#include "SDL\SDL_rect.h"
class LineCollider;
class CircleCollider;

class RectangleCollider : public Collider {
public:
	RectangleCollider(fPoint& position, iPoint& rectangle, float rotation, TypeCollider type);
	virtual ~RectangleCollider(){}

	virtual void paintCollider() const;
	void paintCollider(const iPoint& pivot) const;

	bool checkCollision(const Collider* r) const {
		return r->checkSpecificCollision(this);
	}
	bool checkSpecificCollision(const Collider* self) const;
	bool checkCollision(const RectangleCollider* other) const;
	bool checkCollision(const CircleCollider* other) const;
	bool checkCollision(const LineCollider* other) const;;
	Collider* clone();

	float rotation;
	inline iPoint getSize()const {
		return rect;
	}

private:
	iPoint rect;
	bool checkCollisionRotated(const RectangleCollider* other, const Transform & otherTrans, const Transform & self) const;
	std::vector<fPoint> getPoints(float totalRotation) const;
	fPoint getCenter() const;
};


#endif // !RECTANGLE_COLLIDER_H
