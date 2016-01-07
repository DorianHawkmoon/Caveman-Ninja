#include "Scene.h"

#include "Entity.h"
#include "SceneNode.h"
#include <algorithm>
#include <functional>

Scene::Scene(): root(nullptr), entities() {
	rootE = new Entity();
	root = new SceneNode(rootE);
}

Scene::~Scene() {
	if (root != nullptr) {
		delete root;
	}
}

update_status Scene::preUpdate() {
	if (root != nullptr) {
		root->preUpdate();
	}
	return UPDATE_CONTINUE;
}

update_status Scene::update() {
	if (root != nullptr) {
		root->update();
	}
	return UPDATE_CONTINUE;
}

update_status Scene::postUpdate() {
	if (root != nullptr) {
		root->postUpdate();
	}
	//elimino entidades destruidas
	auto wreckfieldBegin = std::remove_if(entities.begin(), entities.end(), std::mem_fn(&Entity::isDestroyed));
	entities.erase(wreckfieldBegin, entities.end());

	//TODO limpiar de la lista entidades muertas? dejar que lo haga el scene?
	return UPDATE_CONTINUE;
}

bool Scene::cleanUp() {
	root->remove();
	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
		delete *it;
	}
	entities.clear();
	return true;
}