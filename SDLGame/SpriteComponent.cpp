#include "SpriteComponent.h"
#include "Application.h"
#include "ModuleRender.h"
#include "SDL\SDL.h"
#include "ModuleTextures.h"
#include "Transform.h"
#include "Entity.h"

SpriteComponent::SpriteComponent(std::string nameComponent, std::string nameTexture)
	: IComponent(nameComponent), name(nameTexture), speedCamera(1.0f), offset(0, 0) {}

SpriteComponent::~SpriteComponent() {
	//unload just in case
	cleanUp();
}

bool SpriteComponent::start() {
	texture = App->textures->load(name.c_str());
	return texture != nullptr;
}

update_status SpriteComponent::postUpdate() {
	//paint the sprite
	iPoint finalPosition;
	finalPosition.x= static_cast<int>(parent->transform.position.x + offset.x);
	finalPosition.y = static_cast<int>(parent->transform.position.y + offset.y);
	
	App->renderer->blit(texture, finalPosition, &rect, speedCamera);
	return UPDATE_CONTINUE;
}

bool SpriteComponent::cleanUp() {
	App->textures->unload(texture);
	return true;
}

IComponent * SpriteComponent::makeClone() {
	SpriteComponent* clone = new SpriteComponent(getID(), name);
	clone->rect = rect;
	clone->offset = offset;
	return clone;
}
