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
	position.y = 216;

	// idle animation (arcade sprite sheet)
	idle.frames.push_back({7, 14, 60, 90});
	idle.frames.push_back({95, 15, 60, 89});
	idle.frames.push_back({184, 14, 60, 90});
	idle.frames.push_back({276, 11, 60, 93});
	idle.frames.push_back({366, 12, 60, 92});
	idle.speed = 0.1f;
	
	// walk backward animation (arcade sprite sheet)
	backward.frames.push_back({542, 131, 61, 87});
	backward.frames.push_back({628, 129, 59, 90});
	backward.frames.push_back({713, 128, 57, 90});
	backward.frames.push_back({797, 127, 57, 90});
	backward.frames.push_back({883, 128, 58, 91});
	backward.frames.push_back({974, 129, 57, 89});
	backward.speed = 0.1f;

	// TODO 8: setup the walk forward animation from ryu4.png
	forward.frames.push_back({9, 128, 68, 92});//77 220
	forward.frames.push_back({77, 128, 68, 92});//86
	forward.frames.push_back({163, 128, 68, 92});//85
	forward.frames.push_back({259, 128, 68, 92}); //84
	forward.frames.push_back({352, 128, 68, 92}); //86
	forward.frames.push_back({432, 128, 68, 92});//91
	forward.speed = 0.08f;
}

ModulePlayer::~ModulePlayer()
{
	// Homework : check for memory leaks
}

// Load assets
bool ModulePlayer::start(){
	LOG("Loading player");

	graphics = App->textures->load("ryu4.png"); // arcade version
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

	switch (movement) {
		case 0:
			actual = &idle;
			break;
		case 1:
			actual = &forward;
			break;
		case -1:
			actual = &backward;
			break;
	}

	// position while cycling the animation(check Animation.h)
	App->renderer->blit(graphics, position.x, position.y - actual->GetCurrentFrame().h, &(actual->GetCurrentFrame()), 1.0f);

	return UPDATE_CONTINUE;
}