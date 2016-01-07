#include "PlayerHittedComponent.h"
#include "Collider.h"
#include "MotionComponent.h"
#include "Entity.h"
#include "LifeComponent.h"

PlayerHittedComponent::PlayerHittedComponent(const std::string& name): IComponent(name), timer() {}


PlayerHittedComponent::~PlayerHittedComponent() {}

bool PlayerHittedComponent::start() {
	timer.stop();
	return true;
}

update_status PlayerHittedComponent::update() {
	//si hitted and alive and not falling more, damage to zero
	if (hitted) {
		if (timer.isStopped()) {
			MotionComponent* motion = static_cast<MotionComponent*>(this->parent->getComponent("motion"));
			if (motion->velocity.y == 0) {
				motion->velocity.x = 0;
				timer.start();
			}
		} else {
			if (timer.value() >= 800) {
				LifeComponent* life = static_cast<LifeComponent*>(this->parent->getComponent("life"));
				if (life->isAlive()) {
					hitted = false;
					parent->controller.damage = 0;
				}
			}
		}
	}
	return UPDATE_CONTINUE;
}

IComponent * PlayerHittedComponent::makeClone() {
	return new PlayerHittedComponent(getID());
}

void PlayerHittedComponent::onCollisionEnter(const Collider * self, const Collider * another) {
	//si ya es golpeado, no volver a golpear hasta que esté bien
	if (hitted) {
		return;
	}
	//if is the enemy, check where come from and set motion
	if (another->type !=TypeCollider::ENEMY){
		return;
	}

	ControlEntity* controller = &parent->controller;

	Transform globalAnother = another->getGlobalTransform();
	Transform globalMine = Transform(this->parent->transform->getGlobalTransform());
	int x= (globalAnother.position.x < globalMine.position.x) ? 1 : -1; //positivo me han dado por la izquierda

	//seteo la animación que toca
	if (x == 1 && parent->transform->flip == SDL_FLIP_NONE) { //me dan por la izquierda y estoy mirando a la derecha, por detrás
		parent->controller.damage = -1;
	} else if (x == 1 && parent->transform->flip == SDL_FLIP_HORIZONTAL) { //me dan de frente
		parent->controller.damage = 1;
	} else if (parent->transform->flip == SDL_FLIP_NONE) { //miro a la derecha y me dan de frente
		parent->controller.damage = 1;
	} else { //miro izquierda y me dan por detrás
		parent->controller.damage = -1;
	}

	MotionComponent* motion = static_cast<MotionComponent*>(this->parent->getComponent("motion"));
	motion->velocity.x = x*100;
	motion->velocity.y -= 150;

	hitted = true;
}
