#pragma once
#ifndef MODULE_PARTICLES_H
#define MODULE_PARTICLES_H

#include "Module.h"
#include <list>
#include "Point.h"
#include "Particle.h"


class ModuleParticles : public Module {
public:
	ModuleParticles(bool started = true) :Module(started) {}
	~ModuleParticles();

	virtual bool start() {
		return true;
	}
	virtual update_status update();
	virtual update_status postUpdate();
	virtual bool cleanUp();

	void addParticle(const Particle& particle, fPoint position, fPoint velocity, int delay=0);

private:
	std::list<Particle*> particles;
};

#endif // !MODULE_PARTICLES_H

