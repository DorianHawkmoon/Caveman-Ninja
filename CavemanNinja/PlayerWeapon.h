#pragma once
#ifndef PLAYER_WEAPON_H
#define PLAYER_WEAPON_H

class Entity;

enum Weapon {
	TOMAHAWK
};

struct PlayerWeapon {
	static Entity* makeWeapon(const Weapon& type=Weapon::TOMAHAWK);

private:
	static void tomahawk(Entity* entity);
};

#endif // !PLAYER_WEAPON_H

