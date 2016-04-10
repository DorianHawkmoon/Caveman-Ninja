#include "ModuleParticles.h"
#include "Application.h"
#include "ModuleRender.h"
#include "Timer.h"

ModuleParticles::~ModuleParticles() {}

update_status ModuleParticles::update() {
	return UPDATE_CONTINUE;
}

update_status ModuleParticles::postUpdate() {
	for (auto it = particles.begin(); it != particles.end();) {
		Particle* p = *it;
		if (p->update() != UPDATE_CONTINUE) {
			p->cleanUp();
			delete p;
			it = particles.erase(it);
		} else {
			p->postUpdate();
			++it;
		}
	}

	return UPDATE_CONTINUE;
}

bool ModuleParticles::cleanUp() {
	for (auto it = particles.begin(); it != particles.end(); ++it) {
		delete *it;
	}
	particles.clear();
	return UPDATE_CONTINUE;
}

void ModuleParticles::addParticle(const Particle& particle, fPoint position, fPoint velocity, int delay) {
	Particle* p = new Particle(particle);
	p->timer->start();
	p->position = position;
	p->delay = delay;
	p->speed = velocity;
	p->start();
	particles.push_back(p);
}
