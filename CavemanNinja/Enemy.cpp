#include "Enemy.h"
#include "Entity.h"
#include "Animation.h"
#include "StateMachine.h"
#include "State.h"
#include "MotionComponent.h"
#include "AnimationComponent.h" 
#include "Application.h"
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
#include "IAComponent.h"
#include "EnemyBehaviour.h"
#include "DisappearOutCamera.h"
#include "ConditionCallback.h"
#include "LifeComponent.h"
#include "EnemyHittedComponent.h"
#include "DamageComponent.h"
#include "DropItemComponent.h"
#include "Utils.h"

Entity * Enemy::makeEnemy(const TypeItem itemToDrop) {
	//prepare the entity for the player
	Entity* result = new Entity();

	MotionComponent* motion = new MotionComponent("motion");
	motion->velocity.x = 0;
	motion->velocity.y = 0;
	motion->speed = 25;
	motion->doubleSpeed = 75;
	result->addComponent(motion);

	EnemyHittedComponent* hitted = new EnemyHittedComponent("hitted");
	result->addComponent(hitted);

	DamageComponent* damage = new DamageComponent("damage", 5, 5);
	result->addComponent(damage);

	CircleCollider* circleGravity = new CircleCollider(fPoint(12, 41), 1, TypeCollider::GRAVITY);
	GravityComponent* gravity = new GravityComponent("gravity", circleGravity);
	gravity->gravity = 550;
	gravity->maxVelocity = 500;
	result->addComponent(gravity);

	JumpComponent* jump = new JumpComponent("jump");
	jump->speed = 250;
	jump->doubleSpeed = 350;
	result->addComponent(jump);

	RectangleCollider* rectangle = new RectangleCollider(fPoint(4, 5), iPoint(18, 35), 0, TypeCollider::ENEMY);
	CollisionComponent* collider = new CollisionComponent("collider", rectangle);
	result->addComponent(collider);

	LifeComponent* life = new LifeComponent("life", 1);
	result->addComponent(life);

	ControlEntity* controller = &result->controller;
	DisappearOutCamera* disappear = new DisappearOutCamera("runningAway");
	ConditionComparison<int> conditionRunningAway = ConditionComparison<int>(&controller->attack, 3);
	disappear->addCondition(&conditionRunningAway);
	result->addComponent(disappear);

	disappear = new DisappearOutCamera("runningDead");
	ConditionCallback conditionDead = ConditionCallback([result]() {
		LifeComponent* life = static_cast<LifeComponent*>(result->getComponent("life"));
		if (life != nullptr) {
			return !life->isAlive();
		}
		return false;
	});
	disappear->addCondition(&conditionDead);
	result->addComponent(disappear);

	Entity* item = Items::makeItem(itemToDrop);
	DropItemComponent* drop = new DropItemComponent("drop", item);
	result->addComponent(drop);

	IAComponent* IA = new EnemyBehaviour("IA");
	result->addComponent(IA);
	IA->ticks = static_cast<int>(Utils::range(2000, 3000)); //RANDOM IA

	makeAnimations(result);

	return result;
}


void Enemy::makeAnimations(Entity* entity) {
	ControlEntity* controller = &entity->controller;
	StateMachine<Animation>* animations;


	Animation idle(4);
	idle.sizeFrame = {0, 512, 128, 128};
	idle.offset = {-51,-82};
	idle.speed = 0.1f;
	State<Animation>* idleAnimation = new State<Animation>(idle);
	animations = new StateMachine<Animation>(idleAnimation);

	// ---------------------------------------------

	Animation forward(4);
	forward.sizeFrame = {0,0, 128, 128};
	forward.offset = {-51,-82};
	forward.speed = 0.1f;
	State<Animation>* forwardAnimation = new State<Animation>(forward);
	animations->addState(forwardAnimation);

	ConditionComparison<int> conditionForward = ConditionComparison<int>(&controller->moveX, 1);
	ConditionComparison<int> conditionBackward = ConditionComparison<int>(&controller->moveX, -1);
	ConditionComparison<int> conditionIdle1 = ConditionComparison<int>(&controller->moveX, 0);
	StateTransition<Animation> transitionForward = StateTransition<Animation>(forwardAnimation, &conditionForward);
	StateTransition<Animation> transitionBackward = StateTransition<Animation>(forwardAnimation, &conditionBackward);
	StateTransition<Animation> transitionIdle1 = StateTransition<Animation>(idleAnimation, &conditionIdle1);

	idleAnimation->addTransition(&transitionForward);
	idleAnimation->addTransition(&transitionBackward);
	forwardAnimation->addTransition(&transitionIdle1);

	// ---------------------------------------------

	Animation lookingUp(1);
	lookingUp.sizeFrame = {0, 384, 128, 128};
	lookingUp.offset = {-51,-82};
	State<Animation>* lookingUpAnimation = new State<Animation>(lookingUp);
	animations->addState(lookingUpAnimation);

	ConditionComparison<int> conditionLookingUp = ConditionComparison<int>(&controller->moveY, -1);
	ConditionComparison<int> conditionIdle2 = ConditionComparison<int>(&controller->moveY, 0);
	StateTransition<Animation> transitionIdle2 = StateTransition<Animation>(idleAnimation, &conditionIdle2);
	StateTransition<Animation> transitionLookingUp = StateTransition<Animation>(lookingUpAnimation, &conditionLookingUp);

	idleAnimation->addTransition(&transitionLookingUp);
	lookingUpAnimation->addTransition(&transitionIdle2);
	lookingUpAnimation->addTransition(&transitionForward);
	lookingUpAnimation->addTransition(&transitionBackward);

	// ---------------------------------------------

	Animation down(1);
	down.sizeFrame = {128, 384, 128, 128};
	down.offset = {-51,-82};
	State<Animation>* downAnimation = new State<Animation>(down);
	animations->addState(downAnimation);

	ConditionComparison<int> conditionDown = ConditionComparison<int>(&controller->moveY, 1);
	StateTransition<Animation> transitionDown = StateTransition<Animation>(downAnimation, &conditionDown);

	idleAnimation->addTransition(&transitionDown);
	downAnimation->addTransition(&transitionIdle2);

	// ---------------------------------------------

	Animation jump(1);
	jump.sizeFrame = {512,0,128,128};
	jump.offset = {-46,-65};
	jump.flippedOffset.x = -6;
	State<Animation>* jumpAnimation = new State<Animation>(jump);
	animations->addState(jumpAnimation);

	ConditionComparison<TypeJump> conditionJump = ConditionComparison<TypeJump>(&controller->stateJump, TypeJump::JUMP);
	ConditionComparison<TypeJump> conditionJump2 = ConditionComparison<TypeJump>(&controller->stateJump, TypeJump::DOUBLE_JUMP);
	StateTransition<Animation> transitionJump = StateTransition<Animation>(jumpAnimation, &conditionJump);
	StateTransition<Animation> transitionJump2 = StateTransition<Animation>(jumpAnimation, &conditionJump2);

	idleAnimation->addTransition(&transitionJump);
	forwardAnimation->addTransition(&transitionJump);
	lookingUpAnimation->addTransition(&transitionJump);
	idleAnimation->addTransition(&transitionJump2);
	forwardAnimation->addTransition(&transitionJump2);
	lookingUpAnimation->addTransition(&transitionJump2);

	// ---------------------------------------------

	Animation startFall(1);
	startFall.sizeFrame = {640,0,128,128};
	startFall.offset = {-48,-82};
	startFall.flippedOffset.x = -6;
	State<Animation>* startFallAnimation = new State<Animation>(startFall);
	animations->addState(startFallAnimation);

	ConditionComparison<TypeJump> conditionFall = ConditionComparison<TypeJump>(&controller->stateJump, TypeJump::FALL);
	StateTransition<Animation> transitionFall = StateTransition<Animation>(startFallAnimation, &conditionFall);

	jumpAnimation->addTransition(&transitionFall);

	// ---------------------------------------------

	Animation fall(1);
	fall.sizeFrame = {768, 0, 128, 128};
	fall.offset = {-45,-82};
	fall.flippedOffset.x = -12;
	State<Animation>* fallAnimation = new State<Animation>(fall);
	animations->addState(fallAnimation);

	TimerCondition conditionFallTimer = TimerCondition(300);
	StateTransition<Animation> transitionFall2 = StateTransition<Animation>(fallAnimation, &conditionFall);
	StateTransition<Animation> transitionFalling = StateTransition<Animation>(fallAnimation, &conditionFallTimer);
		transitionFalling.addCondition(&conditionFall);

	//idle forward y otros pasan a caer aquí, con fall
	startFallAnimation->addTransition(&transitionFalling); //con el tiempo empieza de caer a cayendo
	//si es cayendo, idle y otros pasan a esta animacion
	idleAnimation->addTransition(&transitionFall2);
	forwardAnimation->addTransition(&transitionFall2);
	downAnimation->addTransition(&transitionFall2);

	// ---------------------------------------------

	Animation endFall(1);
	endFall.sizeFrame = {896, 0, 128, 128};
	endFall.offset = {-45,-82};
	endFall.flippedOffset.x = -10;
	State<Animation>* endFallAnimation = new State<Animation>(endFall);
	animations->addState(endFallAnimation);

	TimerCondition conditionFallTimer2 = TimerCondition(500);
	ConditionCallback conditionFallToEnd = ConditionCallback([controller]() {
		bool result = false;
		result = (controller->stateJump != TypeJump::FALL && controller->stateJump != TypeJump::JUMP_DOWN);
		return result;
	});
	StateTransition<Animation> transitionFallToEnd = StateTransition<Animation>(endFallAnimation, &conditionFallToEnd);
	StateTransition<Animation> transitionFallToIdle = StateTransition<Animation>(idleAnimation, &conditionFallTimer2);
							transitionFallToIdle.addCondition(&conditionFallToEnd);

	startFallAnimation->addTransition(&transitionFallToEnd);
	fallAnimation->addTransition(&transitionFallToEnd);
	endFallAnimation->addTransition(&transitionFallToIdle);

	// ---------------------------------------------

	Animation run(4);
	run.sizeFrame = {0,128, 128, 128};
	run.offset = {-47, -82};
	run.flippedOffset.x = -9;
	run.speed = 0.17f;
	State<Animation>* runAnimation = new State<Animation>(run);
	animations->addState(runAnimation);

	ConditionComparison<bool> conditionRun = ConditionComparison<bool>(&controller->run, true);
	ConditionComparison<bool> conditionNotRun = ConditionComparison<bool>(&controller->run, false);
	StateTransition<Animation> transitionRun = StateTransition<Animation>(runAnimation, &conditionRun);
	StateTransition<Animation> transitionRunIddle = StateTransition<Animation>(idleAnimation, &conditionNotRun);

	idleAnimation->addTransition(&transitionRun);
	forwardAnimation->addTransition(&transitionRun);
	runAnimation->addTransition(&transitionRunIddle);

	// ---------------------------------------------

	Animation attack(3);
	attack.sizeFrame = {0,256,128,128};
	attack.offset = {-53,-82};
	attack.flippedOffset.x = 3;
	attack.speed = 0.2f;
	attack.repeat = 1;
	State<Animation>* attackAnimation = new State<Animation>(attack);
	animations->addState(attackAnimation);

	ConditionComparison<int> conditionAttack = ConditionComparison<int>(&controller->attack, 1);
	StateTransition<Animation> transitionAttack = StateTransition<Animation>(attackAnimation, &conditionAttack);

	idleAnimation->addTransition(&transitionAttack);
	forwardAnimation->addTransition(&transitionAttack);
	lookingUpAnimation->addTransition(&transitionAttack);
	runAnimation->addTransition(&transitionAttack);

	// ---------------------------------------------

	Animation startRunningAway(3);
	startRunningAway.sizeFrame = {0,640,128,128};
	startRunningAway.offset = {-53, -82};
	startRunningAway.flippedOffset.x = 3;
	startRunningAway.repeat = 4;
	startRunningAway.speed = 0.3f;
	State<Animation>* startRunningAwayAnimation = new State<Animation>(startRunningAway);
	animations->addState(startRunningAwayAnimation);

	ConditionComparison<int> conditionStartRunningAway = ConditionComparison<int>(&controller->attack, 2);
	StateTransition<Animation> transitionStartRunningAway = StateTransition<Animation>(startRunningAwayAnimation, &conditionStartRunningAway);

	attackAnimation->addTransition(&transitionStartRunningAway);

	// ---------------------------------------------

	Animation runningAway(5);
	runningAway.sizeFrame = {384,640,128,128};
	runningAway.offset = {-53,-82};
	runningAway.flippedOffset.x = 3;
	runningAway.speed = 0.4f;
	State<Animation>* runningAwayAnimation = new State<Animation>(runningAway);
	animations->addState(runningAwayAnimation);

	ConditionComparison<int> conditionRunningAway = ConditionComparison<int>(&controller->attack, 3);
	StateTransition<Animation> transitionRunningAway = StateTransition<Animation>(runningAwayAnimation, &conditionRunningAway);

	startRunningAwayAnimation->addTransition(&transitionRunningAway);

	// ---------------------------------------------

	Animation backHit(1);
	backHit.sizeFrame = {512,512,128,128};
	backHit.offset = {-49,-82};
	backHit.flippedOffset.x = -6;
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
	startRunningAwayAnimation->addTransition(&transitionBackDamage);
	attackAnimation->addTransition(&transitionBackDamage);
	runAnimation->addTransition(&transitionBackDamage);
	fallAnimation->addTransition(&transitionBackDamage);
	startFallAnimation->addTransition(&transitionBackDamage);
	endFallAnimation->addTransition(&transitionBackDamage);
	backHitAnimation->addTransition(&transitionDamageIdle);

	// ---------------------------------------------

	Animation frontHit(1);
	frontHit.sizeFrame = {512,384,128,128};
	frontHit.offset = {-60,-82};
	frontHit.flippedOffset.x = 17;

	State<Animation>* frontHitAnimation = new State<Animation>(frontHit);
	animations->addState(frontHitAnimation);

	ConditionComparison<int> conditionFrontDamage = ConditionComparison<int>(&controller->damage, 0, Compare::GREATER);
	StateTransition<Animation> transitionFrontDamage = StateTransition<Animation>(frontHitAnimation, &conditionFrontDamage);

	idleAnimation->addTransition(&transitionFrontDamage);
	forwardAnimation->addTransition(&transitionFrontDamage);
	downAnimation->addTransition(&transitionFrontDamage);
	lookingUpAnimation->addTransition(&transitionFrontDamage);
	runAnimation->addTransition(&transitionFrontDamage);
	attackAnimation->addTransition(&transitionFrontDamage);
	startRunningAwayAnimation->addTransition(&transitionFrontDamage);
	fallAnimation->addTransition(&transitionFrontDamage);
	startFallAnimation->addTransition(&transitionFrontDamage);
	endFallAnimation->addTransition(&transitionFrontDamage);
	frontHitAnimation->addTransition(&transitionDamageIdle);

	// ---------------------------------------------
	
	Animation frontDead(3);
	frontDead.sizeFrame = {640,384,128,128};
	frontDead.offset = {-53,-82};
	frontDead.flippedOffset.x = 3;
	frontDead.speed = 0.05f;
	frontDead.repeat = 1;
	State<Animation>* frontDeadAnimation = new State<Animation>(frontDead);
	animations->addState(frontDeadAnimation);

	ConditionComparison<int> conditionDead = ConditionComparison<int>(&controller->damage, 3);
	StateTransition<Animation> transitionFrontDead = StateTransition<Animation>(frontDeadAnimation, &conditionDead);

	frontHitAnimation->addTransition(&transitionFrontDead);
				

	// ---------------------------------------------
	
	Animation backDead(3);
	backDead.sizeFrame = {640,512,128,128};
	backDead.offset = {-53,-82};
	backDead.flippedOffset.x = 3;
	backDead.speed = 0.05f;
	backDead.repeat = 1;
	State<Animation>* backDeadAnimation = new State<Animation>(backDead);
	animations->addState(backDeadAnimation);

	StateTransition<Animation> transitionBackDead = StateTransition<Animation>(backDeadAnimation, &conditionDead);

	backHitAnimation->addTransition(&transitionBackDead);

	//create the component
	AnimationComponent* component = new AnimationComponent("animations", "troglodita.png", animations);
	entity->addComponent(component);
}
