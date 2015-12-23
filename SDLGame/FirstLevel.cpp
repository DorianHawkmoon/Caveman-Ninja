#include "FirstLevel.h"

#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "Entity.h"
#include "SceneNode.h"

#include "SpriteComponent.h"
#include "HorizontalSpriteScrollComponent.h"

FirstLevel::FirstLevel() {
	
}


FirstLevel::~FirstLevel() {}


// Load assets
bool FirstLevel::start() {
	LOG("Loading scene");
	App->player->enable();

	Entity* buffer = new Entity(Category::NONE_CATEGORY);
	HorizontalSpriteScrollComponent* background = new HorizontalSpriteScrollComponent("sky", "cielo.png");
	background->rect.x = 0;
	background->rect.y = 0;
	background->rect.w = 242;
	background->rect.h = 224;
	background->speedCamera = 0.2f;
	buffer->addComponent(background);

	//TODO arreglar los dos prixeles transparentes de mountain
	background = new HorizontalSpriteScrollComponent("mountain", "montanas.png");
	background->rect.x = 0;
	background->rect.y = 0;
	background->rect.w = 258;
	background->rect.h = 224;
	background->speedCamera = 0.3f;
	buffer->addComponent(background);

	root->addChild(buffer);

	root->start();
	return true;
}

update_status FirstLevel::preUpdate() {
	root->preUpdate();
	return UPDATE_CONTINUE;
}

// UnLoad assets
bool FirstLevel::cleanUp() {
	LOG("Unloading first scene");
	root->cleanUp();
	return true;
}

// Update: draw background
update_status FirstLevel::update() {
	root->update();
	return UPDATE_CONTINUE;
}

update_status FirstLevel::postUpdate() {
	root->postUpdate();
	root->removeWrecks();
	return UPDATE_CONTINUE;
}
