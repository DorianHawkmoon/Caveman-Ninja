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
	CircleCollider(fPoint position, float radius, TypeCollider type) : Collider(position, type), radius(radius) {
		
	}
	virtual ~CircleCollider();

	virtual void paintCollider() const;

	inline bool checkCollision(const Collider* r) const {
		return r->checkSpecificCollision(this);
	}

	Collider* clone();
	float radius;

	fPoint getGlobalPoint() const;
private:

	bool checkSpecificCollision(const Collider* self) const;

	bool checkCollision(const RectangleCollider* other) const;

	bool checkCollision(const CircleCollider* other) const;
};


#endif // !CIRCLE_COLLIDER_H
