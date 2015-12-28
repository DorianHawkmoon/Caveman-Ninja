#ifndef __MODULEPLAYER_H__
#define __MODULEPLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "Point.h"
#include "StateMachine.h"
struct SDL_Texture;
class Entity;

class ModulePlayer : public Module
{
public:
	ModulePlayer(bool start_enabled = true);
	~ModulePlayer();

	bool start();
	update_status update();
	update_status postUpdate();
	bool cleanUp();

public:
	Entity* player;
	SDL_Texture* graphics = nullptr;
	StateMachine<Animation>* animations;
	iPoint position;
};

#endif // __MODULEPLAYER_H__