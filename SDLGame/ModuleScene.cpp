#include "ModuleScene.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"

ModuleScene::ModuleScene() {}


ModuleScene::~ModuleScene() {}

bool ModuleScene::Start() {
	img = App->textures->Load("sprites.png");
	music = App->audio->LoadMusic("Dark-Amb.mp3");
	Mix_PlayMusic(music, -1);
	return true;
}

update_status ModuleScene::Update() {
	App->renderer->Blit(img, 0, 0, nullptr);
	if (Mix_PlayingMusic() == 0) {
		//Play the music
		Mix_PlayMusic(music, -1);
	}
	return UPDATE_CONTINUE;
}