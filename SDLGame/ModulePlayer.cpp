#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/SDL.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA
ModulePlayer::ModulePlayer(bool start_enabled) : Module(start_enabled)
{
	position.x = 100;
	position.y = 236;

	// idle animation (arcade sprite sheet)
	idle.frames.push_back({0, 102, 42, 48});
	idle.frames.push_back({42, 102, 42, 48});
	idle.frames.push_back({84, 102, 42, 48});
	idle.frames.push_back({126, 102, 42, 48});
	idle.speed = 0.05f;

}

ModulePlayer::~ModulePlayer()
{
	// Homework : check for memory leaks
}

// Load assets
bool ModulePlayer::start(){
	LOG("Loading player");

	graphics = App->textures->load("joe.png"); // arcade version
	actual = &idle;
	return true;
}

// Unload assets
bool ModulePlayer::cleanUp(){
	LOG("Unloading player");

	App->textures->unload(graphics);

	return true;
}

// Update
update_status ModulePlayer::update(){
	// TODO 9: Draw the player with its animation
	// make sure to detect player movement and change its
	
	int movement = 0;
	if (App->input->GetKey(SDL_SCANCODE_D)) {
 		movement = 1;
	} else if (App->input->GetKey(SDL_SCANCODE_A)) {
		movement = -1;
	}
	position.x += movement;

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

	return UPDATE_CONTINUE;
}

update_status ModulePlayer::postUpdate() {
	// position while cycling the animation(check Animation.h)
	iPoint pos = position;
	pos.y -= actual->GetCurrentFrame().h;
	App->renderer->blit(graphics, pos, &(actual->GetCurrentFrame()));
	return UPDATE_CONTINUE;
}