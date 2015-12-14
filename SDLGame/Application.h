#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "Globals.h"
#include <list>
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleScene;
class ModuleAudio;
class ModuleFadeToBlack;
class ModulePlayer;
class ModuleSceneKen;

class Application {
public:
	Application();
	virtual ~Application();

	bool init();
	bool start();
	update_status preUpdate();
	update_status update();
	update_status postUpdate();
	bool cleanUp();

public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleScene* scene;
	ModuleAudio* audio;
	ModuleFadeToBlack* fade;

	ModulePlayer* player;
	ModuleSceneKen* scene_ken;

private:

	std::list<Module*> modules;
};

extern Application* App;

#endif // !APPLICATION_H
