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

	virtual void paintCollider() const {
		// Determina el color y opacidad del dibujo
		SDL_Color renderColor;
		renderColor.r = 0;
		renderColor.g = 179;
		renderColor.b = 0;
		renderColor.a = 128;

		//const SDL_Texture* texture, const SDL_Color& color, const iPoint& position, float radius, float speed = 1.0f
		App->renderer->paintCircle(renderColor, position, radius);
	}

	bool checkCollision(const Collider* r) const {
		return r->checkSpecificCollision(this);
	}

	Collider* clone();
	float radius;
private:

	bool checkSpecificCollision(const Collider* self) const;

	bool checkCollision(const RectangleCollider* other) const;

	bool checkCollision(const CircleCollider* other) const;
};


#endif // !CIRCLE_COLLIDER_H
