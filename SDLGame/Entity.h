#pragma once
#ifndef ENTITY_H
#define ENTITY_H

enum Category {
	NONE = 0,
	PLAYER = 1 << 0,
	WALL = 1 << 1,
	ENEMY = 1 << 2, 
	PROYECTIL = 1 << 3
};

class Entity {
public:
	Entity(Category category=NONE);
	~Entity();

	/**
	* Determina si la entidad está destruida
	* @return true si la entidad está destruida
	*/
	inline bool isDestroyed() const {
		return destroyed;
	}

	/**
	* Setea la entidad como destruida
	*/
	inline void destroy() {
		destroyed = true;
	}

	/**
	* Devuelve la categoría de la entidad
	* @return categoría de la entidad
	*/
	inline Category getCategory() const {
		return category;
	}

private:
	/**
	* Categoría de la entidad
	*/
	Category category;
	/**
	* Variable que guarda si la entidad está destruida
	*/
	bool destroyed;

};


#endif // !ENTITY_H
