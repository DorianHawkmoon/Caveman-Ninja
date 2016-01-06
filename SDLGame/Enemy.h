#pragma once
#ifndef ENEMY_H
#define ENEMY_H
class Entity;

struct Enemy {
	static Entity* makeEnemy();
private:
	static void makeAnimations(Entity* entity);
	static void makeBehaviour(Entity* entity);

	static void behaviour(bool ticked);
};

#endif // !ENEMY_H

