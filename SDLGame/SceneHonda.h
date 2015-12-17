#pragma once
#ifndef SCENE_HONDA_H
#define SCENE_HONDA_H

#include "Scene.h"

#include "Animation.h"
#include "Globals.h"
struct SDL_Texture;

class SceneHonda :
	public Scene {
public:
	SceneHonda();
	~SceneHonda();

	bool start();
	bool update();
	bool cleanUp();

private:
	SDL_Texture* graphics = nullptr;
	SDL_Rect ground;
	SDL_Rect sky;
	SDL_Rect background;
	SDL_Rect lateral;
};

#endif // !SCENE_HONDA_H

