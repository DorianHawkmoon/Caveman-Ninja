#pragma once
#ifndef KEN_PARTICLE_H
#define KEN_PARTICLE_H

#include "IParticle.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "Application.h"
#include "Globals.h"

class KenParticle :	public IParticle {
public:
	KenParticle();
	~KenParticle();

	inline virtual bool start()  {
		IParticle::start();
		texture = App->textures->load("ryu4.png");
		timelife = 1000;
		velocity.x = 1;
		velocity.y = 1;

		return true;
	}

	update_status preUpdate() {
		//muevo la particula
		position += velocity;
		return UPDATE_CONTINUE;
	}

	inline virtual update_status update() {
		timelife -= SDL_GetTicks() - startLife;
		//pinto la particula
		if (timelife > 0) {
			SDL_Rect r;
			r.x = 493;
			r.y = 1563;
			r.w = 43;
			r.h = 32;
			App->renderer->blit(texture, position.x, position.y, &r);
		}
		return UPDATE_CONTINUE;
	}
};
#endif // !KEN_PARTICLE_H


