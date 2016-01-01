#include "Globals.h"
#include "ModulePlayer.h"
#include "Entity.h"
#include "Joe.h"
#include "MotionComponent.h"

#include "Application.h"
#include "ModuleInput.h"
#include "SDL\SDL_keycode.h"
#include "JumpComponent.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA
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
	bool started=player->start();
	started = started & ((motion = static_cast<MotionComponent*>(player->getComponent("motion")))!=nullptr);
	started = started & ((jump = static_cast<JumpComponent*>(player->getComponent("jump"))) != nullptr);
	player->transform.position.x = 100;
	player->transform.position.y = 150;
	player->transform.speed = 50;

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
	// TODO 9: Draw the player with its animation
	// make sure to detect player movement and change its
	motion->velocity.setToZero();
	ControlEntity* controller = &player->controller;
	Transform* trans = &player->transform;
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

	//don't jump again when jumping or falling
	if (App->input->getKey(SDL_SCANCODE_KP_0) && controller->stateJump==JumpType::NONE) {
			controller->stateJump = JumpType::JUMP;
	}

	if (controller->moveY != 1) {
		motion->velocity.x = controller->moveX * player->transform.speed;
	} else {
		motion->velocity.x = 0.0f;
	}


	player->update();
	return UPDATE_CONTINUE;
}

update_status ModulePlayer::postUpdate() {
	player->postUpdate();
	return UPDATE_CONTINUE;
}