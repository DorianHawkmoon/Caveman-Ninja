#pragma once
#ifndef MODULE_PARTICLES_H
#define MODULE_PARTICLES_H

#include "Module.h"
#include "IParticle.h"
#include <list>

class ModuleParticles :
	public Module {
public:
	ModuleParticles(bool started=true);
	~ModuleParticles();

	void addParticle(IParticle* particle);


	virtual bool start();

	virtual update_status preUpdate();

	virtual update_status update();

	virtual update_status postUpdate();

	virtual bool cleanUp();

private:
	std::list<IParticle*> particles;

	void clearDeadParticles();
};
#endif // !MODULE_PARTICLES_H


