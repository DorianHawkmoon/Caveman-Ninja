#pragma once
#ifndef FIRST_LEVEL_H
#define FIRST_LEVEL_H

#include "Scene.h"

class FirstLevel : public Scene {
public:
	FirstLevel();
	virtual ~FirstLevel();

	bool start();
};
#endif // ! FIRST_LEVEL_H

