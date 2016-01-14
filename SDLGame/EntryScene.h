#pragma once
#ifndef ENTRY_SCENE_H
#define ENTRY_SCENE_H

#include "Scene.h"

class EntryScene : public Scene {
public:
	EntryScene();
	virtual ~EntryScene();

	virtual bool start();
	virtual update_status update();

private:
};
#endif // !ENTRY_SCENE_H
