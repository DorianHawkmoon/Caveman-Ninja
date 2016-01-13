#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

#include "Timer.h"
#include "Point.h"
#include "Animation.h"
#include <string>

struct Particle : public IUpdater {
public:
	Particle(const std::string& nameTexture, const Animation& anim) : anim(anim), nameTexture(nameTexture) {}
	Particle(const Particle& particle): nameTexture(particle.nameTexture), anim(particle.anim) {
		position = particle.position;
		speed = particle.speed;
		delay = particle.delay;
		life = particle.life;
		timer = particle.timer;
	}
	virtual ~Particle() {}

	virtual bool start();

	virtual update_status update();

	virtual update_status postUpdate();
	virtual bool cleanUp();

	SDL_Texture* graphics;
	Animation anim;
	fPoint position = {0, 0};
	fPoint speed = {0, 0};
	Timer timer;
	unsigned int delay = 0;
	unsigned int life = 0;
private:
	const std::string nameTexture;
};


#endif // !PARTICLE_H
