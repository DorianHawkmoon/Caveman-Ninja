#ifndef __MODULESCENEKEN_H__
#define __MODULESCENEKEN_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"

struct SDL_Texture;

class ModuleSceneKen : public Module
{
public:
	ModuleSceneKen( bool started = true);
	~ModuleSceneKen();

	bool start();
	update_status update();
	bool cleanUp();

public:
	
	SDL_Texture* graphics = nullptr;
	SDL_Rect ground;
	SDL_Rect background;
	Animation flag;
	Animation ship;
	Animation girl;
};

#endif // __MODULESCENEKEN_H__