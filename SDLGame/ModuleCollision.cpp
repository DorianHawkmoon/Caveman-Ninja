#include "ModuleCollision.h"
#include "Collider.h"

bool ModuleCollision::start() {
	return true;
}

update_status ModuleCollision::update() {
	//check the collisions and notify them
	checkCollisions();
	//update all of them (checking exit collision, notify it)
	for (std::list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it) {
		(*it)->update();
	}

	return UPDATE_CONTINUE;
}

update_status ModuleCollision::postUpdate() {
	update_status result = UPDATE_CONTINUE;
	//clear frame collision
	for (std::list<Collider*>::iterator it = colliders.begin(); it != colliders.end() && result == UPDATE_CONTINUE; ++it) {
		result = (*it)->postUpdate();
	}
	return UPDATE_CONTINUE;
}

bool ModuleCollision::cleanUp() {
	for (auto it = colliders.begin(); it != colliders.end(); ++it) {
		(*it)->cleanUp();
	}
	colliders.clear(); //don't delete objects, I don't own them
	return true;
}

void ModuleCollision::addCollider(Collider * collider) {
	if (std::find(colliders.begin(), colliders.end(), collider) == colliders.end()) {
		collider->start();
		colliders.push_back(collider);
	}
	collider->start();
}

void ModuleCollision::removeCollider(Collider * collider) {
	auto result = std::find(colliders.begin(), colliders.end(), collider);
	if (result != colliders.end()) {
		(*result)->cleanUp();
		colliders.erase(result);
	}
}

void ModuleCollision::checkCollisions() {
	std::list<Collider*>::iterator one = colliders.begin();
	std::list<Collider*>::iterator two = std::next(one);
	while (one != colliders.end()) {
		#ifdef DEBUG
		(*one)->paintCollider();
		#endif

		while (two != colliders.end()) {
			if ((*one)->checkCollision(*two)) {

				if ((*one)->type != TypeCollider::GROUND && (*two)->type != TypeCollider::GROUND
					&& (*one)->type != TypeCollider::WALL && (*two)->type != TypeCollider::WALL) {
					LOG("collision");
				}

				(*one)->notify(*two);
				(*two)->notify(*one);
			}
			two= std::next(two);
		}
		one = std::next(one);
		two = std::next(one);

	}
}

const std::list<Collider*> ModuleCollision::checkCollisions(Collider * check) {
	std::list<Collider*> result;

#ifdef DEBUG
	check->paintCollider();
#endif

	for (auto it = colliders.begin(); it != colliders.end(); ++it) {
		if (check != *it) {
			if (check->checkCollision(*it)) {
				result.push_back(*it);
			}
		}
	}

	return result;
}
