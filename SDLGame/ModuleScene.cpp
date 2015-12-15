#include "ModuleScene.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"

ModuleScene::ModuleScene(bool started):Module(started) {}


ModuleScene::~ModuleScene() {}

bool ModuleScene::start() {
	img = App->textures->load("sprites.png");
	//music = App->audio->loadMusic("Dark-Amb.mp3");
	Mix_PlayMusic(music, -1);
	return true;
}

update_status ModuleScene::update() {
	App->renderer->blit(img, 0, 0, nullptr);
	if (Mix_PlayingMusic() == 0) {
		//Play the music
		Mix_PlayMusic(music, -1);
	}
	return UPDATE_CONTINUE;
}