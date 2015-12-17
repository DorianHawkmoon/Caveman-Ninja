#pragma once
#ifndef SCENE_KEN_H
#define SCENE_KEN_H

#include "Scene.h"
#include "Animation.h"
#include "Globals.h"
struct SDL_Texture;

class SceneKen :
	public Scene {
public:
	SceneKen();
	~SceneKen();

	bool start();
	bool update();
	bool cleanUp();

public:
	SDL_Texture* graphics = nullptr;
	SDL_Rect ground;
	SDL_Rect background;
	Animation flag;
	Animation ship;
	Animation girl;
};
#endif // !SCENE_KEN_H


