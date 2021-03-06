#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <list>
#include "Globals.h"
#include "Transform.h"
#include "IUpdater.h"
#include "CollisionListener.h"
class IComponent;

enum TypeJump {
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
		damage = 0;
		run = false;
		stateJump = TypeJump::NONE;
	}

	~ControlEntity() {}

	//direction move
	int moveX;
	int moveY;
	//state of the jump jump, double jump, fall...
	TypeJump stateJump;
	//state of attack attacking, running...
	int attack;
	//state of damage (from front or back) 1 front -1 back
	int damage;
	//running (enemy)
	bool run;
};

class Entity : public IUpdater, public CollisionListener {
public:
	Entity();
	virtual ~Entity();

	virtual bool start();
	virtual update_status preUpdate();
	virtual update_status update();
	virtual update_status postUpdate();
	virtual bool cleanUp();


	bool addComponent(IComponent* component);
	IComponent* getComponent(const std::string& name);
	bool removeComponent(IComponent* component);
	bool removeComponent(const std::string& name);
	const std::list<IComponent*>& getComponents() {
		return properties;
	}


	/**
	* Determina si la entidad est� destruida
	* @return true si la entidad est� destruida
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

	virtual void onCollisionEnter(const Collider* self, const Collider* another);
	virtual void onCollisionExit(const Collider* self, const Collider* another);
	virtual void onCollisionStay(const Collider* self, const Collider* another);

	Entity* clone() const;
	
public:
	Transform* transform;
	ControlEntity controller;

private:
	/**
	* Variable que guarda si la entidad est� destruida
	* Ser� destruida por el nodo que la contiene
	*/
	bool destroyed;

	std::list<IComponent*> properties;
};


#endif // !ENTITY_H
