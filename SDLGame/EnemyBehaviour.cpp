#include "EnemyBehaviour.h"
#include "Entity.h"
#include "MotionComponent.h"
#include "RectangleCollider.h"
#include "CollisionComponent.h"
#include "ModuleParticles.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Utils.h"
#include "AnimationComponent.h"
#include "Animation.h"
#include "LifeComponent.h"
#include "Collider.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "Particle.h"

IComponent* EnemyBehaviour::clone() const {
	EnemyBehaviour* result = new EnemyBehaviour(getID());
	return result;
}

bool EnemyBehaviour::start() {
	bool started = true;
	started = started & ((motion = static_cast<MotionComponent*>(parent->getComponent("motion"))) != nullptr);
	started = started & ((collision = static_cast<CollisionComponent*>(parent->getComponent("collider"))) != nullptr);
	started = started & ((life = static_cast<LifeComponent*>(parent->getComponent("life"))) != nullptr);
	started = started & ((animations = static_cast<AnimationComponent*>(parent->getComponent("animations"))) != nullptr);

	hit = App->audio->loadEffect("enemy_hit.wav");
	startRun = App->audio->loadEffect("enemy_caveman_run_start.wav");
	run = App->audio->loadEffect("enemy_caveman_run.wav");
	die = App->audio->loadEffect("enemy_caveman_die.wav");

	Animation anim(1);
	anim.sizeFrame = {64,64,64,64};
	anim.offset = {-32,-32};
	particleAttack = new Particle("effects.png", anim);
	particleAttack->life = 300;
	particleAttack->delay = 0;

	return started;
}

update_status EnemyBehaviour::update() {
	if (!life->isAlive()) {
		return UPDATE_CONTINUE;
	}

	Transform* entityTransform = this->parent->transform;
	Transform globalPlayer = App->player->player->transform->getGlobalTransform();
	Transform globalMine = Transform(entityTransform->getGlobalTransform());

	int attack = parent->controller.attack;
	//check if player is near to attack him
	if (attack == 1 || attack == 2) {
		//is attacking, check if finished the animation
		attacking(globalMine, globalPlayer);
	} else if (attack == 0) {
		//check if player is near to attack him
		checkCollisions(globalMine, globalPlayer);

	} else if (attack == 3) {
		//motion run
		runningAway(globalMine);
	}

	//put the enemy thinking what to do
	if (ticked && parent->controller.attack == 0) {
		updateMotion(globalMine, globalPlayer);
	}

	return UPDATE_CONTINUE;
}

update_status EnemyBehaviour::postUpdate() {
	return UPDATE_CONTINUE;
}

void EnemyBehaviour::attacking(Transform & globalMine, Transform& globalPlayer) {
	int attack = parent->controller.attack;

	const Animation* actualAnimation = animations->getActualAnimation();
	switch (attack) {
		case 1: //atacando, buscamos al jugador
			if (actualAnimation->isFinished()) {
				LOG("Enemy attacking");
				//la animaci�n ha terminado, est� en el �ltimo frame donde se hace efectivo el golpe
				//comprobamos colisi�n a ver si de verdad hemos dado al jugador o se nos ha escapado
				//check collision with player
				const Collider* myCollider = collision->getCollider();
				const Collider* colliderPlayer = static_cast<CollisionComponent*>(App->player->player->getComponent("collider"))->getCollider();

				//same position, but in front of the enemy (width)
				fPoint position = globalMine.position;
				//NONE -> looking left
				iPoint size = myCollider->getSize();
				Transform* entityTransform = this->parent->transform;
				position.x += (entityTransform->flip == SDL_FLIP_NONE) ? -size.x : size.x;

				iPoint rectangle = {31,size.y};
				RectangleCollider colliderCheck = RectangleCollider(position, rectangle, 0, TypeCollider::ENEMY);


				if (colliderPlayer->checkCollision(&colliderCheck)) {
					//damage player and notify him of the collision so he can react
					App->player->life->modifyActualLife(-20);
					App->audio->playEffect(hit);
					LOG("Enemy damage!!");

					position.y += size.y*0.5f;
					fPoint velocity = fPoint(0, 0);
					App->particles->addParticle(*particleAttack, position, velocity, particleAttack->delay);

					App->player->player->onCollisionEnter(colliderPlayer, myCollider);
				}


				//da igual si le hemos dado, escapamos! (segunda fase de atacar)
				parent->controller.attack = 2;
				App->audio->playEffect(startRun);
			}
			break;
		case 2: //precalentamiento de huida
			//si ha terminado, corremos!!
			if (actualAnimation->isFinished()) {
				//quitamos la colisi�n
				collision->disable();

				App->audio->playEffect(run);
				parent->controller.attack = 3;
				//direction of running
				Transform* entityTransform = this->parent->transform;
				float direction = 1;
				if (globalMine.position.x > globalPlayer.position.x) {//correr a derecha
					entityTransform->flip = SDL_FLIP_HORIZONTAL;
				} else {
					direction = -1;
					entityTransform->flip = SDL_FLIP_NONE;
				}
				motion->velocity.x += motion->doubleSpeed * direction * 1.2f;
			}
			break;
	}
}

void EnemyBehaviour::checkCollisions(Transform& globalMine, Transform& globalPlayer) {

	Transform* entityTransform = this->parent->transform;

	//check collision with player
	const Collider* myCollider = collision->getCollider();
	const Collider* colliderPlayer = static_cast<CollisionComponent*>(App->player->player->getComponent("collider"))->getCollider();

	//same position, but in front of the enemy (width)
	fPoint position = globalMine.position;
	//NONE -> looking left
	iPoint size = myCollider->getSize();
	position.x += (entityTransform->flip == SDL_FLIP_NONE) ? -size.x + 10 : size.x;

	iPoint rectangle = {10,size.y};
	RectangleCollider colliderCheck = RectangleCollider(position, rectangle, 0, TypeCollider::ENEMY);


	if (colliderPlayer->checkCollision(&colliderCheck)) {
		//inmediatly react attacking
		parent->controller.attack = 1;
		//stop motion
		MotionComponent* motion = static_cast<MotionComponent*>(parent->getComponent("motion"));
		motion->velocity.setToZero();
		parent->controller.attack = 1;
	}
}

void EnemyBehaviour::updateMotion(Transform& globalMine, Transform& globalPlayer) {
	Transform* entityTransform = this->parent->transform;
	ControlEntity* controller = &parent->controller;

	MotionComponent* motion = static_cast<MotionComponent*>(parent->getComponent("motion"));

	controller->moveX = 0;
	controller->moveY = 0;
	controller->attack = 0;

	float distance = abs(globalPlayer.position.x - globalMine.position.x);
	if (globalPlayer.position.x < globalMine.position.x) {
		controller->moveX -= 1;
		entityTransform->flip = SDL_FLIP_NONE;
		if (distance > 150) {
			controller->run = true;
		}
	}

	if (globalPlayer.position.x > globalMine.position.x) {
		controller->moveX += 1;
		entityTransform->flip = SDL_FLIP_HORIZONTAL;
		if (distance > 80) {
			controller->run = true;
		}
	}

	/*if (App->input->getKey(SDL_SCANCODE_W)) {
	controller->moveY -= 1;
	}

	if (App->input->getKey(SDL_SCANCODE_S)) {
	controller->moveY += 1;
	}*/


	//don't jump again when jumping or falling
	/*if (App->input->getKey(SDL_SCANCODE_KP_0) && controller->stateJump == JumpType::NONE) {
	if (App->input->getKey(SDL_SCANCODE_W)) {
	controller->stateJump = JumpType::DOUBLE_JUMP;
	} else {
	controller->stateJump = JumpType::JUMP;
	}
	}*/

	if (controller->moveY != 1) {
		motion->velocity.x = (controller->run) ? controller->moveX * motion->doubleSpeed : controller->moveX * motion->speed;
	} else {
		motion->velocity.x = 0.0f;
	}
}

void EnemyBehaviour::runningAway(Transform& globalMine) {
	//check if dead to set corresponding motion
	const Collider* myCollider = (static_cast<CollisionComponent*>(parent->getComponent("collider")))->getCollider();
	iPoint size = myCollider->getSize();
	fPoint position = globalMine.position;
	int x = static_cast<int>(position.x);
	int y = static_cast<int>(position.y);
	SDL_Rect rect = {x, y , size.x, size.y};
	if (!App->renderer->insideCamera(rect)) {
		parent->destroy();
	}
}
