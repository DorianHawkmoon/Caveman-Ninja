#pragma once
#ifndef PLAYER_HITTED_COMPONENT_H
#define PLAYER_HITTED_COMPONENT_H

#include "IComponent.h"
#include "Timer.h"
#include "Transform.h"
#include "Particle.h"
class LifeComponent;
class CollisionComponent;
class MotionComponent;

class PlayerHittedComponent : public IComponent {
public:
	PlayerHittedComponent(const std::string& name);
	~PlayerHittedComponent();
	
	virtual bool start();
	virtual update_status preUpdate();
	virtual update_status update();
	virtual bool cleanUp();
	virtual IComponent* makeClone() const;


	virtual void onCollisionEnter(const Collider* self, const Collider* another);

private:
	bool hitted;
	Timer timer;
	Timer securityTimer;
	bool dead;

	LifeComponent* life;
	MotionComponent* motion;
	CollisionComponent* collision;
	
	SDL_Texture* textureEffects;
	Particle* particleJumped;
	unsigned int damageReceivedEffect;

	bool cleaned;

	

private:
	bool hittedEnemy(const Transform& globalAnother, const Transform& globalMine);
	bool hittedMyself(const Transform& globalAnother, const Transform& globalMine);
};


#endif // !HITTED_COMPONENT_H
