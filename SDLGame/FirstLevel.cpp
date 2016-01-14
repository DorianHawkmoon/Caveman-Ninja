#include "FirstLevel.h"

#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleScene.h"
#include "EntryScene.h"
#include "Entity.h"
#include "SceneNode.h"

#include "SpriteComponent.h"
#include "HorizontalSpriteScrollComponent.h"
#include "CollisionComponent.h"
#include "CircleCollider.h"
#include "RectangleCollider.h"
#include "LineCollider.h"
#include <vector>

#include "Transform.h"
#include "Enemy.h"
#include "Trigger.h"
#include "ModuleGUI.h"
#include "GUIContainer.h"
#include "GUIAnimation.h"
#include "GUILifeBar.h"

FirstLevel::FirstLevel() {
	
}


FirstLevel::~FirstLevel() {
}

// Load assets
bool FirstLevel::start() {
	LOG("Loading first level");
	App->player->enable();

	App->renderer->camera.leftLimit = {0,0};
	App->renderer->camera.rightLimit = {1088,256};

	Entity* buffer = new Entity();
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

	root->addChild(buffer);


	// ---------------------------------------------


	buffer = new Entity();
	iPoint rectCollider = {1088, 20};
	RectangleCollider* collider = new RectangleCollider(fPoint(0, 225), rectCollider,0, TypeCollider::FLOOR);
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


	LineCollider* line = new LineCollider(fPoint(0, 0), std::vector<fPoint>{fPoint(300, 211),
		fPoint(340, 200),
		fPoint(443, 154),
		fPoint(487, 118),
		fPoint(512, 99),
		fPoint(544, 88),
		fPoint(578, 88),
		fPoint(616, 103),
		fPoint(664, 141),
		fPoint(700, 155),
		fPoint(708, 151),
		fPoint(741, 153),
		fPoint(756, 169),
		fPoint(781, 185),
		fPoint(798, 196)}, TypeCollider::GROUND);
	line->thickness = 1;
	colliderComponent = new CollisionComponent("lomo", line);
	buffer->addComponent(colliderComponent);

	line= new LineCollider(fPoint(0, 0), std::vector<fPoint>{
		fPoint(520, 128),
		fPoint(520, 129),
		fPoint(550, 115),
		fPoint(562, 115),
		fPoint(576, 124),
		fPoint(587, 145)}, TypeCollider::GROUND);
	line->thickness = 1;
	colliderComponent = new CollisionComponent("lomo2", line);
	buffer->addComponent(colliderComponent);

	line = new LineCollider(fPoint(0,0), std::vector<fPoint>{fPoint(966, 140),
		fPoint(1088, 140)}, TypeCollider::GROUND);
	line->thickness = 1;
	colliderComponent = new CollisionComponent("final", line);
	buffer->addComponent(colliderComponent);


	root->addChild(buffer); //added scenario

	fPoint positionTrigger = {1070,0};
	buffer = Trigger::makeTrigger(positionTrigger, {10,256},
		[]() {
		App->audio->stopMusic();
		Scene* scene = new EntryScene();
		App->scene->changeScene(scene, 1);
	}
	);
	root->addChild(buffer);

	// ---------------------------------------------

	buffer = Enemy::makeEnemy();
	buffer->transform->position = {340, 180};
	root->addChild(buffer);
	
	
	putEnemies();

	root->start();
	makeHUD();

	App->audio->playMusic("backgroundMusic.mp3");

	return true;
}

bool FirstLevel::cleanUp() {
	LOG("Unloading first level");
	Scene::cleanUp();
	App->player->disable();
	return true;
}

void FirstLevel::putEnemies() {
	fPoint positionTrigger = fPoint(180, 0);
	std::vector<fPoint> enemies = std::vector<fPoint>();
		enemies.push_back({350,155});
	makeEnemy(positionTrigger, enemies);

	positionTrigger.x=220;
	enemies.clear();
	enemies.push_back({380,135});
	makeEnemy(positionTrigger, enemies);

	positionTrigger.x = 340;
	enemies.clear();
	enemies.push_back({490,70}); //rock
	enemies.push_back({490,180}); //hidden
	makeEnemy(positionTrigger, enemies);


	positionTrigger.x = 410;
	enemies.clear();
	enemies.push_back({570,45});
	makeEnemy(positionTrigger, enemies);

	positionTrigger.x = 480;
	enemies.clear();
	enemies.push_back({620,65}); //rock
	enemies.push_back({620,180}); //egg
	makeEnemy(positionTrigger, enemies);

	positionTrigger.x = 506;
	enemies.clear();
	enemies.push_back({690,110});
	enemies.push_back({690,180}); //hidden
	makeEnemy(positionTrigger, enemies);

	positionTrigger.x = 580;
	enemies.clear();
	enemies.push_back({430,180}); //por detras
	enemies.push_back({730,180}); //rock
	//helicoptero, no poner
	makeEnemy(positionTrigger, enemies);


	positionTrigger.x = 650;
	enemies.clear();
	enemies.push_back({450,180}); //por detras up
	enemies.push_back({820,180}); //hidden
	makeEnemy(positionTrigger, enemies);

	positionTrigger.x = 720;
	enemies.clear();
	enemies.push_back({890,180});
	makeEnemy(positionTrigger, enemies);

}

void FirstLevel::makeEnemy(fPoint positionTrigger, const std::vector<fPoint>& enemies) {
	SceneNode* enlace = root;
	
	Entity* buffer = Trigger::makeTrigger(positionTrigger, {10,256}, 
							[enlace, enemies]() {
								for (auto it = enemies.cbegin(); it != enemies.cend(); ++it) {
									Entity* enemy = Enemy::makeEnemy();
									fPoint positionEnemy = *it;
									enemy->transform->position.x = positionEnemy.x;
									enemy->transform->position.y = positionEnemy.y;

									enemy->start();
									enlace->addChild(enemy);
								}
							}
						);
	root->addChild(buffer);
}

void FirstLevel::makeHUD() {
	GUI::GUILifeBar* lifebar = new GUI::GUILifeBar("gui_lifebar.png", App->player->life);
	lifebar->transform.position = {24,12};
	lifebar->pointColor[GUI::PointColor::GREEN] = {0,0,5,12};
	lifebar->pointColor[GUI::PointColor::YELLOW] = {5,0,5,12};
	lifebar->pointColor[GUI::PointColor::RED] = {10,0,5,12};
	lifebar->pointColor[GUI::PointColor::EMPTY] = {15,0,5,12};
	rootGUI->pack(lifebar);


	App->gui->registerGUI(rootGUI);
}
