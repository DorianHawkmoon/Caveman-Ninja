#pragma once
#ifndef RECTANGLE_COLLIDER_H
#define RECTANGLE_COLLIDER_H

#include "Collider.h"
#include "Globals.h"
#include "SDL/SDL_rect.h"
class LineCollider;
class CircleCollider;

class RectangleCollider : public Collider {
	friend class LineCollider;

public:
	RectangleCollider(const fPoint& position, const iPoint& rectangle, float rotation, TypeCollider type);
	virtual ~RectangleCollider();

	virtual void paintCollider() const;
	//paint with a rotation/pivot
	void paintCollider(const iPoint& pivot) const;

	bool checkCollision(const Collider* r) const;
	bool checkSpecificCollision(const Collider* self) const;
	bool checkCollision(const RectangleCollider* other) const;
	bool checkCollision(const CircleCollider* other) const;
	bool checkCollision(const LineCollider* other) const;;

	Collider* clone() const;

	float rotation;
	iPoint getSize() const;

private:
	iPoint rect;
	//concrete checks when the rectangle is rotated (not working)
	bool checkCollisionRotated(const RectangleCollider* other, const Transform & otherTrans, const Transform & self) const;
	//get the vertex of the rectangle with the rotation
	std::vector<fPoint> getPoints(float totalRotation) const;
	fPoint getCenter() const;
};


#endif // !RECTANGLE_COLLIDER_H
