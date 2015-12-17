#pragma once
#ifndef RECTANGLE_COLLIDER_H
#define RECTANGLE_COLLIDER_H

#include "ICollider.h"
#include "Application.h"
#include "ModuleRender.h"
#include "SDL\SDL.h"

class RectangleCollider : public ICollider {
public:
	RectangleCollider(fPoint position, SDL_Rect rectangle, TypeCollider type);
	~RectangleCollider(){}

	virtual void paintCollider() const;

private:
	SDL_Rect rect;

	bool checkSpecificCollision(const ICollider* self) const;
	bool checkCollision(const RectangleCollider* other) const;
	bool checkCollision(const CircleCollider* other) const;
};

#endif // !RECTANGLE_COLLIDER_H

