#include "Collider.h"
#include "CollisionListener.h"

Collider::Collider(fPoint position, const TypeCollider type) : position(position), type(type), listeners(), parentTransform(nullptr), parent(nullptr) {
	previousFrame = new std::list<Collider*>();
	actualFrame = new std::list<Collider*>();
}

Collider::~Collider() {
	cleanUp();
	delete previousFrame;
	delete actualFrame;
}

bool Collider::cleanUp() {
	previousFrame->clear();
	actualFrame->clear();
	return true;
}

update_status Collider::preUpdate() {
	
	return UPDATE_CONTINUE;
}

update_status Collider::update() {
	checkExitCollision();
	return UPDATE_CONTINUE;
}

update_status Collider::postUpdate() {
	clearCollisions();


	if (DEBUG_COLLISIONS) {
		paintCollider();
	}
	return UPDATE_CONTINUE;
}

void Collider::notify(Collider * other) {
	//type of collision
	bool newCollision = false;
	if (find(previousFrame->begin(), previousFrame->end(), other) == previousFrame->end()) {
		newCollision = true;
		actualFrame->push_back(other);
	}

	//notify all listeners
	for (auto it = listeners.begin(); it != listeners.end(); ++it) {
		if (newCollision) {
			(*it)->onCollisionEnter(this, other);
		} else {
			(*it)->onCollisionStay(this, other);
		}
	}

}

void Collider::addListener(CollisionListener * newListener) {
	auto it = find(listeners.begin(), listeners.end(), newListener);
	if (it == listeners.end()) {
		listeners.push_back(newListener);
	}
}

void Collider::removeListener(CollisionListener * listener) {
	auto it = find(listeners.begin(), listeners.end(), listener);
	if (it != listeners.end()) {
		listeners.erase(it);
	}
}


//notify onExit
void Collider::checkExitCollision() {
	for (auto it = previousFrame->begin(); it != previousFrame->end(); ++it) {
		if (find(actualFrame->begin(), actualFrame->end(), *it) == actualFrame->end()) {
			//notify
			for (auto it2 = listeners.begin(); it2 != listeners.end(); ++it2) {
				(*it2)->onCollisionExit(this, *it);
			}
		}
	}
}

void Collider::clearCollisions() {
	// Intercambia las listas de colisiones
	std::list<Collider*>* temp = previousFrame;
	previousFrame = actualFrame;
	actualFrame = temp;

	// Vacía la lista de colisiones actuales
	actualFrame->clear();
}
