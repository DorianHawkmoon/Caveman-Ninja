#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "Globals.h"
#include <list>
#include "Module.h"
#include "SDL\SDL.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleScene;
class ModuleAudio;

class Application {
public:
	Application();
	virtual ~Application();

	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleScene* scene;
	ModuleAudio* audio;

private:

	std::list<Module*> modules;
};

extern Application* App;

#endif // !APPLICATION_H
