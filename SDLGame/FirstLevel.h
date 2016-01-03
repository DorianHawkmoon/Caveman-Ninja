#pragma once
#ifndef FIRST_LEVEL_H
#define FIRST_LEVEL_H

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
#endif // ! FIRST_LEVEL_H

