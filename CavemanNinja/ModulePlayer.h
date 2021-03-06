#ifndef __MODULEPLAYER_H__
#define __MODULEPLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "Point.h"
#include "StateMachine.h"
#include "Joe.h"
#include <vector>
#include "Score.h"

class Timer;
class LifeComponent;
class JumpComponent;
class WeaponComponent;
class MotionComponent;
class AnimationComponent;
class Entity;
class ScoreComponent;
struct ControlEntity;
struct Transform;

namespace GUI {
	class GUIComponent;
}

class ModulePlayer : public Module {
public:
	ModulePlayer(bool start_enabled = true);
	virtual ~ModulePlayer();

	bool start();
	update_status preUpdate();
	update_status update();
	update_status postUpdate();

	bool cleanUp();

	GUI::GUIComponent* getHUD() const;

public:
	Entity* player;
	MotionComponent* motion;
	JumpComponent* jump;
	LifeComponent* life;
	WeaponComponent* weapon;
	ScoreComponent* score;
	AnimationComponent* animation;
	bool levelFinished;

private:
	Entity* deadBody;
	Timer* gameOverTimer;
	GUI::GUIComponent* HUD;
	const Animation* attackAnimation;
	int lifes;
	bool ghost;
	unsigned int soundDie;

private:
	void makeHUD();
	void addGameOver();
	void dead();
	void checkInput();
	void levelEnd();
	void debugging();
	GUI::GUIComponent* portraitAnimation();
};

#endif // __MODULEPLAYER_H__