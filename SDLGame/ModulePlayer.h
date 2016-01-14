#ifndef __MODULEPLAYER_H__
#define __MODULEPLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "Point.h"
#include "StateMachine.h"
#include "Joe.h"
#include <vector>
#include "Timer.h"
#include "Score.h"
#include "GUILabel.h"

class LifeComponent;
class JumpComponent;
class WeaponComponent;
class MotionComponent;
class Entity;
class ScoreComponent;

class ModulePlayer : public Module
{
public:
	ModulePlayer(bool start_enabled = true);
	virtual ~ModulePlayer();

	bool start();
	update_status preUpdate();
	update_status update();
	update_status postUpdate();
	bool cleanUp();

public:
	Entity* player;
	MotionComponent* motion;
	JumpComponent* jump;
	LifeComponent* life;
	WeaponComponent* weapon;
	ScoreComponent* score;

private:
	Entity* deadBody;
	Timer gameOverTimer;
	int lifes;
	bool ghost;
	unsigned int soundDie;

	GUI::GUILabel* scoreText;
	GUI::GUILabel* scoreTextHighest;

private:
	void addGameOver();
	void dead();
	void debugging();
};

#endif // __MODULEPLAYER_H__