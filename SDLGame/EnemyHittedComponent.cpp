#include "EnemyHittedComponent.h"
#include "Transform.h"
#include "Collider.h"
#include "MotionComponent.h"
#include "Entity.h"
#include "LifeComponent.h"
#include "CollisionComponent.h"
#include "DamageComponent.h"
#include "Application.h"
#include "AnimationComponent.h"
#include "Animation.h"
#include "DamageComponent.h"
#include "ModuleAudio.h"
#include "ModuleScene.h"
#include "DropItemComponent.h"


EnemyHittedComponent::EnemyHittedComponent(const std::string& name) : IComponent(name), timer(), dead(false), playSound(true) {}


EnemyHittedComponent::~EnemyHittedComponent() {}

bool EnemyHittedComponent::start() {
	bool result = true;
	timer.stop();
	result = result & ((life = static_cast<LifeComponent*>(this->parent->getComponent("life"))) != nullptr);
	result = result & ((motion = static_cast<MotionComponent*>(this->parent->getComponent("motion"))) != nullptr);
	result = result & ((collision = static_cast<CollisionComponent*>(this->parent->getComponent("collider"))) != nullptr);
	soundDie = App->audio->loadEffect("enemy_caveman_die.wav");
	return result;
}

update_status EnemyHittedComponent::preUpdate() {
	if (toClean) {
		cleanUp();
		toClean = false;
	}
	return UPDATE_CONTINUE;
}

update_status EnemyHittedComponent::update() {
	if (dead) {
		deadUpdate();
	}else if (hitted) { 	//si hitted and alive and not falling more, damage to zero
		if (playSound && !life->isAlive()) {
				//sound 
				App->audio->playEffect(soundDie);
				playSound = false;
		}

		if (timer.isStopped()) {
			if (motion->velocity.y == 0) {
				motion->velocity.x = 0;
				timer.start();
			}

		} else if (timer.value() >= 800 && life->isAlive()) {
			hitted = false;
			collision->enable();
			parent->controller.damage = 0;
			timer.stop();

		} else if (timer.value() >= 100 && !life->isAlive()) {
			dead = true;
			int dam = parent->controller.damage;
			parent->controller.damage = (dam / dam) * 3;

			timer.stop();
		}

	}
	return UPDATE_CONTINUE;
}

bool EnemyHittedComponent::cleanUp() {
	if (!cleaned) {
		collision = nullptr;
		life = nullptr;
		motion = nullptr;
		cleaned = true;
	}
	return true;
}

IComponent * EnemyHittedComponent::makeClone() const {
	return new EnemyHittedComponent(getID());
}

void EnemyHittedComponent::onCollisionEnter(const Collider * self, const Collider * another) {
	//si ya es golpeado, no volver a golpear hasta que esté bien (o esta muerto)
	if (hitted || dead) {
		return;
	}
	//if is the enemy, check where come from and set motion
	if (another->type != TypeCollider::PLAYER_SHOT) {
		return;
	}

	ControlEntity* controller = &parent->controller;

	Transform globalAnother = another->getGlobalTransform();
	Transform globalMine = Transform(this->parent->transform->getGlobalTransform());
	int x = (globalAnother.position.x < globalMine.position.x) ? -1 : 1; //positivo me han dado por la derecha

	DamageComponent* damage = static_cast<DamageComponent*>(another->parent->getComponent("damage"));
	int strong = (damage != nullptr && damage->strong) ? 2 : 1;
	if (damage != nullptr) {
		//do damage
		life->modifyActualLife(-damage->getDamage());
	}

	//seteo la animación que toca
	if (x == 1 && parent->transform->flip == SDL_FLIP_NONE) { 
		parent->controller.damage = -strong;
	} else if (x == 1 && parent->transform->flip == SDL_FLIP_HORIZONTAL) {
		parent->controller.damage = strong;
	} else if (parent->transform->flip == SDL_FLIP_NONE) {
		parent->controller.damage = strong;
	} else {
		parent->controller.damage = -strong;
	}

	motion->velocity.x = -1 * x * 100.0f;
	motion->velocity.y -= 150.0f;

	hitted = true;
	collision->disable();
}

void EnemyHittedComponent::deadUpdate() {
	if (timer.isStopped()) {
		//start the time of dead animation
		timer.start();
		//drop the item if i have one
		DropItemComponent* drop = static_cast<DropItemComponent*>(parent->getComponent("drop"));
		if (drop != nullptr && drop->item != nullptr) {
			Entity* item = drop->item->clone();
			Transform global = parent->transform->getGlobalTransform();
			item->transform->position = global.position;
			iPoint size =collision->getCollider()->getSize();
			item->transform->position.y += size.y;

			//prepared, put it on the scene
			App->scene->addEntity(item);
		}

	} else if (timer.value() > 500) {
		AnimationComponent* animation = static_cast<AnimationComponent*>(parent->getComponent("animations"));
		if (animation != nullptr) {
			if (animation->getActualAnimation()->isFinished()) {
				parent->destroy();
			}
		} else {
			parent->destroy();
		}
	}
}
