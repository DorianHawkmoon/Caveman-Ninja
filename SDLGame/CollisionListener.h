#pragma once
#ifndef COLLISION_LISTENER_H
#define COLLISION_LISTENER_H

class Collider;

class CollisionListener {
public:
	virtual void onCollisionEnter(Collider* one, Collider* another) {};
	virtual void onCollisionExit(Collider* one, Collider* another) {};
	virtual void onCollisionStay(Collider* one, Collider* another) {};
};
#endif // !COLLISION_LISTENER_H


