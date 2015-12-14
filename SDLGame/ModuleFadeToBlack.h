#pragma once
#ifndef MODULE_FADE_TO_BLACK_H
#define MODULE_FADE_TO_BLACK_H

#include "Module.h"
#include "SDL\SDL.h"

class ModuleFadeToBlack : public Module {
public:
	ModuleFadeToBlack(bool started=true);
	~ModuleFadeToBlack();

	bool start();
	update_status update();
	void fadeToBlack(Module* moduleOn, Module* moduleOff = nullptr, float time = 1.0f);
	bool isFading() const;

private:

	Uint32 startTime = 0;
	Uint32 totalTime = 0;
	bool fadingIn = true;
	Module* moduleOn = nullptr;
	Module* moduleOff = nullptr;
};

#endif // !MODULE_FADE_TO_BLACK_H


