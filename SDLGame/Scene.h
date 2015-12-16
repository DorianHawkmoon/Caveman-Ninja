#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "Globals.h"
#include <list>

class Entity;
class SceneNode;

class Scene {
public:
	Scene();
	virtual ~Scene();

	virtual bool start() = 0;
	virtual bool preUpdate();
	virtual bool update();
	virtual bool postUpdate();
	virtual bool cleanUp();

protected:
	SceneNode* root;
	std::list<Entity*> entities;
};
#endif // !SCENE_H


