#include "EntryScene.h"

#include "SceneNode.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleRender.h"
#include "SpriteComponent.h"
#include "Entity.h"
#include "ModuleInput.h"
#include "FirstLevel.h"

EntryScene::EntryScene() {}


EntryScene::~EntryScene() {}

bool EntryScene::start() {
	LOG("Loading title");
	App->renderer->camera.leftLimit = {0,0};
	App->renderer->camera.rightLimit = {1088,256};

	Entity* buffer = new Entity(Category::NONE_CATEGORY);
	SpriteComponent* background = new SpriteComponent("title", "title.png");
	background->rect.x = 0;
	background->rect.y = 0;
	background->rect.w = 256;
	background->rect.h = 256;
	background->speedCamera = 0.3f;
	buffer->addComponent(background);

	root->addChild(buffer);
	root->start();
	return true;
}

update_status EntryScene::update() {
	if (root != nullptr) {
		root->update();
	}

	//TODO text blink, need a text module for fonts

	if (App->input->keyActive(SDL_SCANCODE_RETURN) || App->input->keyActive(SDL_SCANCODE_SPACE)) {
		Scene* next = new FirstLevel();
		App->scene->changeScene(next, 1);
	}

	return UPDATE_CONTINUE;
}
