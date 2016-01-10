#include "DisappearOutCamera.h"

#include "Collider.h"
#include "Condition.h"
#include "Entity.h"
#include "Application.h"
#include "ModuleRender.h"
#include "CollisionComponent.h"

DisappearOutCamera::DisappearOutCamera(const std::string& name): IComponent(name), cleaned(true) {}


DisappearOutCamera::~DisappearOutCamera() {
	for (auto it = conditions.begin(); it != conditions.end(); ++it) {
		delete *it;
	}
	conditions.clear();
}

bool DisappearOutCamera::start() {
	bool result=(collider = static_cast<CollisionComponent*>(parent->getComponent("collider"))) != nullptr;
	cleaned = !result;
	return result;
}

update_status DisappearOutCamera::preUpdate() {
	if (toClean) {
		cleanUp();
		toClean = false;
	}
	return UPDATE_CONTINUE;
}

update_status DisappearOutCamera::update() {
	if (collider->isEnable()) {
	//if necessary conditions is true and check if entity is outside of the camera and destroy
		if (checkCondition() && outsideCamera()) {
			parent->destroy();
		}
	}
	return UPDATE_CONTINUE;
}

void DisappearOutCamera::addCondition(const Condition * condition) {
	if (condition != nullptr) {
		Condition* copy = condition->clone(); //owned condition
		if (std::find(conditions.begin(), conditions.end(), copy) == conditions.end()) {
			conditions.push_back(copy);
		}
	}
}

bool DisappearOutCamera::cleanUp() {
	if (!cleaned) {
		collider = nullptr;
		cleaned = true;
	}
	return true;
}

IComponent * DisappearOutCamera::makeClone() {
	DisappearOutCamera* result = new DisappearOutCamera(getID());
	for (auto it = conditions.begin(); it != conditions.end(); ++it) {
		result->addCondition(*it);
	}
	return result;
}

bool DisappearOutCamera::checkCondition() {
	bool result = true;
	for (std::list<Condition*>::iterator it = conditions.begin(); it != conditions.end() && result; ++it) {
		result = (*it)->check();
	}
	return result;
}

bool DisappearOutCamera::outsideCamera() {
	//get the collision of parent
	const Collider* coll=collider->getCollider();
	fPoint position = coll->getGlobalTransform().position;
	iPoint size = coll->getSize();

	SDL_Rect result = {position.x,position.y, size.x, size.y};
	return !App->renderer->insideCamera(result);
}
