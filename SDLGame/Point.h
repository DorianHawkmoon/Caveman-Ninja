#pragma once
#ifndef POINT_H
#define POINT_H

#include <cmath>
#define PI 3.141592653589793238463

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

	Point operator * (TYPE f) const {
		Point r;
		r.x = f * x;
		r.y = f * y;
		return(r);
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

	Point& rotate(float degrees) {
		double rads = degrees * 2 * PI / 360.0;

		double cosine = cos(rads);
		double sine = sin(rads);
		float tx = (float) (x * cosine - y * sine);
		float ty = (float) (x * sine + y * cosine);

		x = tx;
		y = ty;

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
