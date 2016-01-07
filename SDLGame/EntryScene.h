#pragma once
#ifndef ENTRY_SCENE_H
#define ENTRY_SCENE_H

#include "Scene.h"

class EntryScene : public Scene {
public:
	EntryScene();
	virtual ~EntryScene();

	bool start();
	update_status update();
};
#endif // !ENTRY_SCENE_H
