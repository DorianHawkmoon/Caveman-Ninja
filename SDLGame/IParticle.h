#pragma once
#ifndef IPARTICLE_H
#define IPARTICLE_H

#include "Point.h"
#include "Globals.h"
#include "SDL\SDL.h"
struct SDL_Texture;

class IParticle {
public:
	IParticle() : timelife(1000), position(), velocity(),startLife(0) {}
	unsigned int startLife;
	unsigned int timelife;
	fPoint position;
	fPoint velocity;

	inline virtual bool start() {
		startLife = SDL_GetTicks();
		return true;
	}

	inline virtual update_status preUpdate() {
		return UPDATE_CONTINUE;
	}

	inline virtual update_status update() {
		return UPDATE_CONTINUE;
	}

	inline virtual update_status postUpdate() {
		return UPDATE_CONTINUE;
	}

	inline virtual bool cleanUp() {
		return true;
	}

protected:
	SDL_Texture* texture;
};

#endif // !IPARTICLE_H
