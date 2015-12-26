#pragma once
#ifndef MODULE_SCENE_H
#define MODULE_SCENE_H

#include "Module.h"
#include "ModuleAudio.h"
#include "SDL_mixer/SDL_mixer.h"

struct SDL_Texture;
class Scene;

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

private:
	Scene* currentScene;
	Scene* nextScene;

	Uint32 startTime = 0;
	Uint32 totalTime = 0;
	bool fadingIn = true;

private:
	void makeChangeScene();
};

#endif // !MODULE_SCENE_H

