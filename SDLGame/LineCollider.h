#pragma once
#ifndef LINE_COLLIDER_H
#define LINE_COLLIDER_H

#include "Collider.h"
#include <vector>
#include "Point.h"
#include "RectangleCollider.h"
#include "Transform.h"


class LineCollider : public Collider {

public:
	LineCollider(fPoint& position, std::vector<fPoint>& points, TypeCollider type);
	virtual ~LineCollider();

	virtual void paintCollider() const;

	bool checkCollision(const Collider* r) const;
	bool checkSpecificCollision(const Collider* self) const;
	bool checkCollision(const RectangleCollider* other) const;
	bool checkCollision(const CircleCollider* other) const;
	bool checkCollision(const LineCollider* other) const;

	Collider* clone() {
		return new LineCollider(position, points, type);
	}

	inline iPoint getSize() const {
		return iPoint( 1, thickness);
	}

public:
	int thickness;
	std::vector<fPoint> points;

private:
	fPoint getPointGlobalCoordinates(unsigned int index) const;
	RectangleCollider createSegmentCollider(int leftBoundIndex) const;

	bool checkCollisionRectangle(const RectangleCollider& segment, const RectangleCollider* other, const Transform & otherTrans, const std::vector<fPoint> otherPoints) const;
	std::vector<fPoint> getPoints(const RectangleCollider& segment) const;
};

#endif // !LINE_COLLIDER_H

