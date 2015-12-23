#include "SceneNode.h"

#include <algorithm> //for lambda find_if
#include <functional> // for lambda std::mem_fn(&SceneNode::isMarkedForRemoval)
#include "Entity.h"

SceneNode::SceneNode(Entity* entity) : children(), parent(nullptr), entity(entity) {}


SceneNode::~SceneNode() {}

void SceneNode::addChild(SceneNode* child) {
	//set the parent of the child
	child->parent = this;
	children.push_back(std::move(child));
}

SceneNode * SceneNode::addChild(Entity * child) {
	SceneNode* result = new SceneNode(child);
	addChild(result);
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

	// Remove all children which request so
	children.clear();
}

Transform SceneNode::getWorldTransform() const {
	//initialize a transform
	Transform transform;//TODO hacer transform= sf::Transform::Identity;

						//iterate the scene graph to the root to get the absolute transforms
	for (const SceneNode* node = this; node != nullptr; node = node->parent) {
		//transform = node->getTransform() * transform;
	}

	return transform;
}

void SceneNode::removeWrecks() {
	// Remove all children which request so
	auto wreckfieldBegin = std::remove_if(children.begin(), children.end(), std::mem_fn(&SceneNode::isMarkedForRemoval));
	children.erase(wreckfieldBegin, children.end()); //erase the vector from the new end to the last end (remove the positions removed)

													 // Call function recursively for all remaining children
	std::for_each(children.begin(), children.end(), std::mem_fn(&SceneNode::removeWrecks));
}

bool SceneNode::isMarkedForRemoval() const {
	// By default, remove node if entity is destroyed
	return isDestroyed();
}

//TODO cambiar nombre remove wrecks
bool SceneNode::isDestroyed() const {
	return entity->isDestroyed();
}

bool SceneNode::start() {
	entity->start();
	// Call function recursively for all remaining children
	std::for_each(children.begin(), children.end(), std::mem_fn(&SceneNode::start));
	return true;
}

update_status SceneNode::preUpdate() {
	entity->preUpdate();
	// Call function recursively for all remaining children
	std::for_each(children.begin(), children.end(), std::mem_fn(&SceneNode::preUpdate));
	return UPDATE_CONTINUE;
}

update_status SceneNode::update() {
	entity->update();
	// Call function recursively for all remaining children
	std::for_each(children.begin(), children.end(), std::mem_fn(&SceneNode::update));
	return UPDATE_CONTINUE;
}

update_status SceneNode::postUpdate() {
	entity->postUpdate();
	// Call function recursively for all remaining children
	std::for_each(children.begin(), children.end(), std::mem_fn(&SceneNode::postUpdate));
	return UPDATE_CONTINUE;
}

bool SceneNode::cleanUp() {
	entity->cleanUp();
	// Call function recursively for all remaining children
	std::for_each(children.begin(), children.end(), std::mem_fn(&SceneNode::cleanUp));
	return true;
}
