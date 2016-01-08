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

ModulePlayer::ModulePlayer(bool start_enabled) : Module(start_enabled)
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
	player->start();
	started = started & ((motion = static_cast<MotionComponent*>(player->getComponent("motion")))!=nullptr);
	started = started & ((jump = static_cast<JumpComponent*>(player->getComponent("jump"))) != nullptr);
	started = started & ((life = static_cast<LifeComponent*>(player->getComponent("life"))) != nullptr);
	started = started & ((weapon = static_cast<WeaponComponent*>(player->getComponent("weapon"))) != nullptr);
	player->transform->position = {100, 170};

	App->renderer->camera.setCamera(player->transform);
	App->renderer->camera.offset.x = 30;

	return started;
}

update_status ModulePlayer::preUpdate() {
	player->preUpdate();
	return UPDATE_CONTINUE;
}

// Unload assets
bool ModulePlayer::cleanUp(){
	LOG("Unloading player");
	player->cleanUp();
	return true;
}


// Update
update_status ModulePlayer::update(){
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

		if (App->input->getKey(SDL_SCANCODE_W)==KEY_DOWN) {
			controller->moveY -= 1;
		}

		if (App->input->getKey(SDL_SCANCODE_S)) {
			controller->moveY += 1;
		}

		if (App->input->getKey(SDL_SCANCODE_KP_ENTER)) {
			weapon->throwWeapon();
		}



		//don't jump again when jumping or falling
		if (App->input->getKey(SDL_SCANCODE_KP_0) && controller->stateJump == JumpType::NONE) {
			if (App->input->getKey(SDL_SCANCODE_W)) {
				controller->stateJump = JumpType::DOUBLE_JUMP;
			} else {
				controller->stateJump = JumpType::JUMP;
			}
		}

		if (controller->moveY != 1) {
			motion->velocity.x = controller->moveX * motion->speed;
		} else {
			motion->velocity.x = 0.0f;
		}
	}

	player->update();
	return UPDATE_CONTINUE;
}

update_status ModulePlayer::postUpdate() {
	player->postUpdate();
	return UPDATE_CONTINUE;
}