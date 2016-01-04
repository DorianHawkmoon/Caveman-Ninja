#pragma once
#ifndef COLLIDER_H
#define COLLIDER_H

#include "Point.h"
#include <list>
#include "IUpdater.h"
class CircleCollider;
class RectangleCollider;
class CollisionListener;

enum class TypeCollider {
	NONE_COLLIDER = 0,
	WALL = 1 << 0,
	PLAYER = 1 << 1,
	ENEMY = 1 << 2,
	PLAYER_SHOT = 1 << 3,
	ENEMY_SHOT = 1 << 4,
	ITEM = 1 << 5
};

class Collider : public IUpdater {
public:
	Collider(fPoint position, const TypeCollider type = TypeCollider::NONE_COLLIDER);
	virtual ~Collider();
	
	virtual bool cleanUp();
	virtual update_status preUpdate();
	virtual update_status update();
	virtual update_status postUpdate();
	//notify onEnter and onStay
	void notify(Collider* other);

	virtual void paintCollider() const {};
	void addListener(CollisionListener* newListener);
	void removeListener(CollisionListener* listener);

	virtual Collider* clone() = 0;
	virtual bool checkCollision(const Collider* r) const = 0;

	virtual bool checkSpecificCollision(const Collider* self) const = 0;
	virtual bool checkCollision(const RectangleCollider* other) const = 0;
	virtual bool checkCollision(const CircleCollider* other) const = 0;

public:
	fPoint position;
	TypeCollider type;

protected:
	Collider* clone(Collider* cloning) {
		cloning->listeners.insert(cloning->listeners.end(), listeners.begin(), listeners.end());
		cloning->actualFrame->insert(cloning->actualFrame->end(), actualFrame->begin(), actualFrame->end());
		cloning->previousFrame->insert(cloning->previousFrame->end(), previousFrame->begin(), previousFrame->end());
		return cloning;
	}

private:
	std::list<CollisionListener*> listeners;
	std::list<Collider*>* previousFrame;
	std::list<Collider*>* actualFrame;

	//notify onExit
	void checkExitCollision();
	//clear the frame collision
	void clearCollisions();
};

#endif // !COLLIDER_H

