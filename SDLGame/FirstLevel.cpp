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
#include "LineCollider.h"
#include <vector>

#include "Transform.h"
#include "Enemy.h"

FirstLevel::FirstLevel() {
	
}


FirstLevel::~FirstLevel() {
}

// Load assets
bool FirstLevel::start() {
	LOG("Loading scene");
	App->player->enable();

	App->renderer->camera.leftLimit = {0,0};
	App->renderer->camera.rightLimit = {1088,256};

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
	RectangleCollider* collider = new RectangleCollider(fPoint(0, 226), rectCollider, 0, TypeCollider::FLOOR);
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

	LineCollider* line = new LineCollider(fPoint(0,0), std::vector<fPoint>{fPoint(290, 211),
												fPoint(340, 195),
												fPoint(443, 149),
												fPoint(487, 113),
												fPoint(512, 94),
												fPoint(544, 83),
												fPoint(578, 83),
												fPoint(616, 98),
												fPoint(664, 136),
												fPoint(700, 150),
												fPoint(708, 146),
												fPoint(741, 148),
												fPoint(756, 164),
												fPoint(781, 180),
												fPoint(798, 191)}, TypeCollider::GROUND);
	line->thickness=1;
	colliderComponent = new CollisionComponent("lomo", line);
	buffer->addComponent(colliderComponent);

	/*rectCollider = {50,15};
	collider = new RectangleCollider(fPoint(512, 125), rectCollider, -30, TypeCollider::GROUND);
	colliderComponent = new CollisionComponent("pata", collider);
	buffer->addComponent(colliderComponent);
	rectCollider = {25,15};
	collider = new RectangleCollider(fPoint(550, 115), rectCollider, 0, TypeCollider::GROUND);
	colliderComponent = new CollisionComponent("pata2", collider);
	buffer->addComponent(colliderComponent);
	rectCollider = {40,15};*/
	/*
	fPoint(520, 129),
	fPoint(550, 115),
	fPoint(562, 115),
	fPoint(576, 124),
	fPoint(587, 145)
	*/
	/*collider = new RectangleCollider(fPoint(560, 120), rectCollider, 45, TypeCollider::GROUND);
	colliderComponent = new CollisionComponent("pata3", collider);
	buffer->addComponent(colliderComponent);*/

	//adding collisions tests
	/*Entity* collisions = new Entity();
	
	RectangleCollider* rectangle = new RectangleCollider(fPoint(100, 100), iPoint(28,47), 45, TypeCollider::NONE_COLLIDER);
	CollisionComponent* collider2 = new CollisionComponent("collider", rectangle);
	collisions->addComponent(collider2);
	*/
	/*CircleCollider* circle = new CircleCollider(fPoint(180, 190), 5, TypeCollider::NONE_COLLIDER);
	CollisionComponent* collider2 = new CollisionComponent("collider2", circle);
	Entity* collisions = new Entity();
		collisions->addComponent(collider2);
	
	root->addChild(buffer);
	root->addChild(collisions);*/

	//buffer = Enemy::makeEnemy();
	//buffer->transform->position = {150, 170};
	//root->addChild(buffer);

	/*buffer = Enemy::makeEnemy();
	buffer->transform->flip = SDL_FLIP_HORIZONTAL;
	buffer->transform->position = {30, 170};

	root->addChild(buffer);*/
	//entities.push_back(buffer);
	root->addChild(buffer);
	root->start();
	return true;
}