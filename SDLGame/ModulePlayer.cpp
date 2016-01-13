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
#include "Animation.h"
#include "SpriteComponent.h"

ModulePlayer::ModulePlayer(bool start_enabled) : Module(start_enabled), lifes(1)
{
	player = Joe::makeJoe();
}

ModulePlayer::~ModulePlayer()
{
	// Homework : check for memory leaks
	delete player;
}

// Load assets
bool ModulePlayer::start(){
	LOG("Loading player");
	bool started = true;

	if (lifes <= 0) {
		delete player;
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
	player->transform->position = {50, 0};
	player->controller.stateJump = TypeJump::FALL;

	//restore his life
	life->setActualLife(life->getMaxLife());
	

	gameOverTimer.stop();

	App->renderer->camera.setCamera(player->transform);
	App->renderer->camera.offset.x = 30;

	soundDie = App->audio->loadEffect("player_die.wav");

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
	player->cleanUp();
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
			gameOverTimer.start();
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


// Update
update_status ModulePlayer::update(){
	debugging();

	ControlEntity* controller = &player->controller;
	Transform* trans = player->transform;

	//don't move while receiving damage
	if (controller->damage == 0) {
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

		if (App->input->getKey(SDL_SCANCODE_KP_ENTER)==KEY_DOWN) {
			weapon->throwWeapon();
		}



		//don't jump again when jumping or falling
		if (App->input->getKey(SDL_SCANCODE_KP_0)== KEY_DOWN && controller->stateJump == TypeJump::NONE) {
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

	if (!gameOverTimer.isStopped() && gameOverTimer.value() > 10000) {
		Scene* next = new EntryScene();
		App->scene->changeScene(next, 1);
		gameOverTimer.stop();
		//App->audio->stop //stop music
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