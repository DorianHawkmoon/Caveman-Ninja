#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

#include "Timer.h"
#include "Point.h"
#include "Animation.h"

struct Particle {
public:
	Particle(const Animation& anim, const SDL_Texture* texture): anim(anim), graphics(texture) {}
	~Particle(){}

	const SDL_Texture* graphics = nullptr;
	Animation anim;
	iPoint position = {0, 0};
	iPoint speed = {0, 0};
	Timer timer;
	unsigned int delay;
	unsigned int born = 0;
};


#endif // !PARTICLE_H
