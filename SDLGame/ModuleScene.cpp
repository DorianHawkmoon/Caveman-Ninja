#include "ModuleScene.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"

ModuleScene::ModuleScene(bool started):Module(started) {}


ModuleScene::~ModuleScene() {}

bool ModuleScene::start() {
	return true;
}

update_status ModuleScene::preUpdate() {
	return update_status();
}

update_status ModuleScene::update() {
	return UPDATE_CONTINUE;
}

update_status ModuleScene::postUpdate() {
	return update_status();
}
