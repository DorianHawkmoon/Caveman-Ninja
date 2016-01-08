#pragma once
#ifndef ENEMY_HITTED_COMPONENT_H
#define ENEMY_HITTED_COMPONENT_H

#include "IComponent.h"
#include "Timer.h"
class EnemyHittedComponent : public IComponent {
public:
	EnemyHittedComponent(const std::string& name);
	~EnemyHittedComponent();

	bool start();
	virtual update_status update();

	IComponent* makeClone();


	virtual void onCollisionEnter(const Collider* self, const Collider* another);

private:
	bool hitted;
	bool dead;
	Timer timer;
};

#endif // !ENEMY_HITTED_COMPONENT_H

