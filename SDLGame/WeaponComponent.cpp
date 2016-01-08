#include "WeaponComponent.h"


WeaponComponent::~WeaponComponent() {}

bool WeaponComponent::start() {
	time.start();
	return true;
}

update_status WeaponComponent::preUpdate() {
	for (auto it = weapons.begin(); it != weapons.end();) {
		if ((*it)->isDestroyed()) {
			it = weapons.erase(it);

		} else {
			++it;
		}
	}
	return UPDATE_CONTINUE;
}

IComponent * WeaponComponent::makeClone() {
	WeaponComponent* result = new WeaponComponent(getID(), maxThrowable, delay);
	return result;
}

bool WeaponComponent::limitReached() const {
	bool result = false;
	if (maxThrowable == 0) {
		result = true;
	} else if (maxThrowable > 0 && static_cast<int>(weapons.size()) > maxThrowable) {
		result = true;
	}
	return result;
}

void WeaponComponent::throwWeapon() {
	ControlEntity* controller = &parent->controller;
	Transform* trans = parent->transform;

	controller->attack = (trans->flip == SDL_FLIP_HORIZONTAL) ? -1 : 1;//segun la dirección de joe
	if (static_cast<int>(weapons.size()) < maxThrowable && time.value() > delay) {
		time.start();
		Entity* entity = PlayerWeapon::makeWeapon();
		weapons.push_back(entity);
		entity->start();

		entity->transform->position += trans->position;
		const Collider* colliderSize;
		if (controller->attack == -1) {
			entity->transform->flip = SDL_FLIP_HORIZONTAL;
			colliderSize = (static_cast<CollisionComponent*>(entity->getComponent("collider")))->getCollider();
		} else {
			colliderSize = (static_cast<CollisionComponent*>(parent->getComponent("collider")))->getCollider();
		}
		entity->transform->position.x += controller->attack * (colliderSize->getSize().x + 2);

		MotionComponent* motion = static_cast<MotionComponent*>(entity->getComponent("motion"));
		motion->velocity.x += controller->attack * 115;
		motion->velocity.y -= 130;

		//give it to scene module
		App->scene->addEntity(entity);
	}
}
