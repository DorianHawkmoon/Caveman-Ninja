#include "GravityPlayerComponent.h"

void GravityPlayerComponent::loadSound() {
	soundGround = App->audio->loadEffect("landedJump.wav");
}

void GravityPlayerComponent::playSound() {
	App->audio->playEffect(soundGround);
}
