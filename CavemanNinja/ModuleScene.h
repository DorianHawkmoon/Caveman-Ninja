#pragma once
#ifndef MODULE_SCENE_H
#define MODULE_SCENE_H

#include "Module.h"

struct SDL_Texture;
class Scene;
class Entity;
namespace GUI {
	class GUIComponent;
}

class ModuleScene : public Module {
public:
	ModuleScene(bool started=true);
	virtual ~ModuleScene();

	bool start();
	update_status preUpdate();
	update_status update();
	update_status postUpdate();
	bool cleanUp();
	void changeScene(Scene* scene, float time=1.0f);

	void addEntity(Entity* entity);
	void addGUI(GUI::GUIComponent* gui);
	const Scene* getCurrentScene() const;

private:
	Scene* currentScene;
	Scene* nextScene;

	unsigned int startTime;
	unsigned int totalTime;
	bool fadingIn = true;

private:
	void makeChangeScene();
};

#endif // !MODULE_SCENE_H

