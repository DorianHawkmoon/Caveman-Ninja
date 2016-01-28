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
#include "JumpComponent.h"
#include "Globals.h"
#include "LifespanComponent.h"

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
	started = started & ((jump = static_cast<JumpComponent*>(parent->getComponent("jump"))) != nullptr);

	state = SEARCHING;
	controlIA.start();

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

	//first at all, look if i attack
	if (state != RUN_AWAY && state != ATTACK) {
		checkCollisions(globalMine, globalPlayer);
	}

	switch (state) {
		case SEARCHING:
			search(globalMine, globalPlayer);
			break;
		case RUN:
			running(globalMine, globalPlayer);
			break;
		case FORWARD:
			forward(globalMine, globalPlayer);
			break;
		case JUMP:
			jumping(globalMine, globalPlayer);
			break;
		case RUN_AWAY:
			runningAway(globalMine);
			break;
		case ATTACK:
			attacking(globalMine, globalPlayer);
			break;
	}

	return UPDATE_CONTINUE;
}

update_status EnemyBehaviour::postUpdate() {
	return UPDATE_CONTINUE;
}

void EnemyBehaviour::attacking(const Transform & globalMine, const Transform& globalPlayer) {
	int attack = parent->controller.attack;

	const Animation* actualAnimation = animations->getActualAnimation();
	switch (attack) {
		case 1: //atacando, buscamos al jugador
			if (actualAnimation->isFinished()) {
				LOG("Enemy attacking", "");
				//la animación ha terminado, está en el último frame donde se hace efectivo el golpe
				//comprobamos colisión a ver si de verdad hemos dado al jugador o se nos ha escapado
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
					App->audio->playEffect(hit);
					LOG("Enemy damage!!", "");

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
				//set a lifetime
				LifespanComponent* lifespan = new LifespanComponent("lifespan", 5000);
				lifespan->start();
				parent->addComponent(lifespan);

				state = RUN_AWAY;
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

void EnemyBehaviour::checkCollisions(const Transform& globalMine, const Transform& globalPlayer) {
	if (parent->controller.stateJump != TypeJump::NONE) {
		return;
	}
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
		state = ATTACK;
		parent->controller.attack = 1;
		//stop motion
		MotionComponent* motion = static_cast<MotionComponent*>(parent->getComponent("motion"));
		motion->velocity.setToZero();
		parent->controller.attack = 1;
	}
}

void EnemyBehaviour::forward(const Transform& globalMine, const Transform& globalPlayer) {
	ControlEntity* controller = &parent->controller;
	//I'm walking for a time
	if (controlIA.value() > 2000) {
		//stop and search for the player
		iPoint player = directionPlayer(globalMine, globalPlayer);
		previousplayer = player;
		state = SEARCHING;
		//decide inmediatly or thinking?
		if (Utils::range(3) == 0) {
			controlIA.stop();
			controlIA.start();
		}
	} else {
		controller->run = false;
		setController(iPoint(previousplayer.x, 0));
	}
}

void EnemyBehaviour::runningAway(const Transform& globalMine) {
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

void EnemyBehaviour::search(const Transform & globalMine, const  Transform & globalPlayer) {
	ControlEntity* controller = &parent->controller;
	if (controlIA.value() < 1000) {
		setController(iPoint(0, previousplayer.y));
		return;
	}
	//i was looking for player, time for a decision
	iPoint direction = directionPlayer(globalMine, globalPlayer);
	//first, up/down
	if (previousplayer.y != 0 && previousplayer.y == direction.y) {
		//jump to him!!
		if (direction.y < 0) {
			parent->controller.stateJump = TypeJump::DOUBLE_JUMP;
		} else {
			parent->controller.stateJump = TypeJump::JUMP_DOWN;
		}
		controller->moveY = direction.y;
		state = JUMP;

	} else {
		//continue in his direction
		//random to know if running or walking
		if (Utils::range(4) != 0) {
			//run!!
			state = RUN;
			controlIA.stop();
			controlIA.start();
			controller->run = true;
		} else {
			//walk
			state = FORWARD;
			controlIA.stop();
			controlIA.start();
		}
	}

	previousplayer = direction;
}

void EnemyBehaviour::running(const Transform& globalMine, const Transform& globalPlayer) {
	//he is running, stop after a time
	if (controlIA.value() > 1700) {
		state = FORWARD;
		parent->controller.run = false;
		controlIA.stop();
		controlIA.start();
	}
	setController(iPoint(previousplayer.x, 0));
}

void EnemyBehaviour::jumping(const Transform & globalMine, const  Transform & globalPlayer) {
	//make the jump in the player direction
	if (parent->controller.stateJump != TypeJump::JUMP) {
		state = SEARCHING;
		//decide inmediatly or thinking?
		if (Utils::range(3) == 0) {
			controlIA.stop();
			controlIA.start();
		}
	}
}

void EnemyBehaviour::setController(iPoint direction) {
	ControlEntity* controller = &parent->controller;
	Transform* entityTransform = this->parent->transform;

	controller->moveX = 0;
	controller->moveY = 0;
	if (direction.x < 0) {
		controller->moveX = -1;
		entityTransform->flip = SDL_FLIP_NONE;
	} else if (direction.x>0) {
		controller->moveX = +1;
		entityTransform->flip = SDL_FLIP_HORIZONTAL;
	}

	if (direction.y != 0) {
		//stop and look below
		controller->moveX = 0;
		controller->moveY = direction.y;
	}

	if (controller->moveY == 0) {
		motion->velocity.x = (controller->run) ? controller->moveX * motion->doubleSpeed : controller->moveX * motion->speed;
	} else {
		motion->velocity.x = 0.0f;
	}
}

iPoint EnemyBehaviour::directionPlayer(const Transform & globalMine, const Transform & globalPlayer) const {
	iPoint result = iPoint();
	result.setToZero();

	if (globalPlayer.position.x < globalMine.position.x) {
		result.x = -1;
	} else {
		result.x = 1;
	}

	int difference = static_cast<int>(globalPlayer.position.y - globalMine.position.y);
	if (difference > 20) {
		result.y = 1;
	} else if (difference < -20) {
		result.y = -1;
	}

	return result;
}
