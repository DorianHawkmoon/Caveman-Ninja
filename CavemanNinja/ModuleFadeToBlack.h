#pragma once
#ifndef MODULE_FADE_TO_BLACK_H
#define MODULE_FADE_TO_BLACK_H

#include "Module.h"
#include "SDL/SDL.h"

class ModuleFadeToBlack : public Module {
public:
	ModuleFadeToBlack(bool started=true);
	virtual ~ModuleFadeToBlack();

	bool start();
	bool cleanUp();
	update_status update();
	update_status postUpdate();
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


