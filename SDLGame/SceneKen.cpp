#include "SceneKen.h"

#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "Entity.h"
#include "SpriteComponent.h"

SceneKen::SceneKen() {
	
}


SceneKen::~SceneKen() {}


// Load assets
bool SceneKen::start() {
	LOG("Loading ken scene");

	player = new Entity(Category::PLAYER);
	SpriteComponent* sprite = new SpriteComponent("render", "joe.png");
	sprite->rect.x = 0;
	sprite->rect.y = 0;
	sprite->rect.w = 28;
	sprite->rect.h = 47;
	
	player->addComponent(sprite);

	//TODO el entity del player quedará dentro del módulo player para mantenerlo entre niveles
	player->start();
	return true;
}

// UnLoad assets
bool SceneKen::cleanUp() {
	LOG("Unloading ken scene");

	player->cleanUp();

	return true;
}

// Update: draw background
bool SceneKen::update() {
	// Draw everything --------------------------------------
	//TODO move pre and post in their correspondent methods
	player->preUpdate();
	player->update();
	player->postUpdate();
	return UPDATE_CONTINUE;
}