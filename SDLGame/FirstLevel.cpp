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

	iPoint rectCollider = {1088, 20};
	RectangleCollider* collider = new RectangleCollider(fPoint(0, 226), rectCollider, 0, TypeCollider::GROUND);
	CollisionComponent* colliderComponent = new CollisionComponent("ground", collider);
	buffer->addComponent(colliderComponent);

	rectCollider = {20,256};
	collider = new RectangleCollider(fPoint(-10, 0), rectCollider, 0, TypeCollider::WALL);
	colliderComponent = new CollisionComponent("leftLateral", collider);
	buffer->addComponent(colliderComponent);

	rectCollider = {20,256};
	collider = new RectangleCollider(fPoint(1080, 0), rectCollider, 0, TypeCollider::WALL);
	colliderComponent = new CollisionComponent("rightLateral", collider);
	buffer->addComponent(colliderComponent);


	//adding collisions tests
	Entity* collisions = new Entity();
	
	RectangleCollider* rectangle = new RectangleCollider(fPoint(50, 100), iPoint(28,47), 0, TypeCollider::NONE_COLLIDER);
	CollisionComponent* collider2 = new CollisionComponent("collider", rectangle);
	collisions->addComponent(collider2);

	CircleCollider* circle = new CircleCollider(fPoint(170, 190), 5, TypeCollider::NONE_COLLIDER);
	collider2 = new CollisionComponent("collider2", circle);
	collisions->addComponent(collider2);

	
	root->addChild(buffer);
	root->addChild(collisions);
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
