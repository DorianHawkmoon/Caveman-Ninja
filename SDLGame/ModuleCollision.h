#pragma once
#ifndef MODULE_COLLISION_H
#define MODULE_COLLISION_H

#include "Module.h"
#include <list>
#include "ColliderInteraction.h"
class Collider;


class ModuleCollision :	public Module {
public:
	ModuleCollision(bool started=true) :Module(started), colliders() {}
	virtual ~ModuleCollision(){}

	virtual bool start();
	virtual update_status update();
	virtual update_status postUpdate();
	virtual bool cleanUp();

	void addCollider(Collider* collider);
	void removeCollider(Collider* collider);

	void checkCollisions();
	const std::list<Collider*> checkCollisions(Collider* check);

private:
	std::list<Collider*> colliders;
	ColliderInteraction interaction;
};

#endif // !MODULE_COLLISION_H

