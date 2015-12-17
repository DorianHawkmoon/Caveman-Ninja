#pragma once
#ifndef ICOLLIDER_H
#define ICOLLIDER_H

#include "Point.h"
#include "Module.h"

class RectangleCollider;
class CircleCollider;

enum class TypeCollider {
	NONE_COLLIDER = 0,
	WALL = 1 << 0,
	PLAYER = 1 << 1,
	ENEMY = 1 << 2,
	PLAYER_SHOT = 1 << 3,
	ENEMY_SHOT = 1 << 4
};

class ICollider {
public:
	ICollider(fPoint position, Module* module, const TypeCollider type): position(position),module(nullptr),type(type){}
	~ICollider() {}

	virtual bool checkCollision(const ICollider* r) const {
		r->checkSpecificCollision(this);
	}

	virtual bool checkSpecificCollision(const ICollider* self) const=0;
	virtual bool checkCollision(const RectangleCollider* other) const = 0;
	virtual bool checkCollision(const CircleCollider* other) const = 0;

	virtual void paintCollider() const = 0;

public:
	fPoint position;
	Module* module;
	TypeCollider type;
};

#endif // !ICOLLIDER_H

