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

	bool start();
	update_status update();

private:
	std::vector<GUIComponent*> components;
};
#endif // !ENTRY_SCENE_H
