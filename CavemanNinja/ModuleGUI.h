#pragma once
#ifndef MODULE_GUI_H
#define MODULE_GUI_H

#include "Module.h"
#include <vector>
#include "SDL\SDL_events.h"
#include "GUIContainer.h"

namespace GUI {
	class GUIComponent;
}

class ModuleGUI : public Module {
public:
	ModuleGUI(bool started = true) : Module(started), gui() {}
	virtual ~ModuleGUI();

	virtual bool start();
	virtual update_status preUpdate();
	virtual update_status update();
	virtual update_status postUpdate();
	virtual bool cleanUp();

	void registerGUI(GUI::GUIComponent* component);
	void removeGUI(GUI::GUIComponent* component);

	void handleEventGUI(const SDL_Event& event);

private:
	//not owned!
	std::vector<GUI::GUIComponent*> gui;
	/**
	 * root container of the windows
	 */
	GUI::GUIContainer root;

};

#endif // !MODULE_GUI_H

