#pragma once
#ifndef CIRCLE_COLLIDER_H
#define CIRCLE_COLLIDER_H

#include "ICollider.h"

class CircleCollider : public ICollider {
public:
	CircleCollider(fPoint position, float radius, TypeCollider type):ICollider(position,nullptr,type), radius(radius) {}
	~CircleCollider() {}

	virtual void paintCollider() const {
		
	}

private:
	float radius;

	bool checkSpecificCollision(const ICollider* self) const;

	bool checkCollision(const RectangleCollider* other) const;

	bool checkCollision(const CircleCollider* other) const;
};

#endif // !CIRCLE_COLLIDER_H

