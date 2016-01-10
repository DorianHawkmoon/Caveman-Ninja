#include "EnemyHittedComponent.h"
#include "Collider.h"
#include "MotionComponent.h"
#include "Entity.h"
#include "LifeComponent.h"
#include "CollisionComponent.h"
#include "DamageComponent.h"
#include "AnimationComponent.h"
#include "Animation.h"


EnemyHittedComponent::EnemyHittedComponent(const std::string& name) : IComponent(name), timer(), dead(false) {}


EnemyHittedComponent::~EnemyHittedComponent() {}

bool EnemyHittedComponent::start() {
	bool result = true;
	timer.stop();
	result = result & ((life = static_cast<LifeComponent*>(this->parent->getComponent("life"))) != nullptr);
	result = result & ((motion = static_cast<MotionComponent*>(this->parent->getComponent("motion"))) != nullptr);
	result = result & ((collision = static_cast<CollisionComponent*>(this->parent->getComponent("collider"))) != nullptr);
	return result;
}

update_status EnemyHittedComponent::update() {
	if (dead) {
		if (timer.isStopped()) {
			timer.start();
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
		return UPDATE_CONTINUE;
	}

	//si hitted and alive and not falling more, damage to zero
	if (hitted) {
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

IComponent * EnemyHittedComponent::makeClone() {
	return new EnemyHittedComponent(getID());
}

void EnemyHittedComponent::onCollisionEnter(const Collider * self, const Collider * another) {
	//si ya es golpeado, no volver a golpear hasta que esté bien (o esta muerto)
	if (hitted || dead) {
		return;
	}
	//if is the enemy, check where come from and set motion
	if (another->type != TypeCollider::PLAYER_SHOT && another->type != TypeCollider::PLAYER) {
		return;
	}

	ControlEntity* controller = &parent->controller;

	Transform globalAnother = another->getGlobalTransform();
	Transform globalMine = Transform(this->parent->transform->getGlobalTransform());
	int x = (globalAnother.position.x < globalMine.position.x) ? -1 : 1; //positivo me han dado por la derecha

	//seteo la animación que toca
	if (x == 1 && parent->transform->flip == SDL_FLIP_NONE) { 
		parent->controller.damage = -1;
	} else if (x == 1 && parent->transform->flip == SDL_FLIP_HORIZONTAL) {
		parent->controller.damage = 1;
	} else if (parent->transform->flip == SDL_FLIP_NONE) {
		parent->controller.damage = 1;
	} else {
		parent->controller.damage = -1;
	}

	motion->velocity.x = -1 * x * 100.0f;
	motion->velocity.y -= 150.0f;

	hitted = true;
	collision->disable();
}
