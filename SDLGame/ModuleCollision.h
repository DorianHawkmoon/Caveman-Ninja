#pragma once
#ifndef MODULE_COLLISION_H
#define MODULE_COLLISION_H

#include "Module.h"
#include <list>
class Collider;


class ModuleCollision :	public Module {
public:
	ModuleCollision(bool started=true) :Module(started), colliders() {}
	~ModuleCollision(){}

	virtual bool start();
	virtual update_status update();
	virtual update_status postUpdate();
	virtual bool cleanUp();

	void addCollider(Collider* collider);
	void removeCollider(Collider* collider);

	void checkCollisions();

private:
	std::list<Collider*> colliders;
};

#endif // !MODULE_COLLISION_H

