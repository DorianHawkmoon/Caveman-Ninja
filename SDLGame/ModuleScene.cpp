#include "ModuleScene.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"

#include "SceneKen.h"

ModuleScene::ModuleScene(bool started):Module(started) {}


ModuleScene::~ModuleScene() {}

bool ModuleScene::start() {
	currentScene = new SceneKen();
	currentScene->start();
	return true;
}

update_status ModuleScene::preUpdate() {
	if (!currentScene->preUpdate()) {
		return UPDATE_ERROR;
	}
	return UPDATE_CONTINUE;
}

update_status ModuleScene::update() {
	if (!currentScene->update()) {
		return UPDATE_ERROR;
	}
	return UPDATE_CONTINUE;
}

update_status ModuleScene::postUpdate() {
	if (!currentScene->postUpdate()) {
		return UPDATE_ERROR;
	}

	makeChangeScene();
	return UPDATE_CONTINUE;
}

void ModuleScene::changeScene(Scene * scene) {
	LOG("Scene change");
	nextScene = scene;
	
}

void ModuleScene::makeChangeScene() {
	if (nextScene != nullptr) {
		LOG("Change scene");
		// Elimina la escena anterior (de haberla)
		if (currentScene != nullptr) {
			currentScene->cleanUp();
			delete currentScene;
		}

		// Realiza el cambio de escena
		currentScene = nextScene;
		nextScene = nullptr;
		currentScene->start();
	}
}
