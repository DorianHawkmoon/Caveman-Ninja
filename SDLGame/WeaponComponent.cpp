#include "WeaponComponent.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "Entity.h"
#include "Collider.h"

WeaponComponent::WeaponComponent(const std::string & name, int maxThrowable, unsigned int delay) : IComponent(name), maxThrowable(maxThrowable), delay(delay), weapons() {}

WeaponComponent::~WeaponComponent() {}

bool WeaponComponent::start() {
	time.start();
	//prepare sound
	soundEffect = App->audio->loadEffect("attack.wav");
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

IComponent * WeaponComponent::makeClone() const {
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

	//can throw?
	if (static_cast<int>(weapons.size()) < maxThrowable && time.value() > delay) {
		controller->attack = (trans->flip == SDL_FLIP_HORIZONTAL) ? -1 : 1;//segun la dirección de joe
		time.start();
		//create weapon
		Entity* entity = PlayerWeapon::makeWeapon();
		weapons.push_back(entity);

		//get position player
		entity->transform->position += trans->position;
		const Collider* colliderSize;
		iPoint offset(0, 0);
		//set the position of weapon depending of where is player
		if (controller->attack == -1) {
			entity->transform->flip = SDL_FLIP_HORIZONTAL;
			colliderSize = (static_cast<CollisionComponent*>(entity->getComponent("collider")))->getCollider();
		} else {
			colliderSize = (static_cast<CollisionComponent*>(parent->getComponent("collider")))->getCollider();
			offset.x += static_cast<int>(colliderSize->position.x*2);
		}

		//if looking up, dont move x
		if (controller->moveY != -1) {
			entity->transform->position.x += controller->attack * (colliderSize->getSize().x + 2);
		}
		//if looking up, go up
		if (controller->moveY == 1) {
			entity->transform->position.y+=colliderSize->getSize().y*0.5f;
		} else if(controller->moveY ==-1) {
			entity->transform->position.x -= colliderSize->getSize().x*0.5f;
		}

		//set move of weapon
		MotionComponent* motion = static_cast<MotionComponent*>(entity->getComponent("motion"));
		if (controller->moveY == -1) {
			motion->velocity.y -= abs(controller->attack) * 200;
			motion->velocity.x = 0;
		} else {
			motion->velocity.x += controller->attack * 115;
			motion->velocity.y = 0;
		}

		//give it to scene module
		App->scene->addEntity(entity);
		//throw the sound attack
		App->audio->playEffect(soundEffect);
	}
}
