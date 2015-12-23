#pragma once
#ifndef VERTICAL_SPRITE_SCROLL_COMPONENT_H
#define VERTICAL_SPRITE_SCROLL_COMPONENT_H
#include "IComponent.h"
#include <string>

class VerticalSpriteScrollComponent :
	public IComponent {
public:
	VerticalSpriteScrollComponent(std::string nameComponent, std::string nameTexture);
	~VerticalSpriteScrollComponent();

	bool start();
	update_status update();
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

	int scrollingOffset;
	int numberToCover;
};

#endif // VERTICAL_SPRITE_SCROLL_COMPONENT_H