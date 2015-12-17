#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "Globals.h"
#include <list>
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleParticles;
class ModuleTextures;
class ModuleInput;
class ModuleScene;
class ModuleAudio;
class ModuleFadeToBlack;
class ModulePlayer;
class ModuleCollision;
class ModuleSceneKen;
class ModuleSceneHonda;

class Application {
public:
	Application();
	virtual ~Application();

	bool init();
	update_status update();
	bool cleanUp();

public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleScene* scene;
	ModuleAudio* audio;
	ModuleFadeToBlack* fade;
	ModuleParticles* particles;
	ModuleCollision* collisions;
	ModulePlayer* player;
	/*ModuleSceneKen* scene_ken;
	ModuleSceneHonda* scene_honda;*/

private:

	std::list<Module*> modules;
};

extern Application* App;

#endif // !APPLICATION_H
