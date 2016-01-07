#pragma once
#ifndef ENEMY_BEHAVIOUR_H
#define ENEMY_BEHAVIOUR_H

#include "Transform.h"
#include "IAComponent.h"
#include <string>

class EnemyBehaviour : public IAComponent {
public:
	EnemyBehaviour(const std::string& name): IAComponent(name) {}
	~EnemyBehaviour() {}

	IComponent* clone();

	bool start() {
		return true;
	}
	update_status update();
	update_status postUpdate();

private:
	//tres fases de ataque
	//0 no esta atacando
	//1 esta atacando
	//2 se prepara para correr
	//3 esta corriendo
	void attacking(Transform& globalMine, Transform& globalPlayer);
	void checkCollisions(Transform& globalMine, Transform& globalPlayer);
	void updateMotion(Transform& globalMine, Transform& globalPlayer);
	void runningAway();
};

#endif // !ENEMY_BEHAVIOUR_H
