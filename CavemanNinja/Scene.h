#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "Globals.h"
#include <list>
#include "IUpdater.h"
#include <vector>
namespace GUI {
	class GUIContainer;
	class GUIComponent;
}

class Entity;
class SceneNode;

enum TypeScene {
	TITLE_SCENE,
	CUT_SCENE,
	LEVEL_SCENE,

};

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
	void addGUI(GUI::GUIComponent* gui);
	TypeScene getTypeScene() const;

protected:
	SceneNode* root;
	//not owned!
	std::vector<GUI::GUIComponent*> gui;
	std::list<Entity*> entities;
	TypeScene type;

private:
	Entity* rootE;
};
#endif // !SCENE_H


