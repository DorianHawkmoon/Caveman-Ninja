#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Point.h"
#include "SDL/SDL.h"
#include "SceneNode.h"

struct Transform {
	Transform();
	virtual ~Transform();

	SDL_RendererFlip flip;
	float speed;
	fPoint position;
	//float rotation; // not used
	//fPoint scale; //not used
	SceneNode* node;

	//copy
	Transform getLocalTransform() const {
		Transform result;
		result.position = position;
		result.speed = speed;
		//result.rotation = rotation;
		//result.scale = scale;
		result.node = nullptr;
		return result;
	}

	//copy
	Transform getGlobalTransform() const {
		if (node != nullptr) {
			return node->getWorldTransform();
		} else {
			return getLocalTransform();
		}
	}

	Transform operator =(const Transform &v) const {
		Transform r;
		r.setToZero();
		r.flip =  v.flip;
		r.speed = v.speed;
		r.position = v.position;
		//r.scale = v.scale;
		//r.rotation = v.rotation;

		return(r);
	}

	// Math ------------------------------------------------
	Transform operator -(const Transform &v) const {
		Transform r;
		r.flip = static_cast<SDL_RendererFlip>(flip & (!v.flip));
		r.speed = speed - v.speed;
		r.position = position - v.position;
		//r.scale = scale - v.scale;
		//r.rotation = rotation - v.rotation;

		return(r);
	}

	Transform operator + (const Transform &v) const {
		Transform r;
		r.flip = static_cast<SDL_RendererFlip>(flip | v.flip);
		r.speed = speed + v.speed;
		r.position = position + v.position;
		//r.scale = scale + v.scale;
		//r.rotation = rotation + v.rotation;

		return(r);
	}

	const Transform& operator -=(const Transform &v) {
		flip = static_cast<SDL_RendererFlip>(flip & (!v.flip));
		speed -= v.speed;
		position -= v.position;
		//scale -= v.scale;
		//rotation -= v.rotation;

		return(*this);
	}

	const Transform& operator +=(const Transform &v) {
		flip = static_cast<SDL_RendererFlip>(flip | v.flip);
		speed += v.speed;
		position += v.position;
		//scale += v.scale;
		//rotation += v.rotation;

		return(*this);
	}

	// Utils ------------------------------------------------
	bool IsZero() const;

	Transform& setToZero();
};

#endif // !TRANSFORM_H

