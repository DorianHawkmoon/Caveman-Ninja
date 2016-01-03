#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Point.h"
#include "SDL/SDL.h"

struct Transform {
	Transform();
	~Transform();

	SDL_RendererFlip flip;
	float speed;
	fPoint position;
	float rotation; // not used
	fPoint scale; //not used

	// Math ------------------------------------------------
	Transform operator -(const Transform &v) const {
		Transform r;
		r.flip = static_cast<SDL_RendererFlip>(flip & (!v.flip));
		r.speed = speed - v.speed;
		r.position = position - v.position;
		r.scale = scale - v.scale;
		r.rotation = rotation - v.rotation;

		return(r);
	}

	Transform operator + (const Transform &v) const {
		Transform r;
		r.flip = static_cast<SDL_RendererFlip>(flip | v.flip);
		r.speed = speed + v.speed;
		r.position = position + v.position;
		r.scale = scale + v.scale;
		r.rotation = rotation + v.rotation;

		return(r);
	}

	const Transform& operator -=(const Transform &v) {
		flip = static_cast<SDL_RendererFlip>(flip & (!v.flip));
		speed -= v.speed;
		position -= v.position;
		scale -= v.scale;
		rotation -= v.rotation;

		return(*this);
	}

	const Transform& operator +=(const Transform &v) {
		flip = static_cast<SDL_RendererFlip>(flip | v.flip);
		speed += v.speed;
		position += v.position;
		scale += v.scale;
		rotation += v.rotation;

		return(*this);
	}

	// Utils ------------------------------------------------
	bool IsZero() const {
		return rotation == 0 && position.isZero() && scale.isZero() && speed == 0 && flip==SDL_FLIP_NONE;
	}

	Transform& setToZero() {
		speed = 0;
		flip = SDL_FLIP_NONE;
		position.setToZero();
		scale.setToZero();
		rotation = 0;
		return(*this);
	}
};

#endif // !TRANSFORM_H

