#include "AnimationComponent.h"
#include "Application.h"
#include "ModuleRender.h"
#include "Entity.h"
#include "ModuleTextures.h"
#include "State.h"

AnimationComponent::AnimationComponent(const std::string & nameComponent, const std::string & texture, StateMachine<Animation>* stateMachine)
	: IComponent(nameComponent), nameTexture(texture), state(stateMachine), cleaned(true) {

}

AnimationComponent::~AnimationComponent() {
	cleanUp();
	if (state != nullptr) {
		delete state;
	}
}

bool AnimationComponent::start() {
	bool result = true;
	if (cleaned) {
		texture = App->textures->load(nameTexture.c_str());
		cleaned = !(texture!=nullptr);
		result = !cleaned;
	}
	return result;
}

update_status AnimationComponent::preUpdate() {
	if (toClean) {
		cleanUp();
		toClean = false;
	}
	return UPDATE_CONTINUE;
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
	SDL_Rect rectFrame = anim->GetCurrentFrame();
	App->renderer->blit(texture, global, &rectFrame, 1.0);
	return UPDATE_CONTINUE;
}

bool AnimationComponent::cleanUp() {
	if (!cleaned) {
		App->textures->unload(texture);
		cleaned = true;
	}
	return true;
}

//can't be cloned!!! shared pointers inside
IComponent * AnimationComponent::makeClone() const {
	//std::string name = getID();
	//AnimationComponent* clone=new AnimationComponent(name, nameTexture,state);
	//TODO clone animationComponent
	return nullptr;
}

const Animation* AnimationComponent::getActualAnimation() const {
	if (state != nullptr) {
		Animation* anim = state->getState()->getValue();
		return anim;
	} else {
		return nullptr;
	}
}
