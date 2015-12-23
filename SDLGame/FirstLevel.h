#pragma once
#ifndef SCENE_KEN_H
#define SCENE_KEN_H

#include "Scene.h"

class FirstLevel : public Scene {
public:
	FirstLevel();
	~FirstLevel();

	bool start();
	update_status preUpdate();
	update_status update();
	update_status postUpdate();
	bool cleanUp();
};
#endif // !SCENE_KEN_H


