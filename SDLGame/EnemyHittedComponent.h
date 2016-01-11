#pragma once
#ifndef ENEMY_HITTED_COMPONENT_H
#define ENEMY_HITTED_COMPONENT_H

#include "IComponent.h"
#include "Timer.h"
class MotionComponent;
class LifeComponent;
class CollisionComponent;

class EnemyHittedComponent : public IComponent {
public:
	EnemyHittedComponent(const std::string& name);
	~EnemyHittedComponent();

	bool start();
	update_status preUpdate();
	virtual update_status update();
	bool cleanUp();
	IComponent* makeClone() const;


	virtual void onCollisionEnter(const Collider* self, const Collider* another);

private:
	bool hitted;
	bool dead;
	Timer timer;

	CollisionComponent* collision;
	LifeComponent* life;
	MotionComponent* motion;

	bool cleaned;

	unsigned int soundDie;
	bool playSound;
};

#endif // !ENEMY_HITTED_COMPONENT_H

