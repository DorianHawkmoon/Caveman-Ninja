#include "Globals.h"
#include "ModulePlayer.h"
#include "Entity.h"
#include "Joe.h"
#include "MotionComponent.h"

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

	player->transform.position.x = 100;
	player->transform.position.y = 256 - 15 * SCREEN_SIZE;

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
	//motion

	// TODO 9: Draw the player with its animation
	// make sure to detect player movement and change its
	//int movement = 0;
	//if (App->input->GetKey(SDL_SCANCODE_D)) {
 	//	movement = 1;
	//} else if (App->input->GetKey(SDL_SCANCODE_A)) {
	//	movement = -1;
	//}
	//position.x += movement;

	/*switch (movement) {
		case 0:
			actual = &idle;
			break;
		case 1:
			actual = &forward;
			break;
		case -1:
			actual = &backward;
			break;
	}*/

	//animations->proccessState();
	
	player->update();

	return UPDATE_CONTINUE;
}

update_status ModulePlayer::postUpdate() {
	player->postUpdate();
	return UPDATE_CONTINUE;
}