#include "EntryScene.h"

#include "SceneNode.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleRender.h"
#include "SpriteComponent.h"
#include "Entity.h"
#include "ModuleInput.h"
#include "FirstLevel.h"
#include "GUILabel.h"
#include "ModuleGUI.h"
#include "GUIContainer.h"
#include "GUIComponent.h"

EntryScene::EntryScene() {}


EntryScene::~EntryScene() {}

bool EntryScene::start() {
	LOG("Loading title","");
	App->renderer->camera.leftLimit = {0,0};
	App->renderer->camera.rightLimit = {1088,256};
	App->renderer->resetCamera();

	Entity* buffer = new Entity();
	SpriteComponent* background = new SpriteComponent("title", "title.png");
	background->rect.x = 0;
	background->rect.y = 0;
	background->rect.w = 256;
	background->rect.h = 256;
	background->speedCamera = 0.3f;
	buffer->addComponent(background);

	root->addChild(buffer);

	buffer = new Entity();
	root->addChild(buffer);
	root->start();

	SDL_Color color = SDL_Color();
	color.g = 255;
	color.b = 255;
	color.r = 255;
	color.a = 255;
	GUI::GUILabel* label = new GUI::GUILabel("Press SPACE to start", color, "arcadepix.ttf", GUILocation::ABSOLUTE);
	label->transform.position = {35,190};

	rootGUI->pack(label);
	rootGUI->start();

	App->gui->registerGUI(rootGUI);
	return true;
}

update_status EntryScene::update() {
	if (root != nullptr) {
		root->update();
	}


	if (App->input->keyActive(SDL_SCANCODE_RETURN) || App->input->keyActive(SDL_SCANCODE_SPACE)) {
		Scene* next = new FirstLevel();
		App->scene->changeScene(next, 1);
	}

	return UPDATE_CONTINUE;
}
