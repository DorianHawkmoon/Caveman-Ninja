#include "PlayerHittedComponent.h"
#include "Collider.h"
#include "ModuleAudio.h"
#include "MotionComponent.h"
#include "Entity.h"
#include "LifeComponent.h"
#include "Application.h"
#include "CollisionComponent.h"
#include "DamageComponent.h"

PlayerHittedComponent::PlayerHittedComponent(const std::string& name): IComponent(name), timer(), dead(false) {}


PlayerHittedComponent::~PlayerHittedComponent() {}

bool PlayerHittedComponent::start() {
	bool result = true;
	timer.stop();
	result = result & ((life = static_cast<LifeComponent*>(this->parent->getComponent("life"))) != nullptr);
	//damage = static_cast<DamageComponent*>(this->parent->getComponent("damage"));
	result = result & ((motion = static_cast<MotionComponent*>(this->parent->getComponent("motion"))) != nullptr);
	result = result & ((collision = static_cast<CollisionComponent*>(this->parent->getComponent("collider"))) != nullptr);
	damageReceivedEffect = App->audio->loadEffect("hurt.wav");
	return result;
}

update_status PlayerHittedComponent::preUpdate() {
	if (toClean) {
		cleanUp();
		toClean = false;
	}
	return UPDATE_CONTINUE;
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
		} else if (timer.value() >= 800 && life->isAlive()) {
			hitted = false;
			collision->enable();
			parent->controller.damage = 0;
			timer.stop();

		} else if (timer.value() >= 50 && !life->isAlive()) {
			dead = true;
			int dam = (parent->controller.damage>0)? 1:-1;
			parent->controller.damage = dam*3;

			timer.stop();
		}
	}
	return UPDATE_CONTINUE;
}

bool PlayerHittedComponent::cleanUp() {
	if (!cleaned) {
		collision = nullptr;
		life = nullptr;
		motion = nullptr;
		cleaned = true;
	}
	return true;
}

IComponent * PlayerHittedComponent::makeClone() const {
	return new PlayerHittedComponent(getID());
}

void PlayerHittedComponent::onCollisionEnter(const Collider * self, const Collider * another) {
	//si ya es golpeado (o esta muerto), no volver a golpear hasta que est� bien
	if (hitted || dead || self!=collision->getCollider()) {
		return;
	}
	//if is the enemy...
	if (another->type !=TypeCollider::ENEMY){
		return;
	}

	//check if enemy is alive or not being hitted
	CollisionComponent* col = static_cast<CollisionComponent*>(another->parent->getComponent("collider"));
	if (col==nullptr || !col->isEnable()) {
		return;
	}

	//for both functions, I need the global transforms, dont get it twice
	Transform globalAnother = another->getGlobalTransform();
	Transform globalMine = Transform(this->parent->transform->getGlobalTransform());

	//check if i'm hitting from up
	if (hittedEnemy(globalAnother, globalMine)) {
		//do the collision with to the other
		Collider* colliderHitted = self->clone();
		colliderHitted->type = TypeCollider::PLAYER_SHOT;
		colliderHitted->parent = parent;
		another->parent->onCollisionEnter(another,colliderHitted);
		delete colliderHitted;
		//TODO make the particle
	} else {
		hittedMyself(globalAnother, globalMine);
	}

	
}

bool PlayerHittedComponent::hittedEnemy(const Transform& globalAnother, const Transform& globalMine) {
	ControlEntity* controller = &parent->controller;
	bool result = false;

	//player can't be quiet, must be falling or at least jumping
	if (controller->stateJump != JumpType::NONE) {
		return result;
	}

	//check if y is above enemy
	int yPlayer = static_cast<int>(globalMine.position.y + collision->getCollider()->getSize().y);
	int yEnemy = static_cast<int>(globalAnother.position.y);

	//tolerance distance
	if (yPlayer - 5 - yEnemy < 0) {
		result = true;
		//do actions
		controller->stateJump = JumpType::JUMP; //animations as jump
		motion->velocity.y = 5; //jump again a little
	}

	return result;
}

bool PlayerHittedComponent::hittedMyself(const Transform& globalAnother, const Transform& globalMine) {
	ControlEntity* controller = &parent->controller;

	
	int x = (globalAnother.position.x < globalMine.position.x) ? 1 : -1; //positivo me han dado por la izquierda
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

	motion->velocity.x = x*50.0f;
	motion->velocity.y -= 100.0f;

	hitted = true;
	//play the sound effect
	App->audio->playEffect(damageReceivedEffect);
	return true;
}
