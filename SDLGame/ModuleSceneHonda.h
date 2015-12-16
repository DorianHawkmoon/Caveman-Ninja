#pragma once

#ifndef MODULE_SCENE_HONDA_H
#define MODULE_SCENE_HONDA_H

#include "Module.h"

#include "Animation.h"
#include "Globals.h"

struct SDL_Texture;

class ModuleSceneHonda : public Module {
public:
	ModuleSceneHonda(bool started=true);
	~ModuleSceneHonda();

	bool start();
	update_status update();
	bool cleanUp();

public:

	SDL_Texture* graphics = nullptr;
	SDL_Rect ground;
	SDL_Rect sky;
	SDL_Rect background;
	SDL_Rect lateral;
};
#endif // !MODULE_SCENE_HONDA_H


