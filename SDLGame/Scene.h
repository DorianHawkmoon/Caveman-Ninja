#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "Globals.h"
#include <list>
#include "IUpdater.h"
namespace GUI {
	class GUIContainer;
}

class Entity;
class SceneNode;

class Scene : public IUpdater{
public:
	Scene();
	virtual ~Scene();

	virtual bool start() = 0;
	virtual update_status preUpdate();
	virtual update_status update();
	virtual update_status postUpdate();
	virtual bool cleanUp();

	void addNode(Entity* entity);

protected:
	SceneNode* root;
	GUI::GUIContainer* rootGUI;
	std::list<Entity*> entities;

private:
	Entity* rootE;
};
#endif // !SCENE_H


