#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "Globals.h"
#include <list>
#include "Module.h"
#include "GUILabel.h"

class ModuleRender;
class ModuleWindow;
class ModuleParticles;
class ModuleTextures;
class ModuleInput;
class ModuleScene;
class ModuleAudio;
class ModuleFadeToBlack;
class ModulePlayer;
class ModuleTimer;
class ModuleCollision;
class ModuleFonts;
class ModuleGUI;

class Application {
public:
	Application();
	virtual ~Application();

	bool init();
	update_status update();
	bool cleanUp();

	bool isPaused() const;
	void pause(bool pause, bool userPaused=false);

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
	ModuleTimer* timer;
	ModuleFonts* fonts;
	ModuleGUI* gui;

private:
	std::list<Module*> modules;
	bool paused;
	bool userPaused;
	GUI::GUILabel* label;
};

extern Application* App;

#endif // !APPLICATION_H
