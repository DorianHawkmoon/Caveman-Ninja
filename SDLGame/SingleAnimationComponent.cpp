#include "SingleAnimationComponent.h"

#include "Transform.h"
#include "Entity.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"

SingleAnimationComponent::SingleAnimationComponent(const std::string & nameComponent, const std::string & texture, Animation& animation)
	: IComponent(nameComponent), nameTexture(texture), anim(animation) {}

SingleAnimationComponent::~SingleAnimationComponent() {}

bool SingleAnimationComponent::start() {
	texture = App->textures->load(nameTexture.c_str());
	return texture != nullptr;
}

update_status SingleAnimationComponent::update() {
	anim.update();
	return UPDATE_CONTINUE;
}

update_status SingleAnimationComponent::postUpdate() {
	Transform global = parent->transform->getGlobalTransform();
	global.position.x += anim.offset.x;
	global.position.y += anim.offset.y;
	if (global.flip != SDL_FLIP_NONE) {
		global.position.x += anim.flippedOffset.x;
		global.position.y += anim.flippedOffset.y;
	}
	App->renderer->blit(texture, global, &(anim.GetCurrentFrame()), 1.0);
	return UPDATE_CONTINUE;
}

bool SingleAnimationComponent::cleanUp() {
	App->textures->unload(texture);
	return true;
}

IComponent * SingleAnimationComponent::makeClone() {
	return nullptr;
}
