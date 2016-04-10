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
#include "AnimationComponent.h"
#include "SpriteComponent.h"
#include "ScoreComponent.h"
#include "ConditionComparison.h"
#include "GUIScore.h"
#include "TimerCondition.h"
#include "GUIAnimation.h"
#include "ConditionCallback.h"
#include "Timer.h"
#include "GUIContainer.h"
#include "ModuleTimer.h"
#include "GUIComponent.h"
#include "GUILifeBar.h"

ModulePlayer::ModulePlayer(bool start_enabled) : Module(start_enabled), lifes(1), player(nullptr){
}

ModulePlayer::~ModulePlayer(){
	if (player != nullptr) {
		delete player;
	}

	if (HUD != nullptr) {
		delete HUD;
	}
}

// Load assets
bool ModulePlayer::start(){
	LOG("Loading player");
	bool started = true;
	attackAnimation = nullptr;

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
	started = started & ((animation = static_cast<AnimationComponent*>(player->getComponent("animations"))) != nullptr);
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
	makeHUD();

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

GUI::GUIComponent * ModulePlayer::getHUD() const {
	return HUD;
}

void ModulePlayer::addGameOver() {
	GUI::GUISprite* gameOver = new GUI::GUISprite("gameover.png");
	App->scene->addGUI(gameOver);
}

void ModulePlayer::dead() {
	ControlEntity* controller = &player->controller;

	//is a dead body, he's dead, update animation
	if (deadBody != nullptr) {
		motion->velocity.y = -(motion->speed*0.6F);
		motion->velocity.x = 0;
		deadBody->update();

	//check if is dead
	} else if ((controller->damage == 3 || controller->damage == -3) && deadBody == nullptr) {
		AnimationComponent* animation = static_cast<AnimationComponent*>(player->getComponent("animations"));
		if (animation->getActualAnimation()->isInfinity()) {
			//start the countdown!
			gameOverTimer->start();
			addGameOver();
			lifes--;

			//create the body lying on the ground while the player is flying as a ghost
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

void ModulePlayer::makeHUD() {
	GUI::GUIContainer* container = new GUI::GUIContainer();
	container->transform.location = GUILocation::TOP_LEFT;
	container->transform.pivot = GUILocation::TOP_LEFT;
	container->offset = fPoint(3, 3);
	container->setSize(iPoint(90*SCREEN_SIZE+24*SCREEN_SIZE, 15*SCREEN_SIZE));

	//portrait
	GUI::GUIComponent* animationPortrait = portraitAnimation();
	animationPortrait->transform.location = GUILocation::TOP_LEFT;
	animationPortrait->transform.pivot = GUILocation::TOP_LEFT;
	container->pack(animationPortrait);

	//lifebar
	GUI::GUILifeBar* lifebar = new GUI::GUILifeBar("gui_lifebar.png", App->player->life);
	lifebar->transform.location = GUILocation::BOTTOM_RIGHT;
	lifebar->transform.pivot = GUILocation::BOTTOM_RIGHT;

	lifebar->pointColor[GUI::PointColor::GREEN] = {0,0,5,12};
	lifebar->pointColor[GUI::PointColor::YELLOW] = {5,0,5,12};
	lifebar->pointColor[GUI::PointColor::RED] = {10,0,5,12};
	lifebar->pointColor[GUI::PointColor::EMPTY] = {15,0,5,12};
	container->pack(lifebar);

	//score
	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	GUI::GUIScore* scoreText = new GUI::GUIScore(&score->score, color, "arcadepix.ttf", GUILocation::TOP_RIGHT, 8);
	scoreText->transform.pivot = GUILocation::TOP_RIGHT;
	scoreText->offset.y = 2 ;
	container->pack(scoreText);
	
	HUD = container;
}

GUI::GUIComponent* ModulePlayer::portraitAnimation() {
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
	return animationPortrait;
}


// Update
update_status ModulePlayer::update(){
	debugging();

	levelEnd();

	//check the input and move or do things accordingly
	checkInput();

	//do things about the death of the player
	dead();
	//update the entity player after setting all
	player->update();

	//check if the time since he's dead is over and change the scene
	if (!gameOverTimer->isStopped() && gameOverTimer->value() > 10000) {
		Scene* next = new EntryScene();
		App->scene->changeScene(next, 1);
		gameOverTimer->stop();
		App->audio->stopMusic(); //stop music
	}
	return UPDATE_CONTINUE;
}

void ModulePlayer::levelEnd() {
	ControlEntity* controller = &player->controller;

	//finished level and the countdown is over, change the scene
	if (levelFinished && !gameOverTimer->isStopped() && gameOverTimer->value() > 3000) {
		Scene* next = new EntryScene();
		App->scene->changeScene(next, 1);
		gameOverTimer->stop();
		App->audio->stopMusic(); //stop music

	//level finished, start the countdown
	} else if (levelFinished && gameOverTimer->isStopped()) {
		controller->moveX = 0;
		controller->moveY = 0;
		controller->attack = 0;
		controller->damage = 0;
		gameOverTimer->start();
		//put the animation
	}
}

void ModulePlayer::checkInput() {
	ControlEntity* controller = &player->controller;
	Transform* trans = player->transform;

	//don't move while receiving damage
	if (controller->damage != 0 || levelFinished) {
		return;
	}

	controller->moveX = 0;
	controller->moveY = 0;

	//don't move while attacking
	if (controller->attack != 0) {
		//but! only if is not double jumping
		if (controller->stateJump != TypeJump::DOUBLE_JUMP) {
			//attack setted! get the animation and wait until its finished
			//this works because between attack setted and this line of code, is an entire update cicle (only one!)
			//so does not matter if animation component was after or before, it had time to update and
			//set the attack animation
			if (attackAnimation == nullptr) {
				//set animation
				attackAnimation = animation->getActualAnimation();
			}
		}
	}
	controller->attack = 0;


	//don't move while attacking except if is jumping
	if (attackAnimation != nullptr && (!attackAnimation->isFinished()) && controller->stateJump==TypeJump::NONE) {
		motion->velocity.x = 0;
		return;

	//animation attack finished? clear the animation
	} else if(attackAnimation!=nullptr && attackAnimation->isFinished()){
		attackAnimation = nullptr;
	}
	

	//movement
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

	//weapon
	if (App->input->getKey(SDL_SCANCODE_L) == KEY_DOWN) {
		weapon->throwWeapon();
	}

	//don't jump again when jumping or falling
	if (App->input->getKey(SDL_SCANCODE_J) == KEY_DOWN && controller->stateJump == TypeJump::NONE) {
		if (App->input->getKey(SDL_SCANCODE_W)) {
			controller->stateJump = TypeJump::DOUBLE_JUMP;

		} else if (App->input->getKey(SDL_SCANCODE_S)) {
			controller->stateJump = TypeJump::JUMP_DOWN;

		} else {
			controller->stateJump = TypeJump::JUMP;
		}
	}

	//if the player is crouched down, dont move horizontally
	if (controller->moveY != 1) {
		motion->velocity.x = controller->moveX * motion->speed;
	} else {
		motion->velocity.x = 0.0f;
	}
}

update_status ModulePlayer::postUpdate() {
	if (deadBody != nullptr) {
		deadBody->postUpdate();
	}
	player->postUpdate();
	return UPDATE_CONTINUE;
}