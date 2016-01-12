#include "ModuleParticles.h"
#include "Application.h"
#include "ModuleRender.h"

ModuleParticles::~ModuleParticles() {}

update_status ModuleParticles::update() {
	return UPDATE_CONTINUE;
}

update_status ModuleParticles::postUpdate() {
	for (auto it = particles.begin(); it != particles.end();) {
		Particle* p = *it;
		if (p->update() != UPDATE_CONTINUE) {
			delete p;
			it = particles.erase(it);
			LOG("Deleted particle");
		} else {
			LOG("Painting particle");
			p->postUpdate();
			++it;
		}

			/*if (p->fx_played == false) {
				p->fx_played = true;
				App->audio->PlayFx(p->fx);
			}*/
		
		
	}

	return UPDATE_CONTINUE;
}

void ModuleParticles::addParticle(const Particle& particle, fPoint position, fPoint velocity, int delay) {
	Particle* p = new Particle(particle);
	p->timer.start();
	p->position = position;
	p->delay = delay;
	p->speed = velocity;
	LOG("Added particle");
	p->start();
	particles.push_back(p);
}
