#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Point.h"

struct Transform {
	Transform();
	~Transform();

	fPoint position;
	float rotation;
	fPoint scale;

	// Math ------------------------------------------------
	Transform operator -(const Transform &v) const {
		Transform r;

		r.position = position - v.position;
		r.scale = scale - v.scale;
		r.rotation = rotation - v.rotation;

		return(r);
	}

	Transform operator + (const Transform &v) const {
		Transform r;

		r.position = position + v.position;
		r.scale = scale + v.scale;
		r.rotation = rotation + v.rotation;

		return(r);
	}

	const Transform& operator -=(const Transform &v) {
		position -= v.position;
		scale -= v.scale;
		rotation -= v.rotation;

		return(*this);
	}

	const Transform& operator +=(const Transform &v) {
		position += v.position;
		scale += v.scale;
		rotation += v.rotation;

		return(*this);
	}

	// Utils ------------------------------------------------
	bool IsZero() const {
		return rotation==0 && position.isZero() && scale.isZero();
	}

	Transform& setToZero() {
		position.setToZero();
		scale.setToZero();
		rotation = 0;
		return(*this);
	}
};

#endif // !TRANSFORM_H

