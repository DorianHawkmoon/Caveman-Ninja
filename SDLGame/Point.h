#pragma once
#ifndef POINT_H
#define POINT_H

#include <math.h>

template<class TYPE>
class Point {
public:

	TYPE x, y;

	Point() {}
	Point(TYPE x, TYPE y) : x(x), y(y) {}

	// Operators ------------------------------------------------
	Point operator -(const Point &v) const {
		Point r;

		r.x = x - v.x;
		r.y = y - v.y;

		return(r);
	}

	Point operator + (const Point &v) const {
		Point r;

		r.x = x + v.x;
		r.y = y + v.y;

		return(r);
	}

	const Point& operator -=(const Point &v) {
		x -= v.x;
		y -= v.y;

		return(*this);
	}

	const Point& operator +=(const Point &v) {
		x += v.x;
		y += v.y;

		return(*this);
	}

	bool operator ==(const Point& v) const {
		return (x == v.x && y == v.y);
	}

	bool operator !=(const Point& v) const {
		return (x != v.x || y != v.y);
	}

	// Utils ------------------------------------------------
	bool isZero() const {
		return (x == 0 && y == 0);
	}

	Point& setToZero() {
		x = y = 0;
		return(*this);
	}

	Point& negate() {
		x = -x;
		y = -y;

		return(*this);
	}

	// Distances ---------------------------------------------
	TYPE distanceTo(const Point& v) const {
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return static_cast<TYPE>(sqrt((fx*fx) + (fy*fy)));
	}
};

typedef Point<int> iPoint;
typedef Point<float> fPoint;
typedef Point<int> Vector2i;
typedef Point<float> Vector2f;

#endif // !POINT_H
