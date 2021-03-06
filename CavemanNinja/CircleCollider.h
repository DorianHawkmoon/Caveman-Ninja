#pragma once
#ifndef CIRCLE_COLLIDER_H
#define CIRCLE_COLLIDER_H

#include "Collider.h"
#include "Point.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
class RectangleCollider;


class CircleCollider : public Collider {
public:
	CircleCollider(fPoint position, float radius, TypeCollider type);
	virtual ~CircleCollider();

	virtual void paintCollider() const;

	bool checkCollision(const Collider* r) const;

	Collider* clone()const;
	float radius;
	iPoint getSize() const;
	fPoint getGlobalPoint() const;


	bool checkSpecificCollision(const Collider* self) const;
	bool checkCollision(const RectangleCollider* other) const;
	bool checkCollision(const CircleCollider* other) const;
	bool checkCollision(const LineCollider* other) const;;
};


#endif // !CIRCLE_COLLIDER_H
