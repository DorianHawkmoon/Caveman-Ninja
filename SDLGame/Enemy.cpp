#include "Enemy.h"
#include "Entity.h"
#include "Animation.h"
#include "StateMachine.h"
#include "State.h"
#include "ConditionCallback.h"
#include "MotionComponent.h"
#include "SpriteComponent.h"
#include "AnimationComponent.h" 
#include "Application.h"
#include "ModuleInput.h"
#include "SDL\SDL.h"
#include "StateMachine.h"
#include "JumpComponent.h"
#include "ConditionComparison.h"
#include "Gravity.h"
#include "TimerCondition.h"
#include "ModuleCollision.h"
#include "RectangleCollider.h"
#include "CircleCollider.h"
#include "CollisionComponent.h"
#include "SDL\SDL_rect.h"
#include "IAComponent.h"
#include "Utils.h"
#include "ModulePlayer.h"

Entity * Enemy::makeEnemy() {
	//prepare the entity for the player
	Entity* result = new Entity();

	JumpComponent* jump = new JumpComponent("jump");
	jump->speed = 250;
	jump->doubleSpeed = 350;
	result->addComponent(jump);

	Gravity* gravity = new Gravity("gravity");
	gravity->gravity = 550;
	gravity->maxVelocity = 500;
	result->addComponent(gravity);

	RectangleCollider* rectangle = new RectangleCollider(fPoint(0, 0), iPoint(25, 46), 0, TypeCollider::ENEMY);
	CollisionComponent* collider = new CollisionComponent("collider", rectangle);
	result->addComponent(collider);

	MotionComponent* motion = new MotionComponent("motion");
	motion->velocity.x = 0;
	motion->velocity.y = 0;
	motion->speed = 25;
	result->addComponent(motion);

	makeAnimations(result);
	makeBehaviour(result);

	return result;
}


void Enemy::makeAnimations(Entity* entity) {
	//TODO align images and center collider to image
	ControlEntity* controller = &entity->controller;
	StateMachine<Animation>* animations;

	Animation forward(4);
	Animation idle(1);
	Animation lookingUp(1);
	Animation down(1);
	Animation jump(1);
	Animation startFall(1);
	Animation fall(1);
	Animation endFall(1);
	Animation run(4);

	down.sizeFrame = {128, 384, 128, 128};
	down.offset = {-51,-82};

	lookingUp.sizeFrame = {0, 384, 128, 128};
	lookingUp.offset = {-51,-82};

	idle.sizeFrame = {0, 0, 128, 128};
	idle.offset = {-51,-82};
	
	fall.sizeFrame = {768, 0, 128, 128};
	fall.offset = {-45,-82};
	fall.flippedOffset.x = -12;

	jump.sizeFrame = {512,0,128,128};
	jump.offset = {-46,-65};
	jump.flippedOffset.x = -6;

	

	startFall.sizeFrame = {640,0,128,128};
	startFall.offset = {-48,-82};
	startFall.flippedOffset.x = -6;

	forward.sizeFrame = {0,0, 128, 128};
	forward.offset = {-51,-82};
	forward.speed = 0.1f;

	endFall.sizeFrame = {896, 0, 128, 128};
	endFall.offset = {-45,-82};
	endFall.flippedOffset.x = -10;

	run.sizeFrame = {0,128, 128, 128};
	run.offset = {-47, -82}; 
	run.flippedOffset.x = -9;
	run.speed = 0.17f;

	//states
	State<Animation>* forwardAnimation = new State<Animation>(forward);
	State<Animation>* idleAnimation = new State<Animation>(idle);
	State<Animation>* downAnimation = new State<Animation>(down);
	State<Animation>* lookingUpAnimation = new State<Animation>(lookingUp);
	State<Animation>* jumpAnimation = new State<Animation>(jump);
	State<Animation>* startFallAnimation = new State<Animation>(startFall);
	State<Animation>* fallAnimation = new State<Animation>(fall);
	State<Animation>* endFallAnimation = new State<Animation>(endFall);
	State<Animation>* runAnimation = new State<Animation>(run);

	//conditions
	ConditionComparison<int> conditionForward = ConditionComparison<int>(&controller->moveX, 1);
	ConditionComparison<int> conditionBackward = ConditionComparison<int>(&controller->moveX, -1);
	ConditionComparison<int> conditionIdle1 = ConditionComparison<int>(&controller->moveX, 0);
	ConditionComparison<int> conditionIdle2 = ConditionComparison<int>(&controller->moveY, 0);
	ConditionComparison<int> conditionDown = ConditionComparison<int>(&controller->moveY, 1);
	ConditionComparison<int> conditionLookingUp = ConditionComparison<int>(&controller->moveY, -1);
	ConditionComparison<bool> conditionRun = ConditionComparison<bool>(&controller->run, true);
	ConditionComparison<bool> conditionNotRun = ConditionComparison<bool>(&controller->run, false);

	//transitions
	StateTransition<Animation> transitionForward = StateTransition<Animation>(forwardAnimation, &conditionForward);
	StateTransition<Animation> transitionBackward = StateTransition<Animation>(forwardAnimation, &conditionBackward);
	StateTransition<Animation> transitionIdle1 = StateTransition<Animation>(idleAnimation, &conditionIdle1);
	StateTransition<Animation> transitionIdle2 = StateTransition<Animation>(idleAnimation, &conditionIdle2);
	StateTransition<Animation> transitionDown = StateTransition<Animation>(downAnimation, &conditionDown);
	StateTransition<Animation> transitionLookingUp = StateTransition<Animation>(lookingUpAnimation, &conditionLookingUp);
	StateTransition<Animation> transitionRun = StateTransition<Animation>(runAnimation, &conditionRun);
	StateTransition<Animation> transitionRunIddle = StateTransition<Animation>(runAnimation, &conditionNotRun);

	//add the transitions to the states
	idleAnimation->addTransition(&transitionForward);
	idleAnimation->addTransition(&transitionBackward);
	idleAnimation->addTransition(&transitionDown);
	idleAnimation->addTransition(&transitionLookingUp);
	idleAnimation->addTransition(&transitionRun);

	forwardAnimation->addTransition(&transitionRun);
	forwardAnimation->addTransition(&transitionIdle1);

	downAnimation->addTransition(&transitionIdle2);

	lookingUpAnimation->addTransition(&transitionIdle2);
	lookingUpAnimation->addTransition(&transitionForward);
	lookingUpAnimation->addTransition(&transitionBackward);

	runAnimation->addTransition(&transitionRunIddle);

	//add the states;
	animations = new StateMachine<Animation>(idleAnimation);
	animations->addState(forwardAnimation);
	animations->addState(downAnimation);
	animations->addState(lookingUpAnimation);
	animations->addState(jumpAnimation);
	animations->addState(startFallAnimation);
	animations->addState(fallAnimation);
	animations->addState(endFallAnimation);
	animations->addState(runAnimation);

	//create the component
	AnimationComponent* component = new AnimationComponent("animations", "troglodita.png", animations);
	entity->addComponent(component);
}

void Enemy::makeBehaviour(Entity * entity) {
	IAComponent* IA = new IAComponent("IA");
	entity->addComponent(IA);

	IA->ticks = 1000;

	IA->functionUpdate = [](Entity* entity, bool ticked) {
		ControlEntity* controller = &entity->controller;
		Transform* trans = entity->transform;
		Transform globalEnemy = trans->getGlobalTransform();

		//check collision with player
		//fPoint& position, iPoint& rectangle, float rotation, TypeCollider type
		//ancho negativo o positivo, rectangulo, 0, none
		/* fPoint position = {0,0};
		iPoint rectangle = {0,0};
		RectangleCollider colliderCheck = RectangleCollider(position, rectangle, 0, TypeCollider::ENEMY);
		const Collider* colliderPlayer = static_cast<CollisionComponent*>(App->player->player->getComponent("collider"))->getCollider();

		if (colliderPlayer->checkCollision(&colliderCheck)) {
		//inmediatly react attacking

		}*/

		if (ticked) {
			MotionComponent* motion = static_cast<MotionComponent*>(entity->getComponent("motion"));

			controller->moveX = 0;
			controller->moveY = 0;
			controller->attack = 0;

			Transform player = App->player->player->transform->getGlobalTransform();
			float distance = abs(player.position.x - globalEnemy.position.x);
			if (player.position.x < globalEnemy.position.x) {
				controller->moveX -= 1;
				trans->flip = SDL_FLIP_NONE;
				if (distance > 150) {
					controller->run = true;
				}
			}

			if (player.position.x > globalEnemy.position.x) {
				controller->moveX += 1;
				trans->flip = SDL_FLIP_HORIZONTAL;
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
				motion->velocity.x = (controller->run) ?
					(controller->moveX + 0.7 * sgn(controller->moveX)) * motion->speed
					: controller->moveX * motion->speed;
			} else {
				motion->velocity.x = 0.0f;
			}
		}
	};

}