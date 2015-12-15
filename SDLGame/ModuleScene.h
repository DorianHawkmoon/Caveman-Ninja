#pragma once
#ifndef MODULE_SCENE_H
#define MODULE_SCENE_H

#include "Module.h"
#include "ModuleAudio.h"
#include "SDL_mixer/SDL_mixer.h"

struct SDL_Texture;

class ModuleScene : public Module {
public:
	ModuleScene(bool started=true);
	virtual ~ModuleScene();

	bool start();
	update_status update();

private:
	SDL_Texture* img;
	Mix_Music* music;
};

#endif // !MODULE_SCENE_H

