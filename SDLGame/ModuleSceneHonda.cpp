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
	background.x = 216;
	background.y = 176;
	background.w = 544;
	background.h = 130;

	lateral.x = 120;
	lateral.y = 128;
	lateral.w = 96;
	lateral.h = 199;

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
	
	//App->renderer->blit(graphics, 560, 8, &(flag.GetCurrentFrame()), 0.77f); // flag animation

	//App->renderer->blit(graphics, 0, 0, &(ship.GetCurrentFrame()), 0.77f);

	//Draw the girl. Make sure it follows the ship movement!
	//App->renderer->blit(graphics, 191, 103 - (ship.GetCurrentFrame().y - 25), &(girl.GetCurrentFrame()), 0.77f);

	
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
