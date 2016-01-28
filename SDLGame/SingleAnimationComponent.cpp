#include "SingleAnimationComponent.h"

#include "Transform.h"
#include "Entity.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"

SingleAnimationComponent::SingleAnimationComponent(const std::string & nameComponent, const std::string & texture, const Animation& animation)
	: IComponent(nameComponent), nameTexture(texture), anim(animation),cleaned(true) {}

SingleAnimationComponent::~SingleAnimationComponent() {}

bool SingleAnimationComponent::start() {
	if (cleaned) {
		texture = App->textures->load(nameTexture.c_str());
		cleaned = !(texture != nullptr);
		return texture != nullptr;
	} else {
		return true;
	}
}

update_status SingleAnimationComponent::preUpdate() {
	if (toClean) {
		cleanUp();
		toClean = false;
	}
	return UPDATE_CONTINUE;
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
	SDL_Rect rect = anim.GetCurrentFrame();
	App->renderer->blit(texture, global, &rect, 1.0);
	return UPDATE_CONTINUE;
}

bool SingleAnimationComponent::cleanUp() {
	if (!cleaned) {
		App->textures->unload(texture);
		cleaned = true;
	}
	return true;
}

IComponent * SingleAnimationComponent::makeClone() const {
	return nullptr;
}
