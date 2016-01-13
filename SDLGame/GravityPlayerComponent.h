#pragma once
#ifndef GRAVITY_COMPONENT_H
#define GRAVITY_COMPONENT_H

#include "GravityComponent.h"
#include "Application.h"
#include "ModuleAudio.h"

class GravityPlayerComponent :	public GravityComponent {
public:
	GravityPlayerComponent(const std::string& nameComponent, Collider *collider): GravityComponent(nameComponent,collider){}
	virtual ~GravityPlayerComponent(){}

protected:
	unsigned int soundGround;
	virtual void loadSound() {
		soundGround = App->audio->loadEffect("landedJump.wav");
	}
	virtual void playSound() {
		App->audio->playEffect(soundGround);
	}
};


#endif // !GRAVITY_COMPONENT_H
