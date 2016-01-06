#include "ModuleEnemy.h"
#include "Entity.h"
#include "MotionComponent.h"
#include "ModulePlayer.h"
#include "Application.h"
#include <cmath>

ModuleEnemy::ModuleEnemy(bool start_enabled) : Module(start_enabled) {
	
}

ModuleEnemy::~ModuleEnemy() {
	// Homework : check for memory leaks
	enemies.clear();
}

void ModuleEnemy::registerEnemy(Entity * enemy) {
	if (find(enemies.begin(), enemies.end(), enemy)==enemies.end()) {
		enemies.push_back(enemy);
	}
}

void ModuleEnemy::unregisterEnemy(Entity * enemy) {
	auto found = find(enemies.begin(), enemies.end(), enemy);

	if (found != enemies.end()) {
		enemies.erase(found);
	}
}

// Load assets
bool ModuleEnemy::start() {
	return true;
}

update_status ModuleEnemy::preUpdate() {
	return UPDATE_CONTINUE;
}

// Unload assets
bool ModuleEnemy::cleanUp() {
	LOG("Unloading enemies");

	return true;
}

// Update
update_status ModuleEnemy::update() {
	for (auto it = enemies.begin(); it != enemies.end(); ++it) {
		ControlEntity* controller = &(*it)->controller;
		Transform* trans = (*it)->transform;
		Transform globalEnemy = trans->getGlobalTransform();
		MotionComponent* motion = static_cast<MotionComponent*>((*it)->getComponent("motion"));

		controller->moveX = 0;
		controller->moveY = 0;
		controller->attack = 0;

		Transform player = App->player->player->transform->getGlobalTransform();
		float distance = abs(player.position.x - globalEnemy.position.x);
		if (player.position.x < globalEnemy.position.x) {
			controller->moveX -= 1;
			trans->flip = SDL_FLIP_NONE;
			if (distance > 150) {
				controller->run = true;
			}
		}

		if (player.position.x > globalEnemy.position.x) {
			controller->moveX += 1;
			trans->flip = SDL_FLIP_HORIZONTAL;
			if (distance > 80) {
				controller->run=true;
			}
		}

		/*if (App->input->getKey(SDL_SCANCODE_W)) {
			controller->moveY -= 1;
		}

		if (App->input->getKey(SDL_SCANCODE_S)) {
			controller->moveY += 1;
		}*/



		//don't jump again when jumping or falling
		/*if (App->input->getKey(SDL_SCANCODE_KP_0) && controller->stateJump == JumpType::NONE) {
			if (App->input->getKey(SDL_SCANCODE_W)) {
				controller->stateJump = JumpType::DOUBLE_JUMP;
			} else {
				controller->stateJump = JumpType::JUMP;
			}
		}*/

		if (controller->moveY != 1) {
			motion->velocity.x = (controller->run)? (controller->moveX + 0.7) *motion->speed : controller->moveX * motion->speed;
		} else {
			motion->velocity.x = 0.0f;
		}

	}
	return UPDATE_CONTINUE;
}

update_status ModuleEnemy::postUpdate() {
	return UPDATE_CONTINUE;
}