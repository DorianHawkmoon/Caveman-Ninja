#pragma once
#ifndef FIRST_LEVEL_H
#define FIRST_LEVEL_H

#include "Scene.h"
#include <functional>
#include "Point.h"
#include <vector>

class FirstLevel : public Scene {
public:
	FirstLevel();
	virtual ~FirstLevel();

	virtual bool start();
	virtual bool cleanUp();

	virtual update_status update();

private:
	void putEnemies();
	void makeEnemy(fPoint positionTrigger, const std::vector<fPoint>& enemies);

	unsigned int music;
};
#endif // ! FIRST_LEVEL_H

