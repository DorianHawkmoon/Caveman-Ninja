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
	TOP_LEFT = TOP | LEFT,
	TOP_RIGHT = TOP | RIGHT,
	TOP_CENTER = TOP | CENTER,
	BOTTOM_CENTER = BOTTOM | CENTER,
	BOTTOM_LEFT = BOTTOM | LEFT,
	BOTTOM_RIGHT = BOTTOM | RIGHT,
};

struct GUITransform {
	GUITransform();
	virtual ~GUITransform();

	fPoint position;
	float rotation;
	GUILocation location;
	GUILocation pivot;


	GUITransform operator =(const GUITransform &v) const {
		GUITransform r;
		r.position = v.position;
		r.rotation = v.rotation;
		r.location = v.location;
		r.pivot = v.pivot;

		return(r);
	}

	// Math ------------------------------------------------
	GUITransform operator -(const GUITransform &v) const {
		GUITransform r;
		r.position = position - v.position;
		r.rotation = rotation - v.rotation;
		r.location = static_cast<GUILocation>(location & v.location);
		r.pivot = static_cast<GUILocation>(pivot & v.pivot);
		return(r);
	}

	GUITransform operator + (const GUITransform &v) const {
		GUITransform r;
		r.position = position + v.position;
		r.rotation = rotation + v.rotation;
		r.location = static_cast<GUILocation>(location | v.location);
		r.pivot= static_cast<GUILocation>(pivot | v.pivot);
		return(r);
	}

	const GUITransform& operator -=(const GUITransform &v) {
		position -= v.position;
		rotation -= v.rotation;
		location = static_cast<GUILocation>(location & v.location);
		pivot= static_cast<GUILocation>(pivot & v.pivot);
		return(*this);
	}

	const GUITransform& operator +=(const GUITransform &v) {
		position += v.position;
		rotation += v.rotation;
		location = static_cast<GUILocation>(location | v.location);
		pivot= static_cast<GUILocation>(pivot | v.pivot);
		return(*this);
	}

	// Utils ------------------------------------------------
	bool IsZero() const;

	GUITransform& setToZero();
};

#endif // !GUI_TRANSFORM_H
