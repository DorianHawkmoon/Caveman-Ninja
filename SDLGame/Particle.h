#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

#include "Timer.h"
#include "Point.h"
#include "Animation.h"

struct Particle : public IUpdater {
public:
	Particle(const Animation& anim) : anim(anim) {}
	~Particle() {}

	virtual bool start();

	virtual update_status update();

	virtual update_status postUpdate();

	SDL_Texture* graphics;
	Animation anim;
	fPoint position = {0, 0};
	fPoint speed = {0, 0};
	Timer timer;
	unsigned int delay = 0;
	unsigned int life = 0;
};


#endif // !PARTICLE_H
