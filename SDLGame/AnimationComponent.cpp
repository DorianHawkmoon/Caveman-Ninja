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
	if (state != nullptr) {
		delete state;
	}
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
	Transform global = parent->transform->getGlobalTransform();
	Animation* anim = state->getState()->getValue();
	global.position.x += anim->offset.x;
	global.position.y += anim->offset.y;
	if (global.flip!=SDL_FLIP_NONE) {
		global.position.x += anim->flippedOffset.x;
		global.position.y += anim->flippedOffset.y;
	}
	App->renderer->blit(texture, global, &(anim->GetCurrentFrame()), 1.0);
	return UPDATE_CONTINUE;
}

bool AnimationComponent::cleanUp() {
	App->textures->unload(texture);
	return true;
}

//can't be cloned!!! shared pointers inside
IComponent * AnimationComponent::makeClone() {
	//std::string name = getID();
	//AnimationComponent* clone=new AnimationComponent(name, nameTexture,state);
	//TODO clone animationComponent
	return nullptr;
}
