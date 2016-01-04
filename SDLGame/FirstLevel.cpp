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
#include "CollisionComponent.h"
#include "CircleCollider.h"
#include "Point.h"
#include "RectangleCollider.h"

FirstLevel::FirstLevel() {
	
}


FirstLevel::~FirstLevel() {
}


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

	fPoint rectGround = {1088, 10};
	RectangleCollider* groundCollider = new RectangleCollider(fPoint(0, 50), rectGround, TypeCollider::WALL);
	CollisionComponent* ground = new CollisionComponent("ground", groundCollider);
	buffer->addComponent(ground);

	Entity* enti = new Entity();
	enti->transform->position.x = 100;
	SpriteComponent* sprite2 = new SpriteComponent("uu","baseFirstLevel.png");
	sprite2->rect.x = 0;
	sprite2->rect.y = 0;
	sprite2->rect.w = 256;
	sprite2->rect.h = 256;
	enti->addComponent(sprite2);
	
	SceneNode* child = new SceneNode(enti);
	SceneNode* parent = new SceneNode(buffer);

	buffer->transform->position.x = 50;
	
	parent->addChild(child);
	//child->addChild(buffer);
	root->addChild(parent);
	//root->addChild(child);
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
	return UPDATE_CONTINUE;
}
