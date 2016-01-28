#include "SpriteComponent.h"
#include "Application.h"
#include "ModuleRender.h"
#include "SDL/SDL.h"
#include "ModuleTextures.h"
#include "Transform.h"
#include "Entity.h"

SpriteComponent::SpriteComponent(const std::string &nameComponent, const std::string& nameTexture)
	: IComponent(nameComponent), name(nameTexture), speedCamera(1.0f), offset(0, 0), cleaned(true), flippedOffset(0,0) {}

SpriteComponent::~SpriteComponent() {
	//unload just in case
	cleanUp();
}

bool SpriteComponent::start() {
	if (cleaned) {
		texture = App->textures->load(name.c_str());
		cleaned = !(texture != nullptr);
		return texture != nullptr;
	} else {
		return true;
	}
}

update_status SpriteComponent::preUpdate() {
	if (toClean) {
		cleanUp();
		toClean = false;
	}
	return UPDATE_CONTINUE;
}

update_status SpriteComponent::postUpdate() {
	//paint the sprite
	iPoint finalPosition;
	Transform* parentTransform = parent->transform;
	Transform global = parentTransform->getGlobalTransform();
	global.position.x += offset.x;
	global.position.y += offset.y;
	if (global.flip != SDL_FLIP_NONE) {
		global.position.x += flippedOffset.x;
		global.position.y += flippedOffset.y;
	}
	
	App->renderer->blit(texture, global, &rect, speedCamera);
	return UPDATE_CONTINUE;
}

bool SpriteComponent::cleanUp() {
	if (!cleaned) {
		App->textures->unload(texture);
		cleaned = true;
	}
	return true;
}

IComponent * SpriteComponent::makeClone() const {
	SpriteComponent* clone = new SpriteComponent(getID(), name);
	clone->rect = rect;
	clone->offset = offset;
	return clone;
}
