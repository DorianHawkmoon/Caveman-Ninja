#pragma once
#ifndef GUI_TRANSFORM_H
#define GUI_TRANSFORM_H

#include "Point.h"

enum GUILocation {
	ABSOLUTE = 0,
	CENTER = 1 << 0,
	TOP = 1 << 1,
	BOTTOM = 1 << 2,
	RIGHT =  1 << 3,
	LEFT = 1 << 4,
};

struct GUITransform {
	GUITransform();
	virtual ~GUITransform();

	fPoint position;
	float rotation;
	GUILocation location;


	GUITransform operator =(const GUITransform &v) const {
		GUITransform r;
		r.position = v.position;
		r.rotation = v.rotation;
		r.location = v.location;

		return(r);
	}

	// Math ------------------------------------------------
	GUITransform operator -(const GUITransform &v) const {
		GUITransform r;
		r.position = position - v.position;
		r.rotation = rotation - v.rotation;
		r.location = static_cast<GUILocation>(location & v.location);
		return(r);
	}

	GUITransform operator + (const GUITransform &v) const {
		GUITransform r;
		r.position = position + v.position;
		r.rotation = rotation + v.rotation;
		r.location = static_cast<GUILocation>(location | v.location);
		return(r);
	}

	const GUITransform& operator -=(const GUITransform &v) {
		position -= v.position;
		rotation -= v.rotation;
		location = static_cast<GUILocation>(location & v.location);
		return(*this);
	}

	const GUITransform& operator +=(const GUITransform &v) {
		position += v.position;
		rotation += v.rotation;
		location = static_cast<GUILocation>(location | v.location);

		return(*this);
	}

	// Utils ------------------------------------------------
	bool IsZero() const;

	GUITransform& setToZero();
};

#endif // !GUI_TRANSFORM_H
