#pragma once
#ifndef MODULE_PARTICLES_H
#define MODULE_PARTICLES_H

#include "Module.h"
#include <list>
class Particle;

class ModuleParticles : public Module {
public:
	ModuleParticles(bool started = true) :Module(started) {}
	~ModuleParticles();

	virtual bool start() {
		return true;
	}
	virtual update_status update() {
		return UPDATE_CONTINUE;
	}
	virtual update_status postUpdate() {
		return UPDATE_CONTINUE;
	}
	virtual bool cleanUp() {
		return UPDATE_CONTINUE;
	}

	void addParticle() {

	}

private:
	std::list<Particle*> particles;
};

#endif // !MODULE_PARTICLES_H

