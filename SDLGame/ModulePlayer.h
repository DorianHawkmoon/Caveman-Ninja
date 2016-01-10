#ifndef __MODULEPLAYER_H__
#define __MODULEPLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "Point.h"
#include "StateMachine.h"
#include "Joe.h"
#include <vector>

class LifeComponent;
class JumpComponent;
class WeaponComponent;
class MotionComponent;
class Entity;

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

private:
	Entity* deadBody;
	bool ghost;
};

#endif // __MODULEPLAYER_H__