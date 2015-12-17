#pragma once

#ifndef MODULE_COLLISION_H
#define MODULE_COLLISION_H

#include "Module.h"
#include "ICollider.h"
#include <list>

//TODO hacer on collision
//TODO hacer on finish collision
//TODO no comprobar los que están fuera de pantalla
//TODO si los dos estáticos, no comprobar
//TODO quadtree

class ModuleCollision :
	public Module {
public:
	ModuleCollision(bool started=true);
	~ModuleCollision();

	update_status update();

	void addCollider(const ICollider* collider);
	void removeCollider(const ICollider* collider);

private:
	std::list<ICollider*> colliders;
};


#endif // !MODULE_COLLISION_H
