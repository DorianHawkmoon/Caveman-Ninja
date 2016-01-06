#pragma once
#ifndef ENEMY_H
#define ENEMY_H
class Entity;

struct Enemy {
	static Entity* makeEnemy();
private:
	static void makeAnimations(Entity* entity);
};

#endif // !ENEMY_H

