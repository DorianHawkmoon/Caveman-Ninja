#include "ModuleSceneHonda.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "SDL/SDL.h"


ModuleSceneHonda::ModuleSceneHonda(bool started):Module(started) {
	// ground
	ground.x = 8;
	ground.y = 376;
	ground.w = 848;
	ground.h = 64;

	sky.x = 91;
	sky.y = 8;
	sky.w = 765;
	sky.h = 48;


	// Background / sky
	background.x = 216;
	background.y = 176;
	background.w = 544;
	background.h = 130;

	lateral.x = 120;
	lateral.y = 128;
	lateral.w = 96;
	lateral.h = 199;
}


ModuleSceneHonda::~ModuleSceneHonda() {}

bool ModuleSceneHonda::start() {
	LOG("Loading honda scene");

	graphics = App->textures->load("honda_stage.png");

	// Enable the player module
	App->player->enable();
	//App->audio->playMusic("honda.ogg");

	return true;
}

update_status ModuleSceneHonda::update() {
	// Draw everything --------------------------------------

	App->renderer->blit(graphics, -75, 176, &ground, 1.1f);
	App->renderer->blit(graphics, -7, 2, &lateral);
	App->renderer->blit(graphics, -58, 0, &sky,1.1f);
	App->renderer->blit(graphics, 89, 50, &background); // sea and sky

	
	

	return UPDATE_CONTINUE;
}

bool ModuleSceneHonda::cleanUp() {
	LOG("Unloading honda scene");

	App->textures->unload(graphics);
	App->player->disable();

	return true;
}
