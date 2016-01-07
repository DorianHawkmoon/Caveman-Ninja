#include "DisappearOutCamera.h"

#include "Collider.h"
#include "Condition.h"
#include "Entity.h"
#include "Application.h"
#include "ModuleRender.h"
#include "CollisionComponent.h"

DisappearOutCamera::DisappearOutCamera(const std::string& name): IComponent(name) {}


DisappearOutCamera::~DisappearOutCamera() {
	for (auto it = conditions.begin(); it != conditions.end(); ++it) {
		delete *it;
	}
	conditions.clear();
}

update_status DisappearOutCamera::update() {
	//if necessary conditions is true and check if entity is outside of the camera and destroy
	if (checkCondition() && outsideCamera()) {
		parent->destroy();
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
	CollisionComponent* collider=static_cast<CollisionComponent*>(parent->getComponent("collider"));
	const Collider* coll=collider->getCollider();
	fPoint position = coll->getGlobalTransform().position;
	iPoint size = coll->getSize();

	SDL_Rect result = {position.x,position.y, size.x, size.y};
	return !App->renderer->insideCamera(result);
}
