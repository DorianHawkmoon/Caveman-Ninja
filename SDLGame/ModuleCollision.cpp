#include "ModuleCollision.h"



ModuleCollision::ModuleCollision(bool started): Module(started) {}


ModuleCollision::~ModuleCollision() {}

update_status ModuleCollision::update() {
	std::list<ICollider*>::iterator one = colliders.begin();
	std::list<ICollider*>::iterator two = ++one;
	while (one != colliders.end()) {
		//TODO set debug
		//pintamos modo debug
		(*one)->paintCollider();
		while (two != colliders.end()) {
			if ((*one)->checkCollision(*two)) {
				//(*one)->module->onCollision()
				//(*two)->module->onCollision()
			}
			++two;
		}
		++one;
		two = ++one;

	}

	return update_status();
}
