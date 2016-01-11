#pragma once
#ifndef PLAYER_HITTED_COMPONENT_H
#define PLAYER_HITTED_COMPONENT_H

#include "IComponent.h"
#include "Timer.h"
class LifeComponent;
class CollisionComponent;
class MotionComponent;

class PlayerHittedComponent : public IComponent {
public:
	PlayerHittedComponent(const std::string& name);
	~PlayerHittedComponent();
	
	bool start();
	update_status preUpdate();
	virtual update_status update();
	bool cleanUp();
	IComponent* makeClone() const;


	virtual void onCollisionEnter(const Collider* self, const Collider* another);

private:
	bool hitted;
	Timer timer;
	bool dead;

	LifeComponent* life;
	MotionComponent* motion;
	CollisionComponent* collision;

	bool cleaned;

	unsigned int damageReceivedEffect;
};


#endif // !HITTED_COMPONENT_H
