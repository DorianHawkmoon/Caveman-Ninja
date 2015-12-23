#include "HorizontalSpriteScrollComponent.h"
#include "Application.h"
#include "SDL\SDL.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "Camera.h"
#include "Entity.h"
#include <math.h>

HorizontalSpriteScrollComponent::HorizontalSpriteScrollComponent(std::string nameComponent, std::string nameTexture) 
	: IComponent(nameComponent), name(nameTexture), scrollingOffset(0), offset(0,0), speedCamera(1.0f),
	numberToCover(0){}


HorizontalSpriteScrollComponent::~HorizontalSpriteScrollComponent() {
	//unload just in case
	cleanUp();
}

bool HorizontalSpriteScrollComponent::start() {
	texture = App->textures->load(name.c_str());
	float widthWindow = App->renderer->camera.getWindowsSize().w;
	int widthSprite = rect.w * SCREEN_SIZE * speedCamera;
	float result = widthWindow / widthSprite;
	numberToCover = ceil(result);
	numberToCover+=2;
	return texture != nullptr;
}

update_status HorizontalSpriteScrollComponent::update() {
	SDL_Rect cam = App->renderer->camera.getViewArea();
	int xCam = cam.x * speedCamera; //actual position of camera
									  
	//actual position of sprite
	int xPosition = parent->transform.position.x + scrollingOffset;
	int xPositionWidth = xPosition + rect.w;
	
	xPositionWidth = (int) xPositionWidth * SCREEN_SIZE - cam.x; //coordenada local
	xPosition = (int) xPosition * SCREEN_SIZE - cam.x; //coordenada local

	if (xPositionWidth < 0) {
		scrollingOffset += rect.w;

	} else if (xPosition > 0 ) {
		scrollingOffset -= rect.w;
	}
	return UPDATE_CONTINUE;
}

update_status HorizontalSpriteScrollComponent::postUpdate() {
	//paint the sprite
	iPoint finalPosition;
	finalPosition.x = parent->transform.position.x + offset.x + scrollingOffset;
	finalPosition.y = parent->transform.position.y + offset.y;

	for (int i = 0; i < numberToCover; ++i) {
		finalPosition.x += (rect.w * i);
		App->renderer->blit(texture, finalPosition, &rect, speedCamera);
	}
	return UPDATE_CONTINUE;
}

bool HorizontalSpriteScrollComponent::cleanUp() {
	App->textures->unload(texture);
	return true;
}

IComponent * HorizontalSpriteScrollComponent::makeClone() {
	HorizontalSpriteScrollComponent* clone = new HorizontalSpriteScrollComponent(getID(), name);
	clone->rect = rect;
	//TODO hacer el clone
	clone->offset = offset;
	return clone;
}