#include "ModuleScene.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"

#include "SceneKen.h"

ModuleScene::ModuleScene(bool started):Module(started) {}


ModuleScene::~ModuleScene() {}

bool ModuleScene::start() {
	scene = new SceneKen();
	scene->start();
	return true;
}

update_status ModuleScene::preUpdate() {
	if (!scene->preUpdate()) {
		return UPDATE_ERROR;
	}
	return UPDATE_CONTINUE;
}

update_status ModuleScene::update() {
	if (!scene->update()) {
		return UPDATE_ERROR;
	}
	return UPDATE_CONTINUE;
}

update_status ModuleScene::postUpdate() {
	if (!scene->postUpdate()) {
		return UPDATE_ERROR;
	}
	return UPDATE_CONTINUE;
}
