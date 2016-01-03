#include "Joe.h"
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

Entity * Joe::makeJoe() {
	//prepare the entity for the player
	Entity* result = new Entity();

	JumpComponent* jump = new JumpComponent("jump");
	jump->speed = 300;
	jump->doubleSpeed = 400;
	result->addComponent(jump);

	Gravity* gravity = new Gravity("gravity");
	gravity->gravity = 600;
	result->addComponent(gravity);

	MotionComponent* motion = new MotionComponent("motion");
	motion->velocity.x = 0;
	motion->velocity.y = 0;
	result->addComponent(motion);
	SDL_Rect coll = {0, 0, 28, 47};
	RectangleCollider* rectangle= new RectangleCollider(fPoint(0, 0), coll, TypeCollider::PLAYER);
	CollisionComponent* collider = new CollisionComponent("collider", rectangle);
	
	//App->collisions->addCollider(rectangle);
	result->addComponent(collider);

	makeAnimations(result);

	return result;
}


void Joe::makeAnimations(Entity* entity) {
	//TODO align images and center collider to image
	ControlEntity* controller = &entity->controller;
	StateMachine<Animation>* animations;

	Animation forward(4);
	Animation idle(1);
	Animation lookingUp(1);
	Animation down(1);
	Animation startJump(1);
	Animation jump(1);
	Animation doubleJump(8);
	Animation fall(1);
	/*Animation backHit(1);
	Animation frontHit(1);*/

	down.sizeFrame = {128, 0, 64, 64};
	down.offset.x = -18;
	lookingUp.sizeFrame = {64, 0, 64, 64};
	lookingUp.offset.x = -18;
	idle.sizeFrame={0, 0, 64, 64};
	idle.offset.x = -18;
	fall.sizeFrame = {320, 0, 64, 64};
	fall.offset.x = -18;
	/*backHit.sizeFrame = {0, 201, 55, 53};
	frontHit.sizeFrame = {55, 206, 46, 48};*/
	startJump.sizeFrame = {192,0,64,64};
	startJump.offset.x = -18;

	jump.sizeFrame = {256,0,64,64};
	//jump.flippedOffset = {-10,0};
	jump.offset.x = -18;

	doubleJump.sizeFrame = {0,320,64,64};
	doubleJump.speed = 0.35f;
	doubleJump.offset.x = -18;
	doubleJump.offset.y = 5;

	forward.sizeFrame={128, 128, 64, 64};
	forward.offset.x = -18;
	forward.speed = 0.1f;
	//forward.flippedOffset.x = -7;

	//states
	State<Animation>* forwardAnimation = new State<Animation>(forward);
	State<Animation>* idleAnimation = new State<Animation>(idle);
	State<Animation>* downAnimation = new State<Animation>(down);
	State<Animation>* lookingUpAnimation = new State<Animation>(lookingUp);
	State<Animation>* startJumpAnimation = new State<Animation>(startJump);
	State<Animation>* jumpAnimation = new State<Animation>(jump);
	State<Animation>* doubleJumpAnimation = new State<Animation>(doubleJump);
	State<Animation>* fallAnimation = new State<Animation>(fall);
	//State<Animation>* backHitAnimation = new State<Animation>(backHit);
	//State<Animation>* frontHitAnimation = new State<Animation>(frontHit);

	//condiciones
	Condition* conditionForward = new ConditionComparison<int>(&controller->moveX, 1);
	Condition* conditionBackward = new ConditionComparison<int>(&controller->moveX, -1);
	Condition* conditionIdle1 = new ConditionComparison<int>(&controller->moveX, 0);
	Condition* conditionIdle2 = new ConditionComparison<int>(&controller->moveY, 0);
	Condition* conditionDown = new ConditionComparison<int>(&controller->moveY, 1);
	Condition* conditionLookingUp = new ConditionComparison<int>(&controller->moveY, -1);
	Condition* conditionFall = new ConditionCallback([entity, controller](){
		bool result = false;
		result = controller->stateJump == JumpType::FALL;
		IComponent* component = entity->getComponent("motion");
		if (component != nullptr && result ) {
			MotionComponent* motion = static_cast<MotionComponent*>(component);
			result = (motion->velocity.y > 35.0*SCREEN_SIZE);
		}
		return result;
	});

	Condition* conditionJumpDown = new ConditionComparison<JumpType>(&controller->stateJump, JumpType::JUMP_DOWN);

	
	Condition* conditionStartJump1 = new ConditionComparison<JumpType>(&controller->stateJump, JumpType::JUMP);
	Condition* conditionStartJump2 = new ConditionComparison<JumpType>(&controller->stateJump, JumpType::DOUBLE_JUMP);

	Condition* conditionJump = new ConditionComparison<JumpType>(&controller->stateJump, JumpType::JUMP);
	Condition* conditionJump2 = new TimerCondition(150);

	Condition* conditionDoubleJump = new ConditionComparison<JumpType>(&controller->stateJump, JumpType::DOUBLE_JUMP);
	
	Condition* conditionFallToIdle = new ConditionCallback([entity]() {
		bool result = false;
		IComponent* component = entity->getComponent("gravity");
		if (component != nullptr) {
			Gravity* gravity = static_cast<Gravity*>(component);
			result = !gravity->isFalling();
		}
		return result;
	});

	//transitions
	StateTransition<Animation>* transitionForward = new StateTransition<Animation>(forwardAnimation, conditionForward);
	StateTransition<Animation>* transitionBackward = new StateTransition<Animation>(forwardAnimation, conditionBackward);
	StateTransition<Animation>* transitionIdle1 = new StateTransition<Animation>(idleAnimation, conditionIdle1);
	StateTransition<Animation>* transitionIdle2 = new StateTransition<Animation>(idleAnimation, conditionIdle2);
	StateTransition<Animation>* transitionDown = new StateTransition<Animation>(downAnimation, conditionDown);
	StateTransition<Animation>* transitionLookingUp = new StateTransition<Animation>(lookingUpAnimation, conditionLookingUp);
	StateTransition<Animation>* transitionDownToIdle = new StateTransition<Animation>(idleAnimation, conditionIdle2);
	StateTransition<Animation>* transitionForwardToIdle = new StateTransition<Animation>(idleAnimation, conditionIdle1);
	StateTransition<Animation>* transitionJumpDown = new StateTransition<Animation>(fallAnimation, conditionJumpDown);
	StateTransition<Animation>* transitionFall = new StateTransition<Animation>(fallAnimation, conditionFall);
	StateTransition<Animation>* transitionFallToIdle = new StateTransition<Animation>(idleAnimation, conditionFallToIdle);
	//jump
	StateTransition<Animation>* transitionStartJump1 = new StateTransition<Animation>(startJumpAnimation, conditionStartJump1);
	StateTransition<Animation>* transitionStartJump2 = new StateTransition<Animation>(startJumpAnimation, conditionStartJump2);
	StateTransition<Animation>* transitionJump = new StateTransition<Animation>(jumpAnimation, conditionJump);
								transitionJump->addCondition(conditionJump2);
								transitionJump->addCondition(conditionStartJump1); //esta transition solo pasa con el salto normal

	StateTransition<Animation>* transitionDoubleJump = new StateTransition<Animation>(doubleJumpAnimation, conditionDoubleJump);
								transitionDoubleJump->addCondition(conditionJump2);
								transitionDoubleJump->addCondition(conditionStartJump2); //esta transition solo pasa con el salto doble
	/*StateTransition<Animation>* transitionJump = new StateTransition<Animation>(jumpAnimation, conditionJump);
	StateTransition<Animation>* transitionJump = new StateTransition<Animation>(jumpAnimation, conditionJump);*/

	//add the transitions to the states
	idleAnimation->addTransition(transitionForward);
	idleAnimation->addTransition(transitionDown);
	idleAnimation->addTransition(transitionBackward);
	idleAnimation->addTransition(transitionLookingUp);
	idleAnimation->addTransition(transitionStartJump1);
	idleAnimation->addTransition(transitionStartJump2);
	idleAnimation->addTransition(transitionJumpDown);
	idleAnimation->addTransition(transitionFall);

	forwardAnimation->addTransition(transitionDown);
	forwardAnimation->addTransition(transitionStartJump1);
	forwardAnimation->addTransition(transitionStartJump2);
	forwardAnimation->addTransition(transitionFall);
	forwardAnimation->addTransition(transitionIdle1);

	downAnimation->addTransition(transitionDownToIdle);
	downAnimation->addTransition(transitionIdle2);
	downAnimation->addTransition(transitionFall);
	downAnimation->addTransition(transitionJumpDown);
	downAnimation->addTransition(transitionFall);

	lookingUpAnimation->addTransition(transitionIdle2);
	lookingUpAnimation->addTransition(transitionForward);
	lookingUpAnimation->addTransition(transitionBackward);
	lookingUpAnimation->addTransition(transitionFall);
	lookingUpAnimation->addTransition(transitionStartJump2);

	startJumpAnimation->addTransition(transitionJump);//solo desde start animation pasa a jump o double jump
	startJumpAnimation->addTransition(transitionDoubleJump);

	jumpAnimation->addTransition(transitionFall);
	doubleJumpAnimation->addTransition(transitionFall);
	fallAnimation->addTransition(transitionFallToIdle);


	//add the states;
	animations = new StateMachine<Animation>(idleAnimation);
	animations->addState(forwardAnimation);
	animations->addState(lookingUpAnimation);
	animations->addState(downAnimation);
	animations->addState(startJumpAnimation);
	animations->addState(jumpAnimation);
	animations->addState(doubleJumpAnimation);
	animations->addState(fallAnimation);

	//create the component
	AnimationComponent* component=new AnimationComponent("animations", "Joe.png", animations);
	entity->addComponent(component);
}
