#include "PlayerHittedComponent.h"
#include "Collider.h"
#include "ModuleAudio.h"
#include "MotionComponent.h"
#include "Entity.h"
#include "LifeComponent.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleParticles.h"
#include "CollisionComponent.h"
#include "DamageComponent.h"

PlayerHittedComponent::PlayerHittedComponent(const std::string& name) : IComponent(name), timer(), dead(false), securityTimer() {

}


PlayerHittedComponent::~PlayerHittedComponent() {}

bool PlayerHittedComponent::start() {
	bool result = true;
	timer.stop();
	result = result & ((life = static_cast<LifeComponent*>(this->parent->getComponent("life"))) != nullptr);
	result = result & ((motion = static_cast<MotionComponent*>(this->parent->getComponent("motion"))) != nullptr);
	result = result & ((collision = static_cast<CollisionComponent*>(this->parent->getComponent("collider"))) != nullptr);
	result = result & ((textureEffects = App->textures->load("effects.png")) != nullptr);

	Animation anim(1);
	anim.sizeFrame = {192,0,64,64};
	anim.offset = {-32,-32};
	particleJumped = new Particle("effects.png", anim);
	particleJumped->life = 300;
	particleJumped->delay = 0;


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
			if (securityTimer.isStopped()) {
				securityTimer.start();
			} else if (securityTimer.value() > 2000) {
				timer.start();
			}
		} else if (timer.value() >= 600 && life->isAlive()) {
			hitted = false;
			collision->enable();
			parent->controller.damage = 0;
			timer.stop();
			securityTimer.stop();

		} else if (timer.value() >= 50 && !life->isAlive()) {
			dead = true;
			int dam = (parent->controller.damage > 0) ? 1 : -1;
			parent->controller.damage = dam * 3;
			securityTimer.stop();
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
		if (particleJumped != nullptr) {
			delete particleJumped;
		}
	}
	return true;
}

IComponent * PlayerHittedComponent::makeClone() const {
	return new PlayerHittedComponent(getID());
}

void PlayerHittedComponent::onCollisionEnter(const Collider * self, const Collider * another) {
	//si ya es golpeado (o esta muerto), no volver a golpear hasta que esté bien
	if (hitted || dead || self != collision->getCollider()) {
		return;
	}
	//if is the enemy...
	if (another->type != TypeCollider::ENEMY) {
		return;
	}

	//check if enemy is alive or not being hitted
	CollisionComponent* col = static_cast<CollisionComponent*>(another->parent->getComponent("collider"));
	if (col == nullptr || !col->isEnable()) {
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
		another->parent->onCollisionEnter(another, colliderHitted);
		delete colliderHitted;

	} else if (hittedMyself(globalAnother, globalMine)) {
		//get damage if is the case
		DamageComponent* damage = static_cast<DamageComponent*>(another->parent->getComponent("damage"));
		if (damage != nullptr) {
			//do damage
			life->modifyActualLife(-damage->getDamage());
		}
	}
}

bool PlayerHittedComponent::hittedEnemy(const Transform& globalAnother, const Transform& globalMine) {
	ControlEntity* controller = &parent->controller;
	bool result = false;

	//player can't be quiet, must be falling or at least jumping
	if (controller->stateJump == TypeJump::NONE) {
		return result;
	}

	//check if y is above enemy
	int yPlayer = static_cast<int>(globalMine.position.y + collision->getCollider()->getSize().y);
	int yEnemy = static_cast<int>(globalAnother.position.y);

	//tolerance distance
	if (yPlayer - (10 * SCREEN_SIZE) - yEnemy < 0) {
		result = true;
		//do actions
		controller->stateJump = TypeJump::JUMP; //animations as jump
		motion->velocity.y = -1 * (motion->speed*0.15f); //jump again a little

		fPoint position = globalMine.position;
		position.y = static_cast<float>(yEnemy);
		fPoint velocity = fPoint(0, 0);
		App->particles->addParticle(*particleJumped, position, velocity, particleJumped->delay);
	}

	return result;
}

bool PlayerHittedComponent::hittedMyself(const Transform& globalAnother, const Transform& globalMine) {
	ControlEntity* controller = &parent->controller;


	int x = (globalAnother.position.x < globalMine.position.x) ? 1 : -1; //positivo me han dado por la izquierda
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

	motion->velocity.x = x*50.0f;
	motion->velocity.y -= 100.0f;

	hitted = true;

	//play the sound effect
	App->audio->playEffect(damageReceivedEffect);
	return true;
}
