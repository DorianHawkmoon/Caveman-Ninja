#include "Globals.h"
#include "ModulePlayer.h"
#include "Entity.h"
#include "Joe.h"
#include "MotionComponent.h"
#include "ModuleRender.h"
#include "Application.h"
#include "ModuleInput.h"
#include "SDL\SDL_keycode.h"
#include "JumpComponent.h"
#include "LifeComponent.h"
#include "PlayerWeapon.h"
#include "WeaponComponent.h"
#include "CollisionComponent.h"
#include "MotionComponent.h"
#include "ModuleAudio.h"
#include "EntryScene.h"
#include "AnimationComponent.h"
#include "GUISprite.h"
#include "ModuleGUI.h"
#include "Animation.h"
#include "SpriteComponent.h"
#include "ScoreComponent.h"
#include "ConditionComparison.h"
#include "GUIScore.h"
#include "TimerCondition.h"
#include "GUIAnimation.h"
#include "ConditionCallback.h"
#include "Timer.h"
#include "ModuleTimer.h"

ModulePlayer::ModulePlayer(bool start_enabled) : Module(start_enabled), lifes(1), player(nullptr)
{
}

ModulePlayer::~ModulePlayer()
{
	// Homework : check for memory leaks
	if (player != nullptr) {
		delete player;
	}
}

// Load assets
bool ModulePlayer::start(){
	LOG("Loading player");
	bool started = true;

	if (lifes <= 0 || player==nullptr) {
		if (player != nullptr) {
			delete player;
		}
		player = Joe::makeJoe();
		lifes = 1;
	}

	player->start();
	ghost = false;
	deadBody = nullptr;
	started = started & ((motion = static_cast<MotionComponent*>(player->getComponent("motion")))!=nullptr);
	started = started & ((jump = static_cast<JumpComponent*>(player->getComponent("jump"))) != nullptr);
	started = started & ((life = static_cast<LifeComponent*>(player->getComponent("life"))) != nullptr);
	started = started & ((weapon = static_cast<WeaponComponent*>(player->getComponent("weapon"))) != nullptr);
	started = started & ((score = static_cast<ScoreComponent*>(player->getComponent("score"))) != nullptr);
	player->transform->position = {20, 170};
	player->controller.stateJump = TypeJump::FALL;

	//restore his life
	life->setActualLife(life->getMaxLife());
	score->score.resetScore();
	gameOverTimer = App->timer->getTimer();
	gameOverTimer->stop();
	levelFinished = false;

	App->renderer->camera.setCamera(player->transform);
	App->renderer->camera.offset.x = 30;

	soundDie = App->audio->loadEffect("player_die.wav");

	//create the gui
	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	GUI::GUIScore* scoreText  = new GUI::GUIScore(&score->score, color, "arcadepix.ttf", GUILocation::ABSOLUTE, 8);
	scoreText->transform.position = {50, 2};

	App->scene->addGUI(scoreText);
	portraitAnimation();

	return started;
}

update_status ModulePlayer::preUpdate() {
	if (deadBody != nullptr) {
		deadBody->preUpdate();
	}
	player->preUpdate();
	return UPDATE_CONTINUE;
}

// Unload assets
bool ModulePlayer::cleanUp(){
	LOG("Unloading player");
	if (deadBody != nullptr) {
		deadBody->cleanUp();
		delete deadBody;
		deadBody = nullptr;
	}
	if (player != nullptr) {
		player->cleanUp();
	}
	App->timer->deleteTimer(gameOverTimer);
	return true;
}

void ModulePlayer::addGameOver() {
	GUI::GUISprite* gameOver = new GUI::GUISprite("gameover.png");
	App->scene->addGUI(gameOver);
}

void ModulePlayer::dead() {
	ControlEntity* controller = &player->controller;

	if (deadBody != nullptr) {
		motion->velocity.y = -(motion->speed*0.6F);
		motion->velocity.x = 0;

	} else if ((controller->damage == 3 || controller->damage == -3) && deadBody == nullptr) {
		AnimationComponent* animation = static_cast<AnimationComponent*>(player->getComponent("animations"));
		if (animation->getActualAnimation()->isInfinity()) {
			//start the countdown!
			gameOverTimer->start();
			addGameOver();
			lifes--;

			deadBody = new Entity();

			Transform transPlayer = player->transform->getGlobalTransform();
			deadBody->transform->position = transPlayer.position;
			deadBody->transform->flip = transPlayer.flip;
			deadBody->transform->rotation = transPlayer.rotation;

			SpriteComponent* body = new SpriteComponent("body", "Joe.png");
			if (controller->damage < 0) {
				body->rect = {256,320, 128,128};
				body->offset = {-40,-82};
				body->flippedOffset.x = -22;

			} else {
				body->rect = {256,448, 128,128};
				body->offset = {-65,-82};
				body->flippedOffset.x = 22;
			}
			deadBody->addComponent(body);
			deadBody->start();
			App->audio->playEffect(soundDie);
		}
	}
}

void ModulePlayer::debugging() {
	if (App->input->getKey(SDL_SCANCODE_T) == KEY_DOWN) {
		DEBUG_COLLISIONS = (DEBUG_COLLISIONS == 0) ? 1 : 0;
	}
}

void ModulePlayer::portraitAnimation() {
	ControlEntity* controller = &player->controller;
	StateMachine<Animation>* animations;

	Animation idle(1);
	idle.sizeFrame = {0, 0, 24,28};
	State<Animation>* idleAnimation = new State<Animation>(idle);
	animations = new StateMachine<Animation>(idleAnimation);

	// ---------------------------------------------

	Animation damage(1);
	damage.sizeFrame = {0, 28,24,28};
	State<Animation>* damageAnimation = new State<Animation>(damage);
	animations->addState(damageAnimation);

	ConditionComparison<int> conditionDamage = ConditionComparison<int>(&controller->damage, 0, NON_EQUAL);
	ConditionComparison<int> conditionIdle = ConditionComparison<int>(&controller->damage, 0);
	StateTransition<Animation> transitionDamage = StateTransition<Animation>(damageAnimation, &conditionDamage);
	StateTransition<Animation> transitionIdle = StateTransition<Animation>(idleAnimation, &conditionIdle);

	damageAnimation->addTransition(&transitionIdle);
	idleAnimation->addTransition(&transitionDamage);


	// ---------------------------------------------

	Animation eating(1);
	eating.sizeFrame = {24, 0, 24,28};
	State<Animation>* eatingAnimation = new State<Animation>(eating);
	animations->addState(eatingAnimation);

	TimerCondition conditionIdle2 = TimerCondition(1000);
	ConditionCallback conditionEating = ConditionCallback([=](){
		int actuaLife = life->getActualLife();
		if (life->previousLife < actuaLife) {
			life->previousLife = actuaLife;
			return true;
		} else {
			return false;
		}
		});
	StateTransition<Animation> transitionIdle2 = StateTransition<Animation>(idleAnimation, &conditionIdle2);
	StateTransition<Animation> transitionEating = StateTransition<Animation>(eatingAnimation, &conditionEating);

	idleAnimation->addTransition(&transitionEating);
	eatingAnimation->addTransition(&transitionIdle2);
	eatingAnimation->addTransition(&transitionDamage);

	// ---------------------------------------------

	Animation deadly(1);
	deadly.sizeFrame = {24, 28, 24,28};
	State<Animation>* deadlyAnimation = new State<Animation>(deadly);
	animations->addState(deadlyAnimation);

	ConditionComparison<int> conditionDead = ConditionComparison<int>(&controller->damage, 3);
	ConditionComparison<int> conditionDead2 = ConditionComparison<int>(&controller->damage, -3);
	StateTransition<Animation> transitionDead = StateTransition<Animation>(deadlyAnimation, &conditionDead);
	StateTransition<Animation> transitionDead2 = StateTransition<Animation>(deadlyAnimation, &conditionDead2);

	idleAnimation->addTransition(&transitionDead);
	damageAnimation->addTransition(&transitionDead);
	idleAnimation->addTransition(&transitionDead2);
	damageAnimation->addTransition(&transitionDead2);

	GUI::GUIAnimation* animationPortrait = new GUI::GUIAnimation("gui_portrait.png", animations);
	animationPortrait->transform.position = {0,0};
	App->scene->addGUI(animationPortrait);
}


// Update
update_status ModulePlayer::update(){
	debugging();

	ControlEntity* controller = &player->controller;
	Transform* trans = player->transform;

	//finished level and not dead
	if (levelFinished && !gameOverTimer->isStopped() && gameOverTimer->value() > 3000) {
		Scene* next = new EntryScene();
		App->scene->changeScene(next, 1);
		gameOverTimer->stop();
		App->audio->stopMusic(); //stop music

	//level finished because of game over
	}else if (levelFinished && gameOverTimer->isStopped()) {
		controller->moveX = 0;
		controller->moveY = 0;
		controller->attack = 0;
		controller->damage = 0;
		gameOverTimer->start();
		//put the animation
	}

	//don't move while receiving damage
	if (controller->damage == 0 && levelFinished==false) {
		controller->moveX = 0;
		controller->moveY = 0;

		controller->attack = 0;

		if (App->input->getKey(SDL_SCANCODE_A)) {
			controller->moveX -= 1;
			trans->flip = SDL_FLIP_HORIZONTAL;
		}

		if (App->input->getKey(SDL_SCANCODE_D)) {
			controller->moveX += 1;
			trans->flip = SDL_FLIP_NONE;
		}

		if (App->input->getKey(SDL_SCANCODE_W)) {
			controller->moveY -= 1;
		}

		if (App->input->getKey(SDL_SCANCODE_S)) {
			controller->moveY += 1;
		}

		if (App->input->getKey(SDL_SCANCODE_L)==KEY_DOWN) {
			weapon->throwWeapon();
			motion->velocity.x = 0;
		}



		//don't jump again when jumping or falling
		if (App->input->getKey(SDL_SCANCODE_J)== KEY_DOWN && controller->stateJump == TypeJump::NONE) {
			if (App->input->getKey(SDL_SCANCODE_W)) {
				controller->stateJump = TypeJump::DOUBLE_JUMP;
			} else if(App->input->getKey(SDL_SCANCODE_S)){
				controller->stateJump = TypeJump::JUMP_DOWN;
			} else {
				controller->stateJump = TypeJump::JUMP;
			}
		}

		if (controller->moveY != 1) {
			motion->velocity.x = controller->moveX * motion->speed;
		} else {
			motion->velocity.x = 0.0f;
		}
		
	}

	dead();
	if (deadBody != nullptr) {
		deadBody->update();
	}
	player->update();

	if (!gameOverTimer->isStopped() && gameOverTimer->value() > 10000) {
		Scene* next = new EntryScene();
		App->scene->changeScene(next, 1);
		gameOverTimer->stop();
		App->audio->stopMusic(); //stop music
	}
	return UPDATE_CONTINUE;
}

update_status ModulePlayer::postUpdate() {
	if (deadBody != nullptr) {
		deadBody->postUpdate();
	}
	player->postUpdate();
	return UPDATE_CONTINUE;
}