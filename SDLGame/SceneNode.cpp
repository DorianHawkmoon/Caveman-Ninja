#include "SceneNode.h"

#include <algorithm> //for lambda find_if
#include <functional> // for lambda std::mem_fn(&SceneNode::isMarkedForRemoval)
#include "Entity.h"
#include "Transform.h"

SceneNode::SceneNode(Entity* entity) : children(), parent(nullptr), entity(entity) {
	if (this->entity != nullptr) {
		transform = this->entity->transform;
		transform->node = this;
	}
}


SceneNode::~SceneNode() {
	//delete the entity
	if (entity != nullptr) {
		entity->cleanUp();
		delete entity;
	}
	//delete the childs
	for (auto it = children.begin(); it != children.end(); it++) {
		(*it)->cleanUp();
		delete *it;
	}
	children.clear();
}

void SceneNode::addChild(SceneNode* child) {
	//set the parent of the child
	child->parent = this;
	children.push_back(std::move(child));
}

SceneNode * SceneNode::addChild(Entity * child) {
	SceneNode* result = new SceneNode(child);
	result->parent = this;
	children.push_back(std::move(result));
	return result;
}

SceneNode* SceneNode::removeChild(const SceneNode& node) {
	SceneNode* result = nullptr;

	//using lambdas to find the child. Comparing with the pointers
	auto found = std::find_if(children.begin(), children.end(),
		[&](SceneNode* & p) -> bool {
		return p == &node;
	}
	);

	//if found...
	if (found != children.end()) {
		//move the result instead of copy
		result = std::move(*found);
		result->parent = nullptr;
		children.erase(found);
	}

	return result;
}

void SceneNode::remove() {
	// Call function recursively for all remaining children
	std::for_each(children.begin(), children.end(), std::mem_fn(&SceneNode::remove));

	//delete himself
	if (entity != nullptr) {
		delete entity;
		entity = nullptr;
	}

	for (auto it = children.begin(); it != children.end(); ++it) {
		delete (*it);
	}
	// Remove all children which request so
	children.clear();
}

Transform SceneNode::getWorldTransform() const {
	//initialize a transform
	Transform transform = getLocalTransform();

	if (parent != nullptr) {
		transform += parent->getWorldTransform();
	}
	return transform;
}

Transform SceneNode::getLocalTransform() const {
	return transform->getLocalTransform();
}

void SceneNode::removeDead() {
	//delete the dead children
	for (auto it = children.begin(); it != children.end(); ) {
		if ((*it)->isDestroyed()) {
			(*it)->cleanUp();
			delete *it;
			it = children.erase(it);
		} else {
			++it;
		}
	}
}

bool SceneNode::isDestroyed() const {
	return entity->isDestroyed();
}

bool SceneNode::start() {
	if (entity != nullptr) {
		entity->start();
	}
	// Call function recursively for all remaining children
	std::for_each(children.begin(), children.end(), std::mem_fn(&SceneNode::start));
	return true;
}

update_status SceneNode::preUpdate() {
	removeDead();

	if (entity != nullptr) {
		entity->preUpdate();
	}
	// Call function recursively for all remaining children
	std::for_each(children.begin(), children.end(), std::mem_fn(&SceneNode::preUpdate));
	return UPDATE_CONTINUE;
}

update_status SceneNode::update() {
	if (entity != nullptr) {
		entity->update();
	}
	// Call function recursively for all remaining children
	std::for_each(children.begin(), children.end(), std::mem_fn(&SceneNode::update));
	return UPDATE_CONTINUE;
}

update_status SceneNode::postUpdate() {
	if (entity != nullptr) {
		entity->postUpdate();
	}
	// Call function recursively for all remaining children
	std::for_each(children.begin(), children.end(), std::mem_fn(&SceneNode::postUpdate));
	return UPDATE_CONTINUE;
}

bool SceneNode::cleanUp() {
	if (entity != nullptr) {
		entity->cleanUp();
	}
	// Call function recursively for all remaining children
	std::for_each(children.begin(), children.end(), std::mem_fn(&SceneNode::cleanUp));
	return true;
}
