#pragma once
#ifndef MODULE_SCENE_H
#define MODULE_SCENE_H

#include "Module.h"
#include "ModuleAudio.h"
#include "SDL_mixer/SDL_mixer.h"

struct SDL_Texture;
class Scene;

//TODO gestiona el cambio de escena
//TODO instancia la escena y hace el cambio
//TODO load scene
//TODO remove scene
//TODO change scene
//TODO stack scenes (actual scene)?

class ModuleScene : public Module {
public:
	ModuleScene(bool started=true);
	virtual ~ModuleScene();

	bool start();
	update_status preUpdate();
	update_status update();
	update_status postUpdate();



private:
	Scene* scene;
};

#endif // !MODULE_SCENE_H

