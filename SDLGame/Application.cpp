#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleTextures.h"
#include "ModuleScene.h"
#include "ModuleAudio.h"

Application::Application() {
	// Order matters: they will Init/start/update in this order
	modules.push_back(window = new ModuleWindow());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(textures = new ModuleTextures());
	modules.push_back(input = new ModuleInput());
	modules.push_back(scene = new ModuleScene());
	modules.push_back(audio = new ModuleAudio());

	// Homework: Create a new module to handle music and sound effects
}

Application::~Application() {
	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it) {
		delete (*it);
	}
	modules.clear();
	SDL_Quit();
}

bool Application::Init() {
	bool ret = true;

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	return ret;
}

bool Application::Start() {
	bool result = true;
	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && result == true; ++it)
		result = (*it)->Start();
	return result;
}

update_status Application::PreUpdate() {
	update_status ret = UPDATE_CONTINUE;

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	return ret;
}

update_status Application::Update() {
	update_status ret = UPDATE_CONTINUE;

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update();

	return ret;
}

update_status Application::PostUpdate() {
	update_status ret = UPDATE_CONTINUE;

	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	return ret;
}

bool Application::CleanUp() {
	bool ret = true;

	for (std::list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}