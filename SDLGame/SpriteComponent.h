#pragma once
#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include "IComponent.h"
#include <string>
#include "Point.h"
#include "SDL\SDL_rect.h"
#include "ModuleTextures.h"
#include "Application.h"
#include "ModuleRender.h"

struct SDL_Texture;

class SpriteComponent : public IComponent {
public:
	SpriteComponent(std::string nameComponent, std::string nameTexture) : IComponent(nameComponent), name(nameTexture) {}
	~SpriteComponent();

	bool start() {
		texture = App->textures->load(name.c_str());
		return texture != nullptr;
	};
	update_status preUpdate() {
		return UPDATE_CONTINUE;
	};
	update_status update() {
		return UPDATE_CONTINUE;
	};
	update_status postUpdate() {
		//paint the sprite
		App->renderer->blit(texture, 20,20, &rect);
		return UPDATE_CONTINUE;
	};
	bool cleanUp() {
		App->textures->unload(texture);
		return true;
	};

	IComponent* makeClone() {
		SpriteComponent* clone = new SpriteComponent(getID(), name);
		clone->rect = rect;
		clone->offset = offset;
		return clone;
	}

public:
	SDL_Rect rect;
	iPoint offset;

private:
	std::string name;
	SDL_Texture* texture;
};


#endif // !SPRITE_COMPONENT_H
