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
	gravity->gravity = 550;
	gravity->maxVelocity = 500;
	result->addComponent(gravity);

	RectangleCollider* rectangle= new RectangleCollider(fPoint(0,0), fPoint(28,47), TypeCollider::PLAYER);
	CollisionComponent* collider = new CollisionComponent("collider", rectangle);
	result->addComponent(collider);

	MotionComponent* motion = new MotionComponent("motion");
	motion->velocity.x = 0;
	motion->velocity.y = 0;
	motion->speed = 50;
	result->addComponent(motion);
	
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
	down.offset = {-18,-17};
	lookingUp.sizeFrame = {64, 0, 64, 64};
	lookingUp.offset = {-18,-17};
	idle.sizeFrame={0, 0, 64, 64};
	idle.offset = {-18,-17};
	fall.sizeFrame = {320, 0, 64, 64};
	fall.offset = {-18,-17};
	/*backHit.sizeFrame = {0, 201, 55, 53};
	frontHit.sizeFrame = {55, 206, 46, 48};*/
	startJump.sizeFrame = {192,0,64,64};
	startJump.offset = {-18,-17};

	jump.sizeFrame = {256,0,64,64};
	jump.offset = {-18, -17};

	doubleJump.sizeFrame = {0,320,64,64};
	doubleJump.speed = 0.35f;
	doubleJump.offset = {-18, -12};

	forward.sizeFrame={128, 128, 64, 64};
	forward.offset = {-18,-17};
	forward.speed = 0.1f;

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
	ConditionComparison<int> conditionForward = ConditionComparison<int>(&controller->moveX, 1);
	ConditionComparison<int> conditionBackward =  ConditionComparison<int>(&controller->moveX, -1);
	ConditionComparison<int> conditionIdle1 =  ConditionComparison<int>(&controller->moveX, 0);
	ConditionComparison<int> conditionIdle2 = ConditionComparison<int>(&controller->moveY, 0);
	ConditionComparison<int> conditionDown = ConditionComparison<int>(&controller->moveY, 1);
	ConditionComparison<int> conditionLookingUp = ConditionComparison<int>(&controller->moveY, -1);
	ConditionCallback conditionFall = ConditionCallback([entity, controller](){
		bool result = false;
		result = controller->stateJump == JumpType::FALL;
		IComponent* component = entity->getComponent("motion");
		if (component != nullptr && result ) {
			MotionComponent* motion = static_cast<MotionComponent*>(component);
			result = (motion->velocity.y > 35.0*SCREEN_SIZE);
		}
		return result;
	});
	ConditionComparison<JumpType> conditionJumpDown = ConditionComparison<JumpType>(&controller->stateJump, JumpType::JUMP_DOWN);

	ConditionComparison<JumpType> conditionStartJump1 = ConditionComparison<JumpType>(&controller->stateJump, JumpType::JUMP);
	ConditionComparison<JumpType> conditionStartJump2 = ConditionComparison<JumpType>(&controller->stateJump, JumpType::DOUBLE_JUMP);
	ConditionComparison<JumpType> conditionJump = ConditionComparison<JumpType>(&controller->stateJump, JumpType::JUMP);
	TimerCondition conditionJump2 = TimerCondition(150);
	ConditionComparison<JumpType> conditionDoubleJump = ConditionComparison<JumpType>(&controller->stateJump, JumpType::DOUBLE_JUMP);
	ConditionCallback conditionFallToIdle = ConditionCallback([entity]() {
		bool result = false;
		IComponent* component = entity->getComponent("gravity");
		if (component != nullptr) {
			Gravity* gravity = static_cast<Gravity*>(component);
			result = !gravity->isFalling();
		}
		return result;
	});

	//transitions
	StateTransition<Animation> transitionForward = StateTransition<Animation>(forwardAnimation, &conditionForward);
	StateTransition<Animation> transitionBackward =  StateTransition<Animation>(forwardAnimation, &conditionBackward);
	StateTransition<Animation> transitionIdle1 = StateTransition<Animation>(idleAnimation, &conditionIdle1);
	StateTransition<Animation> transitionIdle2 = StateTransition<Animation>(idleAnimation, &conditionIdle2);
	StateTransition<Animation> transitionDown = StateTransition<Animation>(downAnimation, &conditionDown);
	StateTransition<Animation> transitionLookingUp = StateTransition<Animation>(lookingUpAnimation, &conditionLookingUp);
	StateTransition<Animation> transitionDownToIdle = StateTransition<Animation>(idleAnimation, &conditionIdle2);
	StateTransition<Animation> transitionJumpDown = StateTransition<Animation>(fallAnimation, &conditionJumpDown);
	StateTransition<Animation> transitionFall = StateTransition<Animation>(fallAnimation, &conditionFall);
	StateTransition<Animation> transitionFallToIdle = StateTransition<Animation>(idleAnimation, &conditionFallToIdle);
	//jump
	StateTransition<Animation> transitionStartJump1 = StateTransition<Animation>(startJumpAnimation, &conditionStartJump1);
	StateTransition<Animation> transitionStartJump2 = StateTransition<Animation>(startJumpAnimation, &conditionStartJump2);
	StateTransition<Animation> transitionJump = StateTransition<Animation>(jumpAnimation, &conditionJump);
								transitionJump.addCondition(&conditionJump2);
								transitionJump.addCondition(&conditionStartJump1); //esta transition solo pasa con el salto normal

	StateTransition<Animation> transitionDoubleJump = StateTransition<Animation>(doubleJumpAnimation, &conditionDoubleJump);
								transitionDoubleJump.addCondition(&conditionJump2);
								transitionDoubleJump.addCondition(&conditionStartJump2); //esta transition solo pasa con el salto doble
								

	//add the transitions to the states
	idleAnimation->addTransition(&transitionForward);
	idleAnimation->addTransition(&transitionDown);
	idleAnimation->addTransition(&transitionBackward);
	idleAnimation->addTransition(&transitionLookingUp);
	idleAnimation->addTransition(&transitionStartJump1);
	idleAnimation->addTransition(&transitionStartJump2);
	idleAnimation->addTransition(&transitionJumpDown);
	idleAnimation->addTransition(&transitionFall);
	
	forwardAnimation->addTransition(&transitionDown);
	forwardAnimation->addTransition(&transitionStartJump1);
	forwardAnimation->addTransition(&transitionStartJump2);
	forwardAnimation->addTransition(&transitionFall);
	forwardAnimation->addTransition(&transitionIdle1);

	downAnimation->addTransition(&transitionDownToIdle);
	downAnimation->addTransition(&transitionIdle2);
	downAnimation->addTransition(&transitionFall);
	downAnimation->addTransition(&transitionJumpDown);
	
	lookingUpAnimation->addTransition(&transitionIdle2);
	lookingUpAnimation->addTransition(&transitionForward);
	lookingUpAnimation->addTransition(&transitionBackward);
	lookingUpAnimation->addTransition(&transitionFall);
	lookingUpAnimation->addTransition(&transitionStartJump2);

	startJumpAnimation->addTransition(&transitionJump);//solo desde start animation pasa a jump o double jump
	startJumpAnimation->addTransition(&transitionDoubleJump);

	jumpAnimation->addTransition(&transitionFall);
	doubleJumpAnimation->addTransition(&transitionFall);
	fallAnimation->addTransition(&transitionFallToIdle);


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
