#pragma once
#ifndef COLLISION_LISTENER_H
#define COLLISION_LISTENER_H

class Collider;

class CollisionListener {
public:
	virtual void onCollisionEnter(const Collider* self, const Collider* another) {};
	virtual void onCollisionExit(const Collider* self, const Collider* another) {};
	virtual void onCollisionStay(const Collider* self, const Collider* another) {};
};
#endif // !COLLISION_LISTENER_H


