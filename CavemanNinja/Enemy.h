#pragma once
#ifndef ENEMY_H
#define ENEMY_H

class Entity;
#include "Items.h"

struct Enemy {
	static Entity* makeEnemy(const TypeItem itemToDrop=TypeItem::SMALL_FOOD);
private:
	static void makeAnimations(Entity* entity);
};

#endif // !ENEMY_H

