#include "AnimationComponent.h"
#include "Application.h"
#include "ModuleRender.h"
#include "Entity.h"
#include "ModuleTextures.h"
#include "State.h"

AnimationComponent::AnimationComponent(const std::string & nameComponent, const std::string & texture, StateMachine<Animation>* stateMachine)
	: IComponent(nameComponent), nameTexture(texture), state(stateMachine) {

}

AnimationComponent::~AnimationComponent() {
	cleanUp();
}

bool AnimationComponent::start() {
	texture = App->textures->load(nameTexture.c_str());
	return texture != nullptr;
}

update_status AnimationComponent::update() {
	state->getState()->getValue()->update();
	state->update();
	return UPDATE_CONTINUE;
}

update_status AnimationComponent::postUpdate() {
	fPoint fpos = parent->transform.position;
	iPoint pos(static_cast<int>(fpos.x), static_cast<int>(fpos.y));
	Animation* anim = state->getState()->getValue();
	pos.y -= anim->GetCurrentFrame().h;
	App->renderer->blit(texture, pos, &(anim->GetCurrentFrame()));
	return UPDATE_CONTINUE;
}

bool AnimationComponent::cleanUp() {
	App->textures->unload(texture);
	return true;
}

IComponent * AnimationComponent::makeClone() {
	//TODO clone animationComponent
	return nullptr;
}
