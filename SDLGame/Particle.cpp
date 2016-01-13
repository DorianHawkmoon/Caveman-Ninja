#include "Particle.h"

#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTimer.h"
#include "ModuleTextures.h"

bool Particle::start() {
	timer.start();
	graphics = App->textures->load(nameTexture.c_str());
	return true;
}

update_status Particle::update() {
	float seconds = App->timer->getDeltaFrame() / 1000.0f;
	position += speed* seconds;
	return anim.update();
}

update_status Particle::postUpdate() {
	update_status result = UPDATE_CONTINUE;
	//determined by life time
	if (life > 0 || anim.isInfinity()) {
		if (timer.value() > (life + delay)) {
			result = UPDATE_STOP;
		}
	} else {
		//determined by animation
		if (anim.isFinished()) {
			result = UPDATE_STOP;
		}
	}

	//paint if everything correct
	if (result == UPDATE_CONTINUE && timer.value() > delay) {
		App->renderer->paintParticle(this);
	}

	return result;
}

bool Particle::cleanUp() {
	if (graphics != nullptr) {
		App->textures->unload(graphics);
	}
	return true;
}
