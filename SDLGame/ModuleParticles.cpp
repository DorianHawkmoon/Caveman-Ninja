#include "ModuleParticles.h"


ModuleParticles::ModuleParticles(bool started): Module(started) {}

ModuleParticles::~ModuleParticles() {}

void ModuleParticles::addParticle(IParticle * particle) {
	if (particle != nullptr) {
		particle->start();
		particles.push_back(particle);
	}
}

inline bool ModuleParticles::start() {
	LOG("Initialiting particles");
	return true;
}

inline update_status ModuleParticles::preUpdate() {
	update_status result = UPDATE_CONTINUE;
	for (std::list<IParticle*>::iterator it = particles.begin(); it != particles.end() && result == UPDATE_CONTINUE; ++it) {
		result = (*it)->preUpdate();
	}
	return UPDATE_CONTINUE;
}

inline update_status ModuleParticles::update() {
	update_status result = UPDATE_CONTINUE;
	for (std::list<IParticle*>::iterator it = particles.begin(); it != particles.end() && result==UPDATE_CONTINUE; ++it) {
		result = (*it)->update();
	}
	return UPDATE_CONTINUE;
}

inline update_status ModuleParticles::postUpdate() {
	update_status result = UPDATE_CONTINUE;
	for (std::list<IParticle*>::iterator it = particles.begin(); it != particles.end() && result == UPDATE_CONTINUE; ++it) {
		result = (*it)->postUpdate();
	}
	clearDeadParticles();
	return UPDATE_CONTINUE;
}

inline bool ModuleParticles::cleanUp() {
	LOG("Unloading particles");
	for (std::list<IParticle*>::iterator it = particles.begin(); it != particles.end(); ++it) {
		delete *it;
	}
	particles.clear();
	return true;
}

void ModuleParticles::clearDeadParticles() {
	/*for (std::list<IParticle*>::iterator it= particles.begin(); it != particles.end();) {
		if ((*it)->timelife<=0 ) {
			delete *it;
			it = particles.erase(it);
		} else {
			++it;
		}
	}*/
}
