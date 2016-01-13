#pragma once
#ifndef ENTRY_SCENE_H
#define ENTRY_SCENE_H

#include "Scene.h"
#include <vector>
class GUIComponent;

class EntryScene : public Scene {
public:
	EntryScene();
	virtual ~EntryScene();

	virtual bool start();
	virtual update_status update();

private:
	std::vector<GUIComponent*> components;
};
#endif // !ENTRY_SCENE_H
