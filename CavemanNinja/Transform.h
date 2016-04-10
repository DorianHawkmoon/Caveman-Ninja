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
	fPoint position;
	float rotation;
	SceneNode* node;

	//copy
	Transform getLocalTransform() const {
		Transform result;
		result.position = position;
		result.rotation = rotation;
		result.flip = flip;
		result.node = node;
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
		r.flip =  v.flip;
		r.position = v.position;
		r.rotation = v.rotation;

		return(r);
	}

	// Math ------------------------------------------------
	Transform operator -(const Transform &v) const {
		Transform r;
		r.flip = static_cast<SDL_RendererFlip>(flip & (!v.flip));
		r.position = position - v.position;
		r.rotation = rotation - v.rotation;

		return(r);
	}

	Transform operator + (const Transform &v) const {
		Transform r;
		r.flip = static_cast<SDL_RendererFlip>(flip | v.flip);
		r.position = position + v.position;
		r.rotation = rotation + v.rotation;

		return(r);
	}

	const Transform& operator -=(const Transform &v) {
		flip = static_cast<SDL_RendererFlip>(flip & (!v.flip));
		position -= v.position;
		rotation -= v.rotation;

		return(*this);
	}

	const Transform& operator +=(const Transform &v) {
		flip = static_cast<SDL_RendererFlip>(flip | v.flip);
		position += v.position;
		rotation += v.rotation;

		return(*this);
	}

	// Utils ------------------------------------------------
	bool IsZero() const;

	Transform& setToZero();
};

#endif // !TRANSFORM_H

