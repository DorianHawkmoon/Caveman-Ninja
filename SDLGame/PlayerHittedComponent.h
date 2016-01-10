#pragma once
#ifndef PLAYER_HITTED_COMPONENT_H
#define PLAYER_HITTED_COMPONENT_H

#include "IComponent.h"
#include "Timer.h"
class LifeComponent;
class MotionComponent;

class PlayerHittedComponent : public IComponent {
public:
	PlayerHittedComponent(const std::string& name);
	~PlayerHittedComponent();
	
	bool start();
	virtual update_status update();

	IComponent* makeClone();


	virtual void onCollisionEnter(const Collider* self, const Collider* another);

private:
	bool hitted;
	Timer timer;
	bool dead;

	LifeComponent* life;
	MotionComponent* motion;
};


#endif // !HITTED_COMPONENT_H
