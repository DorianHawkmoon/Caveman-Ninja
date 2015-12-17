#include "SceneKen.h"

#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "Entity.h"
SceneKen::SceneKen() {
	// ground
	ground.x = 8;
	ground.y = 391;
	ground.w = 896;
	ground.h = 72;

	// setup the foreground (red ship) with
	// coordinates x,y,w,h from ken_stage.png
	ship.frames.push_back({9, 25, 522, 181});
	ship.frames.push_back({9, 26, 522, 181});
	ship.frames.push_back({9, 27, 522, 181});
	ship.frames.push_back({9, 28, 522, 181});
	ship.frames.push_back({9, 27, 522, 181});
	ship.frames.push_back({9, 26, 522, 181});
	ship.frames.push_back({9, 25, 522, 181});
	ship.speed = 0.05f;

	// Background / sky
	background.x = 72;
	background.y = 208;
	background.w = 768;
	background.h = 176;

	// flag animation
	flag.frames.push_back({848, 208, 40, 40});
	flag.frames.push_back({848, 256, 40, 40});
	flag.frames.push_back({848, 304, 40, 40});
	flag.speed = 0.08f;

	//Setup Girl Animation from coordinates from ken_stage.png
	girl.frames.push_back({624, 16, 32, 52});
	girl.frames.push_back({624, 80, 32, 52});
	girl.frames.push_back({624, 144, 32, 52});
	girl.frames.push_back({624, 80, 32, 52});
	girl.speed = 0.08f;

	
}


SceneKen::~SceneKen() {}


// Load assets
bool SceneKen::start() {
	LOG("Loading ken scene");

	graphics = App->textures->load("ken_stage.png");

	// Enable the player module
	App->player->enable();
	App->input->enable();
	//App->audio->playMusic("ken.ogg");

	return true;
}

// UnLoad assets
bool SceneKen::cleanUp() {
	LOG("Unloading ken scene");

	App->textures->unload(graphics);
	App->player->disable();

	return true;
}

// Update: draw background
bool SceneKen::update() {
	// Draw everything --------------------------------------
	App->renderer->blit(graphics, 0, 0, &background, 0.77f); // sea and sky
	App->renderer->blit(graphics, 560, 8, &(flag.GetCurrentFrame()), 0.77f); // flag animation

	App->renderer->blit(graphics, 0, 0, &(ship.GetCurrentFrame()), 0.77f);

	//Draw the girl. Make sure it follows the ship movement!
	App->renderer->blit(graphics, 191, 103 - (ship.GetCurrentFrame().y - 25), &(girl.GetCurrentFrame()), 0.77f);

	App->renderer->blit(graphics, 0, 170, &ground);

	//  Make that pressing space triggers a switch to honda logic module
	// using FadeToBlack module
	/*if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		LOG("Unloading ken scene and loading scene honda");
		App->input->disable();
		App->fade->fadeToBlack(App->scene_honda, this, 3.0f);
	}*/

	return UPDATE_CONTINUE;
}