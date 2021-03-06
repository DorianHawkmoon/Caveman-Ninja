#pragma once
#ifndef COLLIDER_INTERACTION_H
#define COLLIDER_INTERACTION_H

enum TypeCollider {
	NONE_COLLIDER = 0,
	WALL,
	PLAYER,
	ENEMY,
	PLAYER_SHOT,
	ITEM,
	GROUND,
	FLOOR,
	GRAVITY,
	TRIGGER,
	COLLIDER_MAX
};

class ColliderInteraction {
public:
	ColliderInteraction() {
		matrix[TypeCollider::NONE_COLLIDER][TypeCollider::NONE_COLLIDER] = true;
		matrix[TypeCollider::NONE_COLLIDER][TypeCollider::WALL] = true;
		matrix[TypeCollider::NONE_COLLIDER][TypeCollider::PLAYER] = true;
		matrix[TypeCollider::NONE_COLLIDER][TypeCollider::PLAYER_SHOT] = true;
		matrix[TypeCollider::NONE_COLLIDER][TypeCollider::ENEMY] = true;
		matrix[TypeCollider::NONE_COLLIDER][TypeCollider::GROUND] = true;
		matrix[TypeCollider::NONE_COLLIDER][TypeCollider::FLOOR] = true;
		matrix[TypeCollider::NONE_COLLIDER][TypeCollider::ITEM] = false;
		matrix[TypeCollider::NONE_COLLIDER][TypeCollider::GRAVITY] = true;
		matrix[TypeCollider::NONE_COLLIDER][TypeCollider::TRIGGER] = false;

		matrix[TypeCollider::WALL][TypeCollider::NONE_COLLIDER] = true;
		matrix[TypeCollider::WALL][TypeCollider::WALL] = false;
		matrix[TypeCollider::WALL][TypeCollider::PLAYER] = true;
		matrix[TypeCollider::WALL][TypeCollider::PLAYER_SHOT] = true;
		matrix[TypeCollider::WALL][TypeCollider::ENEMY] = true;
		matrix[TypeCollider::WALL][TypeCollider::GROUND] = false;
		matrix[TypeCollider::WALL][TypeCollider::FLOOR] = false;
		matrix[TypeCollider::WALL][TypeCollider::ITEM] = false;
		matrix[TypeCollider::WALL][TypeCollider::GRAVITY] = true;
		matrix[TypeCollider::WALL][TypeCollider::TRIGGER] = false;

		matrix[TypeCollider::PLAYER][TypeCollider::NONE_COLLIDER] = true;
		matrix[TypeCollider::PLAYER][TypeCollider::WALL] = true;
		matrix[TypeCollider::PLAYER][TypeCollider::PLAYER] = false;
		matrix[TypeCollider::PLAYER][TypeCollider::PLAYER_SHOT] = false;
		matrix[TypeCollider::PLAYER][TypeCollider::ENEMY] = true;
		matrix[TypeCollider::PLAYER][TypeCollider::GROUND] = true;
		matrix[TypeCollider::PLAYER][TypeCollider::FLOOR] = true;
		matrix[TypeCollider::PLAYER][TypeCollider::ITEM] = true;
		matrix[TypeCollider::PLAYER][TypeCollider::GRAVITY] = false;
		matrix[TypeCollider::PLAYER][TypeCollider::TRIGGER] = true;

		matrix[TypeCollider::PLAYER_SHOT][TypeCollider::NONE_COLLIDER] = true;
		matrix[TypeCollider::PLAYER_SHOT][TypeCollider::WALL] = true;
		matrix[TypeCollider::PLAYER_SHOT][TypeCollider::PLAYER] = true;
		matrix[TypeCollider::PLAYER_SHOT][TypeCollider::PLAYER_SHOT] = false;
		matrix[TypeCollider::PLAYER_SHOT][TypeCollider::ENEMY] = true;
		matrix[TypeCollider::PLAYER_SHOT][TypeCollider::GROUND] = false;
		matrix[TypeCollider::PLAYER_SHOT][TypeCollider::FLOOR] = true;
		matrix[TypeCollider::PLAYER_SHOT][TypeCollider::ITEM] = false;
		matrix[TypeCollider::PLAYER_SHOT][TypeCollider::GRAVITY] = false;
		matrix[TypeCollider::PLAYER_SHOT][TypeCollider::TRIGGER] = false;

		matrix[TypeCollider::ENEMY][TypeCollider::NONE_COLLIDER] = true;
		matrix[TypeCollider::ENEMY][TypeCollider::WALL] = true;
		matrix[TypeCollider::ENEMY][TypeCollider::PLAYER] = true;
		matrix[TypeCollider::ENEMY][TypeCollider::PLAYER_SHOT] = true;
		matrix[TypeCollider::ENEMY][TypeCollider::ENEMY] = false;
		matrix[TypeCollider::ENEMY][TypeCollider::GROUND] = true;
		matrix[TypeCollider::ENEMY][TypeCollider::FLOOR] = true;
		matrix[TypeCollider::ENEMY][TypeCollider::ITEM] = false;
		matrix[TypeCollider::ENEMY][TypeCollider::GRAVITY] = false;
		matrix[TypeCollider::ENEMY][TypeCollider::TRIGGER] = false;

		matrix[TypeCollider::GROUND][TypeCollider::NONE_COLLIDER] = true;
		matrix[TypeCollider::GROUND][TypeCollider::WALL] = false;
		matrix[TypeCollider::GROUND][TypeCollider::PLAYER] = false;
		matrix[TypeCollider::GROUND][TypeCollider::PLAYER_SHOT] = false;
		matrix[TypeCollider::GROUND][TypeCollider::ENEMY] = true;
		matrix[TypeCollider::GROUND][TypeCollider::GROUND] = false;
		matrix[TypeCollider::GROUND][TypeCollider::FLOOR] = false;
		matrix[TypeCollider::GROUND][TypeCollider::ITEM] = false;
		matrix[TypeCollider::GROUND][TypeCollider::GRAVITY] = true;
		matrix[TypeCollider::GROUND][TypeCollider::TRIGGER] = false;

		matrix[TypeCollider::FLOOR][TypeCollider::NONE_COLLIDER] = true;
		matrix[TypeCollider::FLOOR][TypeCollider::WALL] = false;
		matrix[TypeCollider::FLOOR][TypeCollider::PLAYER] = true;
		matrix[TypeCollider::FLOOR][TypeCollider::PLAYER_SHOT] = true;
		matrix[TypeCollider::FLOOR][TypeCollider::ENEMY] = true;
		matrix[TypeCollider::FLOOR][TypeCollider::GROUND] = false;
		matrix[TypeCollider::FLOOR][TypeCollider::FLOOR] = false;
		matrix[TypeCollider::FLOOR][TypeCollider::ITEM] = false;
		matrix[TypeCollider::FLOOR][TypeCollider::GRAVITY] = true;
		matrix[TypeCollider::FLOOR][TypeCollider::TRIGGER] = false;

		matrix[TypeCollider::ITEM][TypeCollider::NONE_COLLIDER] = false;
		matrix[TypeCollider::ITEM][TypeCollider::WALL] = false;
		matrix[TypeCollider::ITEM][TypeCollider::PLAYER] = true;
		matrix[TypeCollider::ITEM][TypeCollider::PLAYER_SHOT] = false;
		matrix[TypeCollider::ITEM][TypeCollider::ENEMY] = false;
		matrix[TypeCollider::ITEM][TypeCollider::GROUND] = false;
		matrix[TypeCollider::ITEM][TypeCollider::FLOOR] = false;
		matrix[TypeCollider::ITEM][TypeCollider::ITEM] = false;
		matrix[TypeCollider::ITEM][TypeCollider::GRAVITY] = false;
		matrix[TypeCollider::ITEM][TypeCollider::TRIGGER] = false;

		matrix[TypeCollider::GRAVITY][TypeCollider::NONE_COLLIDER] = true;
		matrix[TypeCollider::GRAVITY][TypeCollider::WALL] = true;
		matrix[TypeCollider::GRAVITY][TypeCollider::PLAYER] = false;
		matrix[TypeCollider::GRAVITY][TypeCollider::PLAYER_SHOT] = false;
		matrix[TypeCollider::GRAVITY][TypeCollider::ENEMY] = false;
		matrix[TypeCollider::GRAVITY][TypeCollider::GROUND] = true;
		matrix[TypeCollider::GRAVITY][TypeCollider::FLOOR] = true;
		matrix[TypeCollider::GRAVITY][TypeCollider::ITEM] = false;
		matrix[TypeCollider::GRAVITY][TypeCollider::GRAVITY] = false;
		matrix[TypeCollider::GRAVITY][TypeCollider::TRIGGER] = false;

		matrix[TypeCollider::TRIGGER][TypeCollider::NONE_COLLIDER] = false;
		matrix[TypeCollider::TRIGGER][TypeCollider::WALL] = false;
		matrix[TypeCollider::TRIGGER][TypeCollider::PLAYER] = true;
		matrix[TypeCollider::TRIGGER][TypeCollider::PLAYER_SHOT] = false;
		matrix[TypeCollider::TRIGGER][TypeCollider::ENEMY] = false;
		matrix[TypeCollider::TRIGGER][TypeCollider::GROUND] = false;
		matrix[TypeCollider::TRIGGER][TypeCollider::FLOOR] = false;
		matrix[TypeCollider::TRIGGER][TypeCollider::ITEM] = false;
		matrix[TypeCollider::TRIGGER][TypeCollider::GRAVITY] = false;
		matrix[TypeCollider::TRIGGER][TypeCollider::TRIGGER] = false;
	}
	
	~ColliderInteraction() {};

	bool permission(const TypeCollider& one, const TypeCollider& another) const {
		return matrix[one][another];
	}

private:
	bool matrix[TypeCollider::COLLIDER_MAX][TypeCollider::COLLIDER_MAX];
};

#endif // !COLLIDER_INTERACTION_H

