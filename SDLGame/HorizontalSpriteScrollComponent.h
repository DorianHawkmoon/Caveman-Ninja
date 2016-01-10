#pragma once
#include "IComponent.h"
#include "SDL\SDL_rect.h"
#include "Point.h"
#include <string>
struct SDL_Texture;

class HorizontalSpriteScrollComponent :	public IComponent {
public:
	HorizontalSpriteScrollComponent(std::string nameComponent, std::string nameTexture);
	virtual ~HorizontalSpriteScrollComponent();

	bool start();
	update_status preUpdate();
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

	bool cleaned;
};

