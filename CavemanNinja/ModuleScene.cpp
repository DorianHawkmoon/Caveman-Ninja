#include "ModuleScene.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "EntryScene.h"
#include "GUIComponent.h"

ModuleScene::ModuleScene(bool started):Module(started), currentScene(false), nextScene(nullptr) {}


ModuleScene::~ModuleScene() {
	delete currentScene;
}

bool ModuleScene::start() {
	SDL_SetRenderDrawBlendMode(App->renderer->renderer, SDL_BLENDMODE_BLEND);
	currentScene = new EntryScene();
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

bool ModuleScene::cleanUp() {
	//SDL_SetRenderDrawBlendMode(App->renderer->renderer, SDL_BLENDMODE_NONE);
	return currentScene->cleanUp();
}

void ModuleScene::changeScene(Scene * scene, float time) {
	LOG("Scene change");
	nextScene = scene;
	App->input->disable();
	fadingIn = (currentScene != nullptr) ? true : false;
	startTime = SDL_GetTicks();
	totalTime = (Uint32) (time  * 0.5f * 1000.0f);
}

void ModuleScene::addEntity(Entity * entity) {
	currentScene->addNode(entity);
}

void ModuleScene::addGUI(GUI::GUIComponent * gui) {
	currentScene->addGUI(gui);
}

const Scene * ModuleScene::getCurrentScene() const {
	return currentScene;
}

void ModuleScene::makeChangeScene() {
	if (startTime > 0) {
		Uint32 now = SDL_GetTicks() - startTime;
		float normalized = (float) now / (float) totalTime;

		if (normalized > 1.0f) {
			normalized = 1.0f;
		}

		if (fadingIn == false) {
			normalized = 1.0f - normalized;
		}

		// Draw a screen-size balck rectangle with alpha
		App->renderer->setFaddingEffect(normalized);

		if (now >= totalTime) {
			if (fadingIn == true) {
				// Elimina la escena anterior (de haberla)
				if (currentScene != nullptr) {
					//clean the scene and delete it
					LOG("Cleaning scene");
					currentScene->cleanUp();
					delete currentScene;
				}

				// Realiza el cambio de escena
				currentScene = nextScene;
				nextScene = nullptr;
				currentScene->start();

				totalTime += totalTime;
				startTime = SDL_GetTicks();
				fadingIn = false;
			} else {
				startTime = 0;
				App->renderer->clearFaddingEffect();
				App->input->enable();
			}
		}
	}
}
