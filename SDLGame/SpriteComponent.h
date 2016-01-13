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
	SpriteComponent(const std::string& nameComponent, const std::string& nameTexture);
	virtual ~SpriteComponent();

	virtual bool start();
	virtual update_status preUpdate();
	virtual update_status postUpdate();
	virtual bool cleanUp();

	IComponent* makeClone() const;

public:
	SDL_Rect rect;
	iPoint offset;
	iPoint flippedOffset;
	float speedCamera;

private:
	const std::string name;
	SDL_Texture* texture;

	bool cleaned;
};


#endif // !SPRITE_COMPONENT_H
