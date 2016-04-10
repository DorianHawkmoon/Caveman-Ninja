#include "ModuleGUI.h"
#include "GUIComponent.h"
#include "Transform.h"
#include "Application.h"
#include "ModuleRender.h"

ModuleGUI::~ModuleGUI() {}

bool ModuleGUI::start() {
	//get the size windows and set a root container
	SDL_Rect windows = App->renderer->camera.getWindowsSize();
	root.setSize(iPoint(windows.h, windows.w));
	return true;
}

update_status ModuleGUI::preUpdate() {
	//pass events from input!
	for (auto it = gui.begin(); it != gui.end(); ++it) {
		(*it)->preUpdate();
	}
	return UPDATE_CONTINUE;
}

update_status ModuleGUI::update() {
	for (auto it = gui.begin(); it != gui.end(); ++it) {
		(*it)->update();
	}
	return UPDATE_CONTINUE;
}

update_status ModuleGUI::postUpdate() {
	for (auto it = gui.begin(); it != gui.end(); ++it) {
		GUITransform trans = GUITransform();
		(*it)->draw(trans);
	}
	return UPDATE_CONTINUE;
}

bool ModuleGUI::cleanUp() {
	for (auto it = gui.begin(); it != gui.end(); ++it) {
		(*it)->cleanUp(); //don't delete, not owned
	}
	gui.clear();
	return true;
}

void ModuleGUI::registerGUI(GUI::GUIComponent * component) {
	if (std::find(gui.begin(), gui.end(), component) == gui.end()) {
		component->setParent(&root);
		component->start();
		gui.push_back(component);
	}
}

void ModuleGUI::removeGUI(GUI::GUIComponent * component) {
	auto result = std::find(gui.begin(), gui.end(), component);
	if (result != gui.end()) {
		(*result)->cleanUp();
		(*result)->setParent(nullptr);
		gui.erase(result);
	}
}

void ModuleGUI::handleEventGUI(const SDL_Event & event) {
	for (auto it = gui.begin(); it != gui.end(); ++it) {
		(*it)->handleEvent(event);
	}
}
