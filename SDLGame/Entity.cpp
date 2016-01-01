#include "Entity.h"
#include <algorithm>
#include <functional>
#include "IComponent.h"

Entity::Entity(Category category) : destroyed(false), category(category), transform()
{

}


Entity::~Entity() {
	//just in case, clean up
	cleanUp();

	//destroy entities
	for (std::list<IComponent*>::iterator it = properties.begin(); it != properties.end(); ++it) {
		delete (*it);
	}
	properties.clear();
}

bool Entity::start() {
	bool result = true;
	transform.setToZero();
	for (std::list<IComponent*>::iterator it = properties.begin(); it != properties.end() && result; ++it) {
		result &= (*it)->start();
	}
	return result;
}

update_status Entity::preUpdate() {
	update_status result = UPDATE_CONTINUE;
	for (std::list<IComponent*>::iterator it = properties.begin(); it != properties.end() && result==UPDATE_CONTINUE; ++it) {
		result = (*it)->preUpdate();
	}
	return result;
}

update_status Entity::update() {
	update_status result = UPDATE_CONTINUE;
	for (std::list<IComponent*>::iterator it = properties.begin(); it != properties.end() && result == UPDATE_CONTINUE; ++it) {
		result = (*it)->update();
	}
	return result;
}

update_status Entity::postUpdate() {
	update_status result=UPDATE_CONTINUE;
	for (std::list<IComponent*>::iterator it = properties.begin(); it != properties.end() && result == UPDATE_CONTINUE; ++it) {
		result = (*it)->postUpdate();
	}
	return result;
}

bool Entity::cleanUp() {
	bool result = true;
	for (std::list<IComponent*>::iterator it = properties.begin(); it != properties.end() && result; ++it) {
		result &= (*it)->cleanUp();
	}
	return result;
}

bool Entity::addComponent(IComponent * component) {
	bool result = false;
	if (component != nullptr) {
		std::list<IComponent*>::iterator it = std::find_if(properties.begin(), properties.end(),
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

	if (it != properties.end()) {
		result = *it;
	}
	return result;
}

bool Entity::removeComponent(IComponent * component) {
	properties.remove(component);
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
