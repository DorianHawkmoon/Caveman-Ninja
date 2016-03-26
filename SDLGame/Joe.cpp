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
#include "GravityComponent.h"
#include "TimerCondition.h"
#include "ModuleCollision.h"
#include "RectangleCollider.h"
#include "CircleCollider.h"
#include "CollisionComponent.h"
#include "SDL\SDL_rect.h"
#include "Animation.h"
#include "LifeComponent.h"
#include "WallCollisionComponent.h"
#include "WeaponComponent.h"
#include "DamageComponent.h"
#include "PlayerHittedComponent.h"
#include "ModulePlayer.h"
#include "PickupItemComponent.h"
#include "ScoreComponent.h"
#include "GravityPlayerComponent.h"

Entity * Joe::makeJoe() {
	//prepare the entity for the player
	Entity* result = new Entity();

	

	PlayerHittedComponent* hitted = new PlayerHittedComponent("hitted");
	result->addComponent(hitted);

	DamageComponent* damage = new DamageComponent("damage",1,1);
	result->addComponent(damage);

	JumpComponent* jump = new JumpComponent("jump");
	jump->speed = 300;
	jump->doubleSpeed = 400;
	result->addComponent(jump);

	WallCollisionComponent* walls = new WallCollisionComponent("walls");
	result->addComponent(walls);

	RectangleCollider* rectangle= new RectangleCollider(fPoint(4,4), iPoint(20,38), 0, TypeCollider::PLAYER);
	CollisionComponent* collider = new CollisionComponent("collider", rectangle);
	result->addComponent(collider);

	LifeComponent* life = new LifeComponent("life",18);
	result->addComponent(life);

	PickupItemComponent* pickup = new PickupItemComponent("pickup");
	result->addComponent(pickup);

	ScoreComponent* score = new ScoreComponent("score");
	result->addComponent(score);
	
	WeaponComponent* weapon = new WeaponComponent("weapon", 2, 250);
	result->addComponent(weapon);

	CircleCollider* circleGravity = new CircleCollider(fPoint(14, 41), 1, TypeCollider::GRAVITY);
	GravityComponent* gravity = new GravityPlayerComponent("gravity", circleGravity);
	gravity->gravity = 550;
	gravity->maxVelocity = 500;
	result->addComponent(gravity);

	MotionComponent* motion = new MotionComponent("motion");
	motion->velocity.x = 0;
	motion->velocity.y = 0;
	motion->speed = 50;
	motion->doubleSpeed = 50;
	result->addComponent(motion);

	makeAnimations(result);

	return result;
}


void Joe::makeAnimations(Entity* entity) {
	ControlEntity* controller = &entity->controller;
	StateMachine<Animation>* animations;

	
	Animation idle(1);
	idle.sizeFrame = { 0, 0, 64, 64 };
	idle.offset = {-18,-17};
	State<Animation>* idleAnimation = new State<Animation>(idle);
	animations = new StateMachine<Animation>(idleAnimation);

	// ---------------------------------------------

	Animation forward(4);
	forward.sizeFrame = {128, 128, 64, 64};
	forward.offset = {-18,-17};
	forward.speed = 0.1f;
	State<Animation>* forwardAnimation = new State<Animation>(forward);
	animations->addState(forwardAnimation);

	ConditionComparison<int> conditionForward = ConditionComparison<int>(&controller->moveX, 1);
	ConditionComparison<int> conditionBackward = ConditionComparison<int>(&controller->moveX, -1);
	ConditionComparison<int> conditionIdle1 = ConditionComparison<int>(&controller->moveX, 0);
	StateTransition<Animation> transitionForward = StateTransition<Animation>(forwardAnimation, &conditionForward);
	StateTransition<Animation> transitionBackward = StateTransition<Animation>(forwardAnimation, &conditionBackward);
	StateTransition<Animation> transitionIdle1 = StateTransition<Animation>(idleAnimation, &conditionIdle1);

	forwardAnimation->addTransition(&transitionIdle1);
	idleAnimation->addTransition(&transitionForward);
	idleAnimation->addTransition(&transitionBackward);
	

	// ---------------------------------------------

	Animation lookingUp(1);
	lookingUp.sizeFrame = {64, 0, 64, 64};
	lookingUp.offset = {-18,-17};
	State<Animation>* lookingUpAnimation = new State<Animation>(lookingUp);
	animations->addState(lookingUpAnimation);

	ConditionComparison<int> conditionIdle2 = ConditionComparison<int>(&controller->moveY, 0);
	ConditionComparison<int> conditionLookingUp = ConditionComparison<int>(&controller->moveY, -1);
	StateTransition<Animation> transitionIdle2 = StateTransition<Animation>(idleAnimation, &conditionIdle2);
	StateTransition<Animation> transitionLookingUp = StateTransition<Animation>(lookingUpAnimation, &conditionLookingUp);

	idleAnimation->addTransition(&transitionLookingUp);
	lookingUpAnimation->addTransition(&transitionIdle2);
	lookingUpAnimation->addTransition(&transitionForward);
	lookingUpAnimation->addTransition(&transitionBackward);

	// ---------------------------------------------

	Animation down(1);
	down.sizeFrame = {128, 0, 64, 64};
	down.offset = {-18,-17};
	State<Animation>* downAnimation = new State<Animation>(down);
	animations->addState(downAnimation);

	ConditionComparison<int> conditionDown = ConditionComparison<int>(&controller->moveY, 1);
	StateTransition<Animation> transitionDown = StateTransition<Animation>(downAnimation, &conditionDown);

	idleAnimation->addTransition(&transitionDown);
	forwardAnimation->addTransition(&transitionDown);
	downAnimation->addTransition(&transitionIdle2);

	// ---------------------------------------------

	Animation startJump(1);
	startJump.sizeFrame = {192,0,64,64};
	startJump.offset = {-18,-17};
	State<Animation>* startJumpAnimation = new State<Animation>(startJump);
	animations->addState(startJumpAnimation);

	ConditionComparison<TypeJump> conditionStartJump1 = ConditionComparison<TypeJump>(&controller->stateJump, TypeJump::JUMP);
	ConditionComparison<TypeJump> conditionStartJump2 = ConditionComparison<TypeJump>(&controller->stateJump, TypeJump::DOUBLE_JUMP);
	ConditionComparison<TypeJump> conditionEndJump = ConditionComparison<TypeJump>(&controller->stateJump, TypeJump::NONE);
	StateTransition<Animation> transitionEndJump = StateTransition<Animation>(idleAnimation, &conditionEndJump);
	StateTransition<Animation> transitionStartJump1 = StateTransition<Animation>(startJumpAnimation, &conditionStartJump1);
	StateTransition<Animation> transitionStartJump2 = StateTransition<Animation>(startJumpAnimation, &conditionStartJump2);

	idleAnimation->addTransition(&transitionStartJump1);
	idleAnimation->addTransition(&transitionStartJump2);
	forwardAnimation->addTransition(&transitionStartJump1);
	forwardAnimation->addTransition(&transitionStartJump2);
	lookingUpAnimation->addTransition(&transitionStartJump2);
	startJumpAnimation->addTransition(&transitionEndJump);

	// ---------------------------------------------

	Animation jump(1);
	jump.sizeFrame = {256,0,64,64};
	jump.offset = {-18, -17};
	State<Animation>* jumpAnimation = new State<Animation>(jump);
	animations->addState(jumpAnimation);

	TimerCondition conditionJump2 = TimerCondition(150);
	ConditionComparison<TypeJump> conditionJump = ConditionComparison<TypeJump>(&controller->stateJump, TypeJump::JUMP);
	StateTransition<Animation> transitionJump = StateTransition<Animation>(jumpAnimation, &conditionJump);
					transitionJump.addCondition(&conditionJump2);
					transitionJump.addCondition(&conditionStartJump1);

	startJumpAnimation->addTransition(&transitionJump);
	jumpAnimation->addTransition(&transitionEndJump);

	// ---------------------------------------------

	Animation doubleJump(8);
	doubleJump.sizeFrame = {0,192,64,64};
	doubleJump.speed = 0.35f;
	doubleJump.offset = {-18, -12};
	State<Animation>* doubleJumpAnimation = new State<Animation>(doubleJump);
	animations->addState(doubleJumpAnimation);

	ConditionComparison<TypeJump> conditionDoubleJump = ConditionComparison<TypeJump>(&controller->stateJump, TypeJump::DOUBLE_JUMP);
	StateTransition<Animation> transitionDoubleJump = StateTransition<Animation>(doubleJumpAnimation, &conditionDoubleJump);
					transitionDoubleJump.addCondition(&conditionJump2);
					transitionDoubleJump.addCondition(&conditionStartJump2);

	startJumpAnimation->addTransition(&transitionDoubleJump);
	doubleJumpAnimation->addTransition(&transitionEndJump);

	// ---------------------------------------------

	Animation fall(1);
	fall.sizeFrame = {320, 0, 64, 64};
	fall.offset = {-18,-17};
	State<Animation>* fallAnimation = new State<Animation>(fall);
	animations->addState(fallAnimation);

	ConditionComparison<TypeJump> conditionFall = ConditionComparison<TypeJump>(&controller->stateJump, TypeJump::FALL);
	ConditionCallback conditionFallToIdle = ConditionCallback([controller]() {
		bool result = false;
		result = (controller->stateJump != TypeJump::FALL && controller->stateJump != TypeJump::JUMP_DOWN);
		return result;
	});
	TimerCondition conditionFallTimer = TimerCondition(300);
	StateTransition<Animation> transitionFall = StateTransition<Animation>(fallAnimation, &conditionFall);
	transitionFall.addCondition(&conditionFallTimer);
	StateTransition<Animation> transitionFallToIdle = StateTransition<Animation>(idleAnimation, &conditionFallToIdle);

	fallAnimation->addTransition(&transitionFallToIdle);
	idleAnimation->addTransition(&transitionFall);
	forwardAnimation->addTransition(&transitionFall);
	downAnimation->addTransition(&transitionFall);
	lookingUpAnimation->addTransition(&transitionFall);
	jumpAnimation->addTransition(&transitionFall);
	doubleJumpAnimation->addTransition(&transitionFall);

	// ---------------------------------------------

	Animation backHit(1);
	backHit.sizeFrame = {0, 128, 64, 64};
	backHit.offset = {-12,-17};
	backHit.flippedOffset.x = -13;
	State<Animation>* backHitAnimation = new State<Animation>(backHit);
	animations->addState(backHitAnimation);

	ConditionCallback isAlive = ConditionCallback([entity]() {
		bool result = false;
		IComponent* component = entity->getComponent("life");
		if (component != nullptr) {
			LifeComponent* life = static_cast<LifeComponent*>(component);
			result = life->isAlive();
		}
		return result;
	});
	ConditionComparison<int> conditionDamageToIdle = ConditionComparison<int>(&controller->damage, 0);
	ConditionComparison<int> conditionBackDamage = ConditionComparison<int>(&controller->damage, 0, Compare::LESS);
	StateTransition<Animation> transitionBackDamage = StateTransition<Animation>(backHitAnimation, &conditionBackDamage);
	StateTransition<Animation> transitionDamageIdle = StateTransition<Animation>(idleAnimation, &conditionDamageToIdle);
				transitionDamageIdle.addCondition(&isAlive);

	idleAnimation->addTransition(&transitionBackDamage);
	forwardAnimation->addTransition(&transitionBackDamage);
	downAnimation->addTransition(&transitionBackDamage);
	lookingUpAnimation->addTransition(&transitionBackDamage);
	jumpAnimation->addTransition(&transitionBackDamage);
	fallAnimation->addTransition(&transitionBackDamage);
	doubleJumpAnimation->addTransition(&transitionBackDamage);
	startJumpAnimation->addTransition(&transitionBackDamage);
	backHitAnimation->addTransition(&transitionDamageIdle);

	// ---------------------------------------------

	Animation frontHit(1);
	frontHit.sizeFrame = {64, 128, 64,64};
	frontHit.offset = {-18,-17};
	State<Animation>* frontHitAnimation = new State<Animation>(frontHit);
	animations->addState(frontHitAnimation);

	ConditionComparison<int> conditionFrontDamage = ConditionComparison<int>(&controller->damage, 0, Compare::GREATER);
	StateTransition<Animation> transitionFrontDamage = StateTransition<Animation>(frontHitAnimation, &conditionFrontDamage);

	idleAnimation->addTransition(&transitionFrontDamage);
	downAnimation->addTransition(&transitionFrontDamage);
	lookingUpAnimation->addTransition(&transitionFrontDamage);
	startJumpAnimation->addTransition(&transitionFrontDamage);
	jumpAnimation->addTransition(&transitionFrontDamage);
	forwardAnimation->addTransition(&transitionFrontDamage);
	doubleJumpAnimation->addTransition(&transitionFrontDamage);
	fallAnimation->addTransition(&transitionFrontDamage);
	frontHitAnimation->addTransition(&transitionDamageIdle);

	// ---------------------------------------------

	Animation simpleAttack(2);
	simpleAttack.sizeFrame = {384,128,64,64};
	simpleAttack.speed = 0.2f;
	simpleAttack.offset = {-18,-17};
	simpleAttack.repeat = 1;
	State<Animation>* simpleAttackAnimation = new State<Animation>(simpleAttack);
	animations->addState(simpleAttackAnimation);

	TimerCondition conditionTimer = TimerCondition(200);
	ConditionCallback finishedAnimation = ConditionCallback([entity]() {
		bool result = false;
		IComponent*component = entity->getComponent("animations");
		if (component != nullptr) {
			AnimationComponent* animation = static_cast<AnimationComponent*>(component);
			result = animation->getActualAnimation()->isFinished();
		}
		return result;
	});
	ConditionComparison<int> conditionSimpleAttack = ConditionComparison<int>(&controller->attack, 0, Compare::NON_EQUAL);
	StateTransition<Animation> transitionSimpleAttack = StateTransition<Animation>(simpleAttackAnimation, &conditionSimpleAttack);
	StateTransition<Animation> transitionIdleAttack = StateTransition<Animation>(idleAnimation, &finishedAnimation);
				transitionIdleAttack.addCondition(&conditionTimer);

	idleAnimation->addTransition(&transitionSimpleAttack);
	forwardAnimation->addTransition(&transitionSimpleAttack);
	simpleAttackAnimation->addTransition(&transitionIdleAttack);

	// ---------------------------------------------

	Animation simpleAttackDown(2);
	simpleAttackDown.sizeFrame = {512,128,64,64};
	simpleAttackDown.speed = 0.2f;
	simpleAttackDown.offset = {-18,-17};
	simpleAttackDown.repeat = 1;
	State<Animation>* simpleAttackDownAnimation = new State<Animation>(simpleAttackDown);
	animations->addState(simpleAttackDownAnimation);

	StateTransition<Animation> transitionSimpleAttackDown = StateTransition<Animation>(simpleAttackDownAnimation, &conditionSimpleAttack);
	StateTransition<Animation> transitionToDown= StateTransition<Animation>(downAnimation, &finishedAnimation);
								transitionToDown.addCondition(&conditionTimer);

	simpleAttackDownAnimation->addTransition(&transitionToDown);
	downAnimation->addTransition(&transitionSimpleAttackDown);

	// ---------------------------------------------

	Animation simpleAttackUp(2);
	simpleAttackUp.sizeFrame = {512,192,64,64};
	simpleAttackUp.speed = 0.2f;
	simpleAttackUp.offset = {-18,-17};
	simpleAttackUp.repeat = 1;
	State<Animation>* simpleAttackUpAnimation = new State<Animation>(simpleAttackUp);
	animations->addState(simpleAttackUpAnimation);

	StateTransition<Animation> transitionSimpleAttackUp = StateTransition<Animation>(simpleAttackUpAnimation, &conditionSimpleAttack);
	StateTransition<Animation> transitionToUp = StateTransition<Animation>(lookingUpAnimation, &finishedAnimation);
	transitionToUp.addCondition(&conditionTimer);

	simpleAttackUpAnimation->addTransition(&transitionToUp);
	lookingUpAnimation->addTransition(&transitionSimpleAttackUp);

	// ---------------------------------------------

	Animation simpleAttackJumping(2);
	simpleAttackJumping.sizeFrame = {640,128,64,64};
	simpleAttackJumping.speed = 0.2f;
	simpleAttackJumping.offset = {-18,-17};
	simpleAttackJumping.repeat = 1;
	State<Animation>* simpleAttackJumpingAnimation = new State<Animation>(simpleAttackJumping);
	animations->addState(simpleAttackJumpingAnimation);
	
	ConditionCallback fallOrIdle= ConditionCallback([controller]() {
		return controller->stateJump== TypeJump::FALL || controller->stateJump == TypeJump::NONE;
	});
	StateTransition<Animation> transitionSimpleAttackJumping= StateTransition<Animation>(simpleAttackJumpingAnimation, &conditionSimpleAttack);
				transitionSimpleAttack.addCondition(&conditionJump);
	StateTransition<Animation> transitionAttackJump = StateTransition<Animation>(jumpAnimation, &finishedAnimation);
				transitionAttackJump.addCondition(&conditionTimer);
				transitionAttackJump.addCondition(&conditionJump);
	StateTransition<Animation> transitionAttackFall = StateTransition<Animation>(fallAnimation, &finishedAnimation);
				transitionAttackFall.addCondition(&conditionTimer);
				transitionAttackFall.addCondition(&fallOrIdle);

	jumpAnimation->addTransition(&transitionSimpleAttackJumping);
	idleAnimation->addTransition(&transitionSimpleAttackJumping);
	simpleAttackJumpingAnimation->addTransition(&transitionAttackJump);
	fallAnimation->addTransition(&transitionSimpleAttackJumping);
	simpleAttackJumpingAnimation->addTransition(&transitionAttackFall);

	// ---------------------------------------------

	Animation frontDead(3);
	frontDead.sizeFrame = {0, 448,128,128};
	frontDead.offset = {-65,-82};
	frontDead.flippedOffset.x = 22;
	frontDead.speed = 0.2f;
	frontDead.repeat = 1;
	State<Animation>* frontDeadAnimation = new State<Animation>(frontDead);
	animations->addState(frontDeadAnimation);

	ConditionComparison<int> conditionDead = ConditionComparison<int>(&controller->damage, 3);
	StateTransition<Animation> transitionFrontDead = StateTransition<Animation>(frontDeadAnimation, &conditionDead);
	ConditionComparison<int> conditionDead2 = ConditionComparison<int>(&controller->damage, -3);
	StateTransition<Animation> transitionFrontDead2 = StateTransition<Animation>(frontDeadAnimation, &conditionDead2);

	frontHitAnimation->addTransition(&transitionFrontDead);
	frontHitAnimation->addTransition(&transitionFrontDead2);
				

	// ---------------------------------------------
	
	Animation backDead(3);
	backDead.sizeFrame = {0,320,128,128};
	backDead.offset = {-40,-82};
	backDead.flippedOffset.x = -22;
	backDead.speed = 0.2f;
	backDead.repeat = 1;
	State<Animation>* backDeadAnimation = new State<Animation>(backDead);
	animations->addState(backDeadAnimation);

	StateTransition<Animation> transitionBackDead = StateTransition<Animation>(backDeadAnimation, &conditionDead);
	StateTransition<Animation> transitionBackDead2 = StateTransition<Animation>(backDeadAnimation, &conditionDead2);

	backHitAnimation->addTransition(&transitionBackDead);
	backHitAnimation->addTransition(&transitionBackDead2);


	// ---------------------------------------------

	Animation ghostBackDead(4);
	ghostBackDead.sizeFrame = {256,320,128,128};
	ghostBackDead.offset = {-40,-82};
	ghostBackDead.flippedOffset.x = -22;
	ghostBackDead.speed = 0.08f;
	ghostBackDead.repeat = 1;
	State<Animation>* ghostBackDeadAnimation = new State<Animation>(ghostBackDead);
	animations->addState(ghostBackDeadAnimation);
	
	StateTransition<Animation> transitionghostBackDead = StateTransition<Animation>(ghostBackDeadAnimation, &finishedAnimation);
	backDeadAnimation->addTransition(&transitionghostBackDead);

	// ---------------------------------------------

	Animation ghostFrontDead(4);
	ghostFrontDead.sizeFrame = {256,448,128,128};
	ghostFrontDead.offset = {-65,-82};
	ghostFrontDead.flippedOffset.x = 22;
	ghostFrontDead.speed = 0.08f;
	ghostFrontDead.repeat = 1;
	State<Animation>* ghostFrontDeadAnimation = new State<Animation>(ghostFrontDead);
	animations->addState(ghostFrontDeadAnimation);

	StateTransition<Animation> transitionghostFrontDead = StateTransition<Animation>(ghostFrontDeadAnimation, &finishedAnimation);
	frontDeadAnimation->addTransition(&transitionghostFrontDead);

	// ---------------------------------------------

	Animation ghostBack(4);
	ghostBack.sizeFrame = {0,256,64,64};
	ghostBack.offset = {13,-25};
	ghostBack.flippedOffset.x = -56;
	ghostBack.speed = 0.08f;
	State<Animation>* ghostBackAnimation = new State<Animation>(ghostBack);
	animations->addState(ghostBackAnimation);

	StateTransition<Animation> transitionghostBack = StateTransition<Animation>(ghostBackAnimation, &finishedAnimation);
	ghostBackDeadAnimation->addTransition(&transitionghostBack);


	// ---------------------------------------------

	Animation ghostFront(4);
	ghostFront.sizeFrame = {0,256,64,64};
	ghostFront.offset = {-23,-25};
	ghostFront.flippedOffset.x = 0;
	ghostFront.speed = 0.08f;
	State<Animation>* ghostFrontAnimation = new State<Animation>(ghostFront);
	animations->addState(ghostFrontAnimation);

	StateTransition<Animation> transitionghostFront = StateTransition<Animation>(ghostFrontAnimation, &finishedAnimation);
	ghostFrontDeadAnimation->addTransition(&transitionghostFront);


	// ---------------------------------------------

	Animation victory(4);
	victory.sizeFrame = {384, 0, 64, 64};
	victory.offset = {-18,-17};
	victory.speed = 0.1f;
	State<Animation>* victoryAnimation = new State<Animation>(victory);
	animations->addState(victoryAnimation);

	ConditionComparison<bool> conditionVictory = ConditionComparison<bool>(&App->player->levelFinished, true);
	ConditionComparison<bool> conditionIdleVictory = ConditionComparison<bool>(&App->player->levelFinished, false);
	StateTransition<Animation> transitionVictory = StateTransition<Animation>(victoryAnimation, &conditionVictory);
	StateTransition<Animation> transitionVictoryIdle = StateTransition<Animation>(idleAnimation, &conditionIdleVictory);
	
	victoryAnimation->addTransition(&transitionVictoryIdle);
	forwardAnimation->addTransition(&transitionVictory);
	idleAnimation->addTransition(&transitionVictory);
	simpleAttackAnimation->addTransition(&transitionVictory);
	lookingUpAnimation->addTransition(&transitionVictory);



	//create the component
	AnimationComponent* component=new AnimationComponent("animations", "Joe.png", animations);
	entity->addComponent(component);
}
