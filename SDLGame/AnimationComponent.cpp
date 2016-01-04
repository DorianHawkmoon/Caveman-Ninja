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
	fPoint fpos = parent->transform->getGlobalTransform().position;
	iPoint pos(static_cast<int>(fpos.x), static_cast<int>(fpos.y));
	Animation* anim = state->getState()->getValue();

	App->renderer->blit(texture, pos, &(anim->GetCurrentFrame()), anim->offset, 1.0, parent->transform->flip);
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
