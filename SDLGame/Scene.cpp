#include "Scene.h"

#include "Entity.h"
#include "SceneNode.h"
#include <algorithm>
#include <functional>

Scene::Scene(): root(nullptr), entities() {}

Scene::~Scene() {
	if (root != nullptr) {
		delete root;
	}
}

bool Scene::preUpdate() {
	if (root != nullptr) {
		root->preUpdate();
	}
	return true;
}

bool Scene::update() {
	if (root != nullptr) {
		root->update();
	}
	return true;
}

bool Scene::postUpdate() {
	if (root != nullptr) {
		root->postUpdate();
	}
	//elimino entidades destruidas
	auto wreckfieldBegin = std::remove_if(entities.begin(), entities.end(), std::mem_fn(&Entity::isDestroyed));
	entities.erase(wreckfieldBegin, entities.end());
	return true;
}

bool Scene::cleanUp() {
	root->remove();
	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
		delete *it;
	}
	entities.clear();
	return true;
}