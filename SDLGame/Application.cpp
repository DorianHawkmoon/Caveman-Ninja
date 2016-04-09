#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleTextures.h"
#include "ModuleScene.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleFadeToBlack.h"
#include "ModuleTimer.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleFonts.h"
#include "ModuleGUI.h"
#include "SDL/SDL.h"

#include <iomanip>
#include <locale>
#include <sstream>
#include <string> 

int DEBUG_COLLISIONS = 0;

Application::Application() {
	// Order matters: they will init/start/update in this order
	modules.push_back(timer = new ModuleTimer());
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(fonts = new ModuleFonts);
	modules.push_back(audio = new ModuleAudio());

	// Game Modules
	modules.push_back(scene = new ModuleScene());
	modules.push_back(player = new ModulePlayer(false));
	modules.push_back(particles = new ModuleParticles());
	modules.push_back(collisions = new ModuleCollision());

	modules.push_back(gui = new ModuleGUI());
	modules.push_back(fade = new ModuleFadeToBlack());
	modules.push_back(renderer = new ModuleRender());
}

Application::~Application() {
	for (auto it = modules.begin(); it != modules.end(); ++it) {
		delete (*it);
		*it = nullptr;
	}
	modules.clear();
	SDL_Quit();
}

bool Application::init() {
	bool ret = true;

	for (auto it = modules.begin(); it != modules.end() && ret; ++it) {
		ret = (*it)->init();
	}
	
	for(auto it = modules.begin(); it != modules.end() && ret; ++it){
		if ((*it)->isEnabled()) {
			ret = (*it)->start();
		}
	}

	SDL_Color color = SDL_Color();
	color.g = 255;
	color.b = 255;
	color.r = 255;
	color.a = 255;
	label = new GUI::GUILabel("Paused", color, "arcadepix.ttf", CENTER, 50);
	label->transform.pivot = GUILocation::CENTER;

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
	
	//control fps
	int delay = static_cast<int>(1.0 / FPS) - (timer->getDeltaFrame() / 1000);
	if (delay > 0) {
		SDL_Delay(delay);
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

bool Application::isPaused() const {
	return paused;
}

void Application::pause(bool pause) {
	if (paused == pause) return;

	paused = pause;
	if (paused) {
		timer->pause();
		SDL_Color c;
		c.b = 0;
		c.r = 0;
		c.g = 0;
		renderer->setFaddingEffect(0.5f, c);
		gui->registerGUI(label);
	} else {
		timer->unpause();
		gui->removeGUI(label);
		renderer->clearFaddingEffect();
	}
}
