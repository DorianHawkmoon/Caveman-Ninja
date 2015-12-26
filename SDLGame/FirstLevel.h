#pragma once
#ifndef SCENE_KEN_H
#define SCENE_KEN_H

#include "Scene.h"
//TODO es el que hace la instancia de la siguiente escena y se lo pasa al módulo
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


