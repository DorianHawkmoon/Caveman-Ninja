#include "PlayerWeapon.h"

#include "Entity.h"
#include "GravityWeaponComponent.h"
#include "SingleAnimationComponent.h"
#include "Animation.h"
#include "MotionComponent.h"
#include "ColliderInteraction.h"
#include "RectangleCollider.h"
#include "CollisionComponent.h"
#include "LifespanComponent.h"
#include "DestroyOnCollisionComponent.h"
#include "CircleCollider.h"
#include "DamageComponent.h"
#include "OnCollisionWeaponComponent.h"

Entity * PlayerWeapon::makeWeapon(const Weapon& type) {
	Entity* result = new Entity();

	switch (type) {
	case TOMAHAWK:
		tomahawk(result);
		break;
	}

	return result;
}

void PlayerWeapon::tomahawk(Entity * entity) {
	GravityWeaponComponent* gravity = new GravityWeaponComponent("gravity");
	gravity->gravity = 550;
	gravity->maxVelocity = 500;
	entity->addComponent(gravity);

	
	Animation anim = Animation(8);
	anim.sizeFrame = {0,0,32,32};
	anim.offset = {-5,-6};
	anim.speed = 0.3f;
	SingleAnimationComponent* animation = new SingleAnimationComponent("animations", "weapons.png", anim);
	entity->addComponent(animation);

	MotionComponent* motion = new MotionComponent("motion");
	motion->speed = 50;
	motion->doubleSpeed = 50;
	entity->addComponent(motion);

	CircleCollider* circle = new CircleCollider(fPoint(11, 11), 10, TypeCollider::PLAYER_SHOT);
	CollisionComponent* collider = new CollisionComponent("collider", circle);
	entity->addComponent(collider);

	
	Animation strong(1);
	strong.sizeFrame = {128,0,64,64};
	strong.offset = {-32,-32};
	Animation normal(1);
	normal.sizeFrame = {64,0,64,64};
	normal.offset = {-32,-32};
	DestroyOnCollisionComponent* destroy = new OnCollisionWeaponComponent("destroyOnCollision", "effects.png", normal, strong);
	entity->addComponent(destroy);

	LifespanComponent* lifespan = new LifespanComponent("lifespan", 2000);
	entity->addComponent(lifespan);


	DamageComponent* damage = new DamageComponent("damage", 5, 10);
	entity->addComponent(damage);

}
