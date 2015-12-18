#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleTextures.h"
#include "ModuleScene.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleFadeToBlack.h"
#include "ModuleParticles.h"
#include "ModuleCollision.h"
#include "SDL/SDL.h"

Application::Application() {
	// Order matters: they will init/start/update in this order
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(audio = new ModuleAudio());
	

	// Game Modules
	
	modules.push_back(scene = new ModuleScene());
	modules.push_back(player = new ModulePlayer());

	/*modules.push_back(particles = new ModuleParticles());
	modules.push_back(collisions = new ModuleCollision());*/

	modules.push_back(fade = new ModuleFadeToBlack());
}

Application::~Application() {
	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it) {
		delete (*it);
	}
	modules.clear();
	SDL_Quit();
}

bool Application::init() {
	bool ret = true;

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it) {
		ret = (*it)->init();
	}
	
	for(std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it){
		if ((*it)->isEnabled()) {
			ret = (*it)->start();
		}
	}

	// Start the first scene --
	fade->fadeToBlack(scene, nullptr, 3.0f);

	return ret;
}

update_status Application::update() {
	update_status ret = UPDATE_CONTINUE;

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it) {
		if ((*it)->isEnabled() == true) {
			ret = (*it)->preUpdate();
		}
	}

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it) {
		if ((*it)->isEnabled() == true) {
			ret = (*it)->update();
		}
	}

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it) {
		if ((*it)->isEnabled() == true) {
			ret = (*it)->postUpdate();
		}
	}

	return ret;
}

bool Application::cleanUp() {
	bool ret = true;

	for (std::list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it) {
		ret = (*it)->cleanUp();
	}

	return ret;
}