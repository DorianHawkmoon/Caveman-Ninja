#include "Entity.h"
#include <algorithm>
#include <functional>
#include "IComponent.h"

Entity::Entity(Category category) : destroyed(false), category(category) {
	transform = new Transform();
	transform->setToZero();
}


Entity::~Entity() {
	//just in case, clean up
	cleanUp();
	//destroy entities
	for (auto it = properties.begin(); it != properties.end(); ++it) {
		delete (*it);
	}
	properties.clear();
	if (transform != nullptr) {
		delete transform;
	}
}

bool Entity::start() {
	bool result = true;
	for (auto it = properties.begin(); it != properties.end() && result; ++it) {
		if ((*it)->isEnable()) {
			result &= (*it)->start();
		}
	}
	return result;
}

update_status Entity::preUpdate() {
	update_status result = UPDATE_CONTINUE;
	for (auto it = properties.begin(); it != properties.end() && result==UPDATE_CONTINUE; ++it) {
		if ((*it)->isEnable()) {
			result = (*it)->preUpdate();
		}
	}
	return result;
}

update_status Entity::update() {
	update_status result = UPDATE_CONTINUE;
	for (auto it = properties.begin(); it != properties.end() && result == UPDATE_CONTINUE; ++it) {
		if ((*it)->isEnable()) {
			result = (*it)->update();
		}
	}
	return result;
}

update_status Entity::postUpdate() {
	update_status result=UPDATE_CONTINUE;
	for (auto it = properties.begin(); it != properties.end() && result == UPDATE_CONTINUE; ++it) {
		if ((*it)->isEnable()) {
			result = (*it)->postUpdate();
		}
	}
	return result;
}

bool Entity::cleanUp() {
	bool result = true;
	for (auto it = properties.begin(); it != properties.end() && result; ++it) {
		if ((*it)->isEnable()) {
			result &= (*it)->cleanUp();
		}
	}
	return result;
}

bool Entity::addComponent(IComponent * component) {
	bool result = false;
	if (component != nullptr) {
		auto it = std::find_if(properties.begin(), properties.end(),
			[&component](IComponent* comp) { return comp->getID().compare(component->getID()) == 0; });

		if (it == properties.end()) {
			component->setParent(this);
			properties.push_back(component);
			result = true;
		}
	}
	return result;
}

IComponent * Entity::getComponent(const std::string& id) {
	IComponent* result = nullptr;
	std::list<IComponent*>::iterator it = std::find_if(properties.begin(), properties.end(),
		[&id](IComponent* comp) { return comp->getID().compare(id) == 0; });

	if (it != properties.end() && (*it)->isEnable()) {
			result = *it;
	}
	return result;
}

bool Entity::removeComponent(IComponent * component) {
	properties.remove(component);
	if (component->isEnable()) {
		component->cleanUp();
	}
	delete component;
	return false;
}

bool Entity::removeComponent(const std::string & name) {
	bool result = true;
	IComponent* component = getComponent(name);
	if (component == nullptr) {
		result = false;
	} else {
		result=removeComponent(component);
	}
	return result;
}

void Entity::onCollisionEnter(const Collider * self, const Collider * another) {
	std::for_each(properties.begin(), properties.end(), [&](auto collider) {
		collider->onCollisionEnter(self, another); 
	});
}

void Entity::onCollisionExit(const Collider * self, const Collider * another) {
	std::for_each(properties.begin(), properties.end(), [&](auto collider) {
		collider->onCollisionExit(self, another);
	});
}

void Entity::onCollisionStay(const Collider * self, const Collider * another) {
	std::for_each(properties.begin(), properties.end(), [&](auto collider) {
		collider->onCollisionStay(self, another); 
	});
}

Entity * Entity::clone() const {
	Entity* result = new Entity();
	result->category = category;
	result->destroyed = destroyed;
	for (auto it = properties.begin(); it != properties.end(); ++it) {
		result->properties.push_back((*it)->makeClone());
	}
	return result;
}
