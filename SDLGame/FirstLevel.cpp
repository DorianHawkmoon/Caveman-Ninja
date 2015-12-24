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
	background->rect.w = 256;
	background->rect.h = 256;
	background->speedCamera = 0.3f;
	buffer->addComponent(background);

	//TODO arreglar los dos prixeles transparentes de mountain
	background = new HorizontalSpriteScrollComponent("mountain", "montanas.png");
	background->rect.x = 0;
	background->rect.y = 0;
	background->rect.w = 256;
	background->rect.h = 256;
	background->speedCamera = 0.4f;
	buffer->addComponent(background);

	background = new HorizontalSpriteScrollComponent("trees", "trees.png");
	background->rect.x = 0;
	background->rect.y = 0;
	background->rect.w = 384;
	background->rect.h = 208;
	background->speedCamera = 0.5f;
	buffer->addComponent(background);

	SpriteComponent* sprite = new SpriteComponent("level", "firstLevel.png");
	sprite->rect.x = 0;
	sprite->rect.y = 0;
	sprite->rect.w = 1088;
	sprite->rect.h = 256;
	sprite->speedCamera = 1;
	buffer->addComponent(sprite);

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
