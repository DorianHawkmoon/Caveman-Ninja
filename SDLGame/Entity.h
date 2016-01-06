#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <list>
#include "Globals.h"
#include "Transform.h"
#include "IUpdater.h"
#include "CollisionListener.h"
class IComponent;

enum class Category {
	NONE_CATEGORY = 0,
	PLAYER = 1 << 0,
	WALL = 1 << 1,
	ENEMY = 1 << 2, 
	PROYECTIL = 1 << 3
};

enum JumpType {
	NONE,
	JUMP,
	JUMP_DOWN,
	DOUBLE_JUMP,
	FALL
};

struct ControlEntity {
	ControlEntity() {
		moveX = 0;
		moveY = 0;
		attack = 0;
		run = false;
		stateJump = JumpType::NONE;
	}

	virtual ~ControlEntity() {}

	int moveX;
	int moveY;
	JumpType stateJump;
	int attack;
	bool run;
};

class Entity : public IUpdater, public CollisionListener {
public:
	Entity(Category category=Category::NONE_CATEGORY);
	virtual ~Entity();

	bool start();
	update_status preUpdate();
	update_status update();
	update_status postUpdate();
	bool cleanUp();


	bool addComponent(IComponent* component);
	IComponent* getComponent(const std::string& name);
	bool removeComponent(IComponent* component);
	bool removeComponent(const std::string& name);
	const std::list<IComponent*>& getComponents() {
		return properties;
	}


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

	virtual void onCollisionEnter(Collider* one, Collider* another);
	virtual void onCollisionExit(Collider* one, Collider* another);
	virtual void onCollisionStay(Collider* one, Collider* another);
	
public:
	Transform* transform;
	ControlEntity controller;

private:
	/**
	* Categoría de la entidad
	*/
	Category category;
	/**
	* Variable que guarda si la entidad está destruida
	* Será destruida por el nodo que la contiene
	*/
	bool destroyed;

	std::list<IComponent*> properties;
};


#endif // !ENTITY_H
