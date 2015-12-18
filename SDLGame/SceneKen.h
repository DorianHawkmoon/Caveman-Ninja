#pragma once
#ifndef SCENE_KEN_H
#define SCENE_KEN_H

#include "Scene.h"

class SceneKen :
	public Scene {
public:
	SceneKen();
	~SceneKen();

	bool start();
	bool update();
	bool cleanUp();

	Entity* player;
};
#endif // !SCENE_KEN_H


