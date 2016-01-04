#pragma once
#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include "IComponent.h"
#include <string>
#include "Point.h"
#include "SDL\SDL_rect.h"

struct SDL_Texture;

class SpriteComponent : public IComponent {
public:
	SpriteComponent(std::string nameComponent, std::string nameTexture);
	virtual ~SpriteComponent();

	bool start();
	update_status postUpdate();
	bool cleanUp();

	IComponent* makeClone();

public:
	SDL_Rect rect;
	iPoint offset;
	float speedCamera;

private:
	std::string name;
	SDL_Texture* texture;
};


#endif // !SPRITE_COMPONENT_H
