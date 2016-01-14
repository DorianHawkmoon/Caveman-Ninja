#pragma once
#ifndef ENEMY_BEHAVIOUR_H
#define ENEMY_BEHAVIOUR_H

#include "Transform.h"
#include "IAComponent.h"
#include <string>
#include "Timer.h"
struct Particle;
class LifeComponent;
class MotionComponent;
class CollisionComponent;
class JumpComponent;
class AnimationComponent;

class EnemyBehaviour : public IAComponent {
private:
	enum statesIA {
		SEARCHING,
		FORWARD,
		RUN,
		RUN_AWAY,
		JUMP,
		ATTACK
	};

public:
	EnemyBehaviour(const std::string& name): IAComponent(name) {}
	virtual ~EnemyBehaviour() {}

	virtual IComponent* clone() const;

	virtual bool start();
	virtual update_status update();
	virtual update_status postUpdate();

private:
	//tres fases de ataque
	//0 no esta atacando
	//1 esta atacando
	//2 se prepara para correr
	//3 esta corriendo
	void attacking(Transform& globalMine, Transform& globalPlayer);
	void checkCollisions(Transform& globalMine, Transform& globalPlayer);
	void forward(Transform& globalMine, Transform& globalPlayer);
	void runningAway(Transform& globalMine);
	void search(Transform& globalMine, Transform& globalPlayer);
	void running(Transform& globalMine, Transform& globalPlayer);
	void jumping(Transform& globalMine, Transform& globalPlayer);
	void setController(iPoint direction);

	iPoint directionPlayer(Transform & globalMine, Transform & globalPlayer) const;

	statesIA state;
	iPoint previousplayer;
	Timer controlIA;



	LifeComponent* life;
	MotionComponent* motion;
	CollisionComponent* collision;
	AnimationComponent* animations;
	JumpComponent* jump;

	//sounds of caveman
	unsigned int hit;
	unsigned int startRun;
	unsigned int run;
	unsigned int die;

	//effects
	SDL_Texture* effects; //preload effect
	Particle* particleAttack;
};

#endif // !ENEMY_BEHAVIOUR_H

