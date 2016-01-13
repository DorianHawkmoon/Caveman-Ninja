#pragma once
#ifndef VERTICAL_SPRITE_SCROLL_COMPONENT_H
#define VERTICAL_SPRITE_SCROLL_COMPONENT_H

#include "IComponent.h"
#include <string>
#include "SDL/SDL.h"
#include "Point.h"

class VerticalSpriteScrollComponent : public IComponent {
public:
	VerticalSpriteScrollComponent(std::string nameComponent, std::string nameTexture);
	virtual ~VerticalSpriteScrollComponent();

	virtual bool start();
	virtual update_status preUpdate();
	virtual update_status update();
	virtual update_status postUpdate();
	virtual bool cleanUp();

	virtual IComponent* makeClone() const;

public:
	SDL_Rect rect;
	iPoint offset;
	float speedCamera;

private:
	std::string name;
	SDL_Texture* texture;

	int scrollingOffset;
	int numberToCover;

	bool cleaned;
};

#endif // VERTICAL_SPRITE_SCROLL_COMPONENT_H