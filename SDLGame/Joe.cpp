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

Entity * Joe::makeJoe() {
	//prepare the entity for the player
	Entity* result = new Entity();

	JumpComponent* jump = new JumpComponent("jump", 32, 30);
	jump->speed = 200;
	result->addComponent(jump);

	Gravity* gravity = new Gravity("gravity");
	gravity->gravity = 120;
	result->addComponent(gravity);

	MotionComponent* motion = new MotionComponent("motion");
	motion->velocity.x = 0;
	motion->velocity.y = 0;
	result->addComponent(motion);

	makeAnimations(result);

	return result;
}

void Joe::makeAnimations(Entity* entity) {
	ControlEntity* controller = &entity->controller;
	StateMachine<Animation>* animations;

	Animation forward(4);
	Animation idle(1);
	Animation lookingUp(1);
	Animation down(1);
	Animation jump(2);
	Animation fall(1);
	Animation backHit(1);
	Animation frontHit(1);

	down.sizeFrame = {56, 13, 31, 34};
	lookingUp.sizeFrame = {28, 0, 28, 47};
	idle.sizeFrame={0, 0, 28, 47};
	fall.sizeFrame = {80, 152, 28, 49};
	backHit.sizeFrame = {0, 201, 55, 53};
	frontHit.sizeFrame = {55, 206, 46, 48};

	jump.sizeFrame = {0,150,40,51};
	jump.repeat = 1;
	jump.speed = 0.05f;
	jump.flippedOffset = {-10,0};

	forward.sizeFrame={0, 102, 42, 48};
	forward.speed = 0.1f;
	forward.flippedOffset.x = -14;

	//states
	State<Animation>* forwardAnimation = new State<Animation>(forward);
	State<Animation>* idleAnimation = new State<Animation>(idle);
	State<Animation>* downAnimation = new State<Animation>(down);
	State<Animation>* lookingUpAnimation = new State<Animation>(lookingUp);
	State<Animation>* jumpAnimation = new State<Animation>(jump);
	State<Animation>* fallAnimation = new State<Animation>(fall);
	State<Animation>* backHitAnimation = new State<Animation>(backHit);
	State<Animation>* frontkHitAnimation = new State<Animation>(frontHit);

	//condiciones
	Condition* conditionForward = new ConditionComparison<int>(&controller->moveX, 1);
	Condition* conditionBackward = new ConditionComparison<int>(&controller->moveX, -1);
	Condition* conditionIdle1 = new ConditionComparison<int>(&controller->moveX, 0);
	Condition* conditionIdle2 = new ConditionComparison<int>(&controller->moveY, 0);
	Condition* conditionDown = new ConditionComparison<int>(&controller->moveY, 1);
	Condition* conditionLookingUp = new ConditionComparison<int>(&controller->moveY, -1);
	Condition* conditionFall = new ConditionComparison<JumpType>(&controller->stateJump, JumpType::FALL); //TODO
	Condition* conditionJumpDown = new ConditionComparison<JumpType>(&controller->stateJump, JumpType::JUMP_DOWN);
	Condition* conditionJump = new ConditionComparison<JumpType>(&controller->stateJump, JumpType::JUMP);
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
	StateTransition<Animation>* transitionJump = new StateTransition<Animation>(jumpAnimation, conditionJump);
	StateTransition<Animation>* transitionDownToIdle = new StateTransition<Animation>(idleAnimation, conditionIdle2);
	StateTransition<Animation>* transitionForwardToIdle = new StateTransition<Animation>(idleAnimation, conditionIdle1);
	StateTransition<Animation>* transitionJumpDown = new StateTransition<Animation>(fallAnimation, conditionJumpDown);
	StateTransition<Animation>* transitionFall = new StateTransition<Animation>(fallAnimation, conditionFall);
	StateTransition<Animation>* transitionFallToIdle = new StateTransition<Animation>(idleAnimation, conditionFallToIdle);
	

	//add the transitions to the states
	idleAnimation->addTransition(transitionForward);
	idleAnimation->addTransition(transitionDown);
	idleAnimation->addTransition(transitionBackward);
	idleAnimation->addTransition(transitionLookingUp);
	idleAnimation->addTransition(transitionJump);
	idleAnimation->addTransition(transitionJumpDown);
	idleAnimation->addTransition(transitionFall);

	forwardAnimation->addTransition(transitionDown);
	forwardAnimation->addTransition(transitionJump);
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

	jumpAnimation->addTransition(transitionFall);
	//jumpAnimation->addTransition(transitionIdle);
	fallAnimation->addTransition(transitionFallToIdle);


	//add the states;
	animations = new StateMachine<Animation>(idleAnimation);
	animations->addState(forwardAnimation);
	animations->addState(downAnimation);

	//create the component
	AnimationComponent* component=new AnimationComponent("animations", "Joe.png", animations);
	entity->addComponent(component);
}
