#pragma once
#ifndef COLLISION_LISTENER_H
#define COLLISION_LISTENER_H

class Collider;

class CollisionListener {
public:
	virtual void onCollisionEnter(Collider* self, Collider* another) {};
	virtual void onCollisionExit(Collider* self, Collider* another) {};
	virtual void onCollisionStay(Collider* self, Collider* another) {};
};
#endif // !COLLISION_LISTENER_H


