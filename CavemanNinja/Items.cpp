#include "Items.h"
#include "CircleCollider.h"
#include "CollisionComponent.h"
#include "Entity.h"
#include "SpriteComponent.h"
#include "DestroyOnCollisionComponent.h"
#include "DataItemComponent.h"
#include "Utils.h"
#include "GravityComponent.h"
#include "LifespanComponent.h"

Entity * Items::makeItem() {
	int random=Utils::range(100);
	if (random < 50) {
		return makeItem(TypeItem::SMALL_FOOD);
	} else if (random < 80) {
		return makeItem(TypeItem::MEDIUM_FOOD);
	} else {
		return makeItem(TypeItem::BIG_FOOD);
	}
}

Entity * Items::makeItem(TypeItem item) {
	Entity* result = new Entity();
	
	DestroyOnCollisionComponent* destroy = new DestroyOnCollisionComponent("destroy");
	result->addComponent(destroy);

	LifespanComponent* lifespan = new LifespanComponent("lifespan",5000);
	result->addComponent(lifespan);

	int life = 0;
	int points = 0;
	SDL_Rect rect;
	float radius;
	Animation anim(1);
	switch (item) {
		case TypeItem::SMALL_FOOD:
			rect.x = Utils::range(7) * 32;
			rect.y = Utils::range(3) * 32;
			points = 100;
			life = 1;
			radius = 8;
			anim.sizeFrame = {0,0,16,16};
			break;

		case TypeItem::MEDIUM_FOOD:
			rect.x = Utils::range(2) * 32;
			rect.y = 128;
			points = 300;
			life = 3;
			radius = 8;
			anim.sizeFrame = {32,0,16,16};
			break;

		case TypeItem::BIG_FOOD:
			rect.x = Utils::range(7,3) * 32;
			rect.y = 128;
			points = 500;
			life = 5;
			radius = 16;
			anim.sizeFrame = {0,16,16,16};
			break;
	}
	rect.w = 32;
	rect.h = 32;

	iPoint offset;
	offset.x = -16;
	offset.y = -16;

	DataItemComponent* data = new DataItemComponent("data");
	data->type = item;
	data->life = life;
	data->points = points;
	data->effectScore = anim;
	result->addComponent(data);

	CircleCollider* circle = new CircleCollider(fPoint(0,0), radius, TypeCollider::ITEM);
	CollisionComponent* collider = new CollisionComponent("collider", circle);
	result->addComponent(collider);

	SpriteComponent* sprite = new SpriteComponent("sprite", "pickups_food.png");
	sprite->rect = rect;
	sprite->offset = offset;
	result->addComponent(sprite);

	return result;
}
