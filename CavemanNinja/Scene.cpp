#include "Scene.h"

#include "Entity.h"
#include "SceneNode.h"
#include <algorithm>
#include "GUIContainer.h"
#include "Application.h"
#include "ModuleGUI.h"
#include "GUIComponent.h"
#include <functional>

Scene::Scene(): root(nullptr), entities(), gui() {
	rootE = new Entity();
	root = new SceneNode(rootE);
}

Scene::~Scene() {
	if (root != nullptr) {
		delete root;
	}
	//dont delete gui, not owned
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

	return UPDATE_CONTINUE;
}

bool Scene::cleanUp() {
	LOG("unloading scene");
	root->cleanUp();
	root->remove();
	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
		delete *it;
	}
	entities.clear();

	for (auto it = gui.begin(); it != gui.end(); ++it) {
		//remove from gui module
		App->gui->removeGUI(*it);
		(*it)->cleanUp(); //don't delete, not owned
	}
	gui.clear();

	return true;
}

void Scene::addNode(Entity * entity) {
	if (root != nullptr) {
		root->addChild(entity);
		entity->start();
	}
}

void Scene::addGUI(GUI::GUIComponent * component) {
	gui.push_back(component);
	App->gui->registerGUI(component);
}