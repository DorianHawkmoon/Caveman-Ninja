#pragma once
#ifndef ITEMS_H
#define ITEMS_H

class Entity;

enum class TypeItem {
	SMALL_FOOD,
	MEDIUM_FOOD,
	BIG_FOOD,
	WEAPON
};


class Items {
public:
	static Entity* makeItem();
	static Entity* makeItem(TypeItem item);
};
#endif // !ITEMS_H
