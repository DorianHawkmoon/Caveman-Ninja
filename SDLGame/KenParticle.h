#pragma once
#ifndef KEN_PARTICLE_H
#define KEN_PARTICLE_H

#include "IParticle.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "Application.h"
#include "Globals.h"
#include "Animation.h"

class KenParticle :	public IParticle {
public:
	KenParticle();
	~KenParticle();

	inline virtual bool start()  {
		IParticle::start();
		texture = App->textures->load("ryu4.png");
		timelife = 1000;
		velocity.x = 1;
		velocity.y = 0;

		animation.loop = 1;
		animation.speed = 0.03f;
		animation.frames.push_back({493,1563,43,32});
		animation.frames.push_back({550,1563,56,32});
		animation.frames.push_back({614,1563,26,32});
		/*animation.frames.push_back({652,1563,15,32});
		animation.frames.push_back({677,1563,28,32});*/

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
			App->renderer->blit(texture, position.x, position.y, &animation.GetCurrentFrame());
		}
		return UPDATE_CONTINUE;
	}

private:
	Animation animation;
};
#endif // !KEN_PARTICLE_H


