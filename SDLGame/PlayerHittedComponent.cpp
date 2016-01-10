#include "PlayerHittedComponent.h"
#include "Collider.h"
#include "MotionComponent.h"
#include "Entity.h"
#include "LifeComponent.h"
#include "CollisionComponent.h"

PlayerHittedComponent::PlayerHittedComponent(const std::string& name): IComponent(name), timer(), dead(false) {}


PlayerHittedComponent::~PlayerHittedComponent() {}

bool PlayerHittedComponent::start() {
	bool result = true;
	timer.stop();
	result = result & ((life = static_cast<LifeComponent*>(this->parent->getComponent("life"))) != nullptr);
	result = result & ((motion = static_cast<MotionComponent*>(this->parent->getComponent("motion"))) != nullptr);
	return result;
}

update_status PlayerHittedComponent::update() {
	if (dead) {
		return UPDATE_CONTINUE;
	}
	//si hitted and alive and not falling more, damage to zero
	if (hitted) {
		if (timer.isStopped()) {
			if (motion->velocity.y == 0) {
				motion->velocity.x = 0;
				timer.start();
			}
		} else {
			if (timer.value() >= 800) {
				if (life->isAlive()) {
					hitted = false;
					parent->controller.damage = 0;
				} else {
					dead = true;
					//parent->controller.damage = 0; //to -2 if dead?
				}
				timer.stop();
				
			}
		}
	}
	return UPDATE_CONTINUE;
}

IComponent * PlayerHittedComponent::makeClone() {
	return new PlayerHittedComponent(getID());
}

void PlayerHittedComponent::onCollisionEnter(const Collider * self, const Collider * another) {
	//si ya es golpeado, no volver a golpear hasta que est� bien
	if (hitted || dead) {
		return;
	}
	//if is the enemy, check where come from and set motion
	if (another->type !=TypeCollider::ENEMY){
		return;
	}

	//check if enemy is alive or not being hitted
	CollisionComponent* col = static_cast<CollisionComponent*>(another->parent->getComponent("collider"));
	if (col==nullptr || !col->isEnable()) {
		return;
	}
	ControlEntity* controller = &parent->controller;

	Transform globalAnother = another->getGlobalTransform();
	Transform globalMine = Transform(this->parent->transform->getGlobalTransform());
	int x= (globalAnother.position.x < globalMine.position.x) ? 1 : -1; //positivo me han dado por la izquierda

	//seteo la animaci�n que toca
	if (x == 1 && parent->transform->flip == SDL_FLIP_NONE) { //me dan por la izquierda y estoy mirando a la derecha, por detr�s
		parent->controller.damage = -1;
	} else if (x == 1 && parent->transform->flip == SDL_FLIP_HORIZONTAL) { //me dan de frente
		parent->controller.damage = 1;
	} else if (parent->transform->flip == SDL_FLIP_NONE) { //miro a la derecha y me dan de frente
		parent->controller.damage = 1;
	} else { //miro izquierda y me dan por detr�s
		parent->controller.damage = -1;
	}

	motion->velocity.x = x*100.0f;
	motion->velocity.y -= 150.0f;

	hitted = true;
}
