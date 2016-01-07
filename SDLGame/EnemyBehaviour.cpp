#include "EnemyBehaviour.h"
#include "Entity.h"
#include "MotionComponent.h"
#include "RectangleCollider.h"
#include "CollisionComponent.h"

#include "Application.h"
#include "ModulePlayer.h"
#include "Utils.h"
#include "AnimationComponent.h"
#include "Animation.h"
#include "LifeComponent.h"

IComponent* EnemyBehaviour::clone() {
	EnemyBehaviour* result = new EnemyBehaviour(getID());
	return result;
}

update_status EnemyBehaviour::update() {
	Transform* entityTransform = this->parent->transform;
	Transform globalPlayer = App->player->player->transform->getGlobalTransform();
	Transform globalMine = Transform(entityTransform->getGlobalTransform());
	
	int attack = parent->controller.attack;
	//check if player is near to attack him
	if (attack == 1 || attack == 2) {
		//is attacking, check if finished the animation
		attacking(globalMine,globalPlayer);
	} else if (attack == 0) {
		//check if player is near to attack him
		checkCollisions(globalMine, globalPlayer);

	}else if(attack==3){
		//motion run
		runningAway();

	} 

	//put the enemy thinking what to do
	if (ticked && parent->controller.attack==0) {
		updateMotion(globalMine, globalPlayer);
	}

	return UPDATE_CONTINUE;
}

update_status EnemyBehaviour::postUpdate() {
	return UPDATE_CONTINUE;
}

void EnemyBehaviour::attacking(Transform & globalMine, Transform& globalPlayer) {
	int attack = parent->controller.attack;
	AnimationComponent* animations = static_cast<AnimationComponent*>(parent->getComponent("animations"));
	const Animation* actualAnimation = animations->getActualAnimation();
	switch (attack) {
		case 1: //atacando, buscamos al jugador
			if (actualAnimation->isFinished()) {
				LOG("Enemy attacking");
				//la animación ha terminado, está en el último frame donde se hace efectivo el golpe
				//comprobamos colisión a ver si de verdad hemos dado al jugador o se nos ha escapado
				//check collision with player
				const Collider* myCollider = (static_cast<CollisionComponent*>(parent->getComponent("collider")))->getCollider();
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
					App->player->life->modifyActualLife(-10);
					LOG("Enemy damage!!");
					//colliderPlayer->notify(myCollider);
					//parent->getComponent("collider")->onCollisionEnter()
				}


				//da igual si le hemos dado, escapamos! (segunda fase de atacar)
				parent->controller.attack = 2;
			}
			break;
		case 2: //precalentamiento de huida
			//si ha terminado, corremos!!
			if (actualAnimation->isFinished()) {
				parent->controller.attack = 3;
				//direction of running
				Transform* entityTransform = this->parent->transform;
				MotionComponent* motion = static_cast<MotionComponent*>(parent->getComponent("motion"));
				float direction = 1;
				if (globalMine.position.x > globalPlayer.position.x) {//correr a derecha
					entityTransform->flip = SDL_FLIP_HORIZONTAL;
				} else {
					direction = -1;
					entityTransform->flip = SDL_FLIP_NONE;
				}
				motion->velocity.x += motion->doubleSpeed * direction * 1.2;
			}
			break;
	}
}

void EnemyBehaviour::checkCollisions(Transform& globalMine, Transform& globalPlayer) {
	Transform* entityTransform = this->parent->transform;

	//check collision with player
	const Collider* myCollider = (static_cast<CollisionComponent*>(parent->getComponent("collider")))->getCollider();
	const Collider* colliderPlayer = static_cast<CollisionComponent*>(App->player->player->getComponent("collider"))->getCollider();

	//same position, but in front of the enemy (width)
	fPoint position = globalMine.position;
	//NONE -> looking left
	iPoint size = myCollider->getSize();
	position.x += (entityTransform->flip == SDL_FLIP_NONE) ? -size.x : size.x;

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

void EnemyBehaviour::runningAway() {
	//check if dead to set corresponding motion

}
