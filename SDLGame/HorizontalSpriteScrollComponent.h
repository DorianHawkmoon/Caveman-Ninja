#pragma once
#include "IComponent.h"
#include "SDL/SDL_rect.h"
#include "Point.h"
#include <string>
struct SDL_Texture;

class HorizontalSpriteScrollComponent :	public IComponent {
public:
	HorizontalSpriteScrollComponent(const std::string& nameComponent, const std::string& nameTexture);
	virtual ~HorizontalSpriteScrollComponent();

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
	const std::string name;
	SDL_Texture* texture;

	int scrollingOffset;
	int numberToCover;

	bool cleaned;
};

