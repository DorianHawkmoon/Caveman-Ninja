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

Entity * Joe::makeJoe() {
	//prepare the entity for the player
	Entity* result = new Entity();

	MotionComponent* motion = new MotionComponent("motion");
	motion->velocity.x = 0;
	motion->velocity.y = 0;
	result->addComponent(motion);

	makeAnimations(result);

	return result;
}

void Joe::makeAnimations(Entity* entity) {
	StateMachine<Animation>* animations;

	Animation forward(4);
	Animation idle(1);

	idle.sizeFrame={0, 0, 28, 47};
	forward.sizeFrame={0, 102, 42, 48};
	forward.speed = 0.05f;

	//states
	State<Animation>* forwardAnimation = new State<Animation>(forward);
	State<Animation>* idleAnimation = new State<Animation>(idle);

	//condition's function
	//condition to go forward
	auto toForward = []() {
		KeyState key = App->input->GetKey(SDL_SCANCODE_D);
		return (key == KEY_DOWN || key == KEY_REPEAT);
	};
	//condition to go to idle
	auto toIdle = []() {
		return (App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE);
	};
	//condiciones
	Condition* conditionForward = new ConditionCallback(toForward);
	Condition* conditionIdle = new ConditionCallback(toIdle);

	//transitions
	StateTransition<Animation>* transitionForward = new StateTransition<Animation>(forwardAnimation);
	transitionForward->addCondition(conditionForward);
	idleAnimation->addTransition(transitionForward);

	StateTransition<Animation>* transitionIdle = new StateTransition<Animation>(idleAnimation);
	transitionIdle->addCondition(conditionIdle);
	forwardAnimation->addTransition(transitionIdle);
	

	animations = new StateMachine<Animation>(idleAnimation);
	animations->addState(forwardAnimation);

	//create the component
	AnimationComponent* component=new AnimationComponent("animations", "Joe.png", animations);
	entity->addComponent(component);
}
