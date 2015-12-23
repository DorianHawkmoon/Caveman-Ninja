#include "VerticalSpriteScrollComponent.h"

#include "HorizontalSpriteScrollComponent.h"
#include "Application.h"
#include "SDL\SDL.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "Camera.h"
#include "Entity.h"
#include <math.h>

VerticalSpriteScrollComponent::VerticalSpriteScrollComponent(std::string nameComponent, std::string nameTexture)
	: IComponent(nameComponent), name(nameTexture), scrollingOffset(0), offset(0, 0), speedCamera(1.0f),
	numberToCover(0) {}


VerticalSpriteScrollComponent::~VerticalSpriteScrollComponent() {
	//unload just in case
	cleanUp();
}

bool VerticalSpriteScrollComponent::start() {
	texture = App->textures->load(name.c_str());
	float widthWindow = App->renderer->camera.getWindowsSize().w;
	int widthSprite = rect.h * SCREEN_SIZE;
	float result = widthWindow / widthSprite;
	numberToCover = ceil(result);
	numberToCover++;
	return texture != nullptr;
}

update_status VerticalSpriteScrollComponent::update() {
	SDL_Rect cam = App->renderer->camera.getViewArea();
	int yCam = cam.y * speedCamera; //actual position of camera

									//actual absolute position of sprite
	int yPosition = parent->transform.position.x + rect.h*scrollingOffset;
	int yPositionWidth = parent->transform.position.x + rect.h*(scrollingOffset + 1);

	yPosition = (int) yPosition * SCREEN_SIZE - yCam; //coordenada local
	yPositionWidth = (int) yPositionWidth * SCREEN_SIZE - yCam; //coordenada local

	if (yPositionWidth < 0) {
		scrollingOffset++;
	} else if (yPosition >= 0) {
		scrollingOffset--;
	}
	return UPDATE_CONTINUE;
}

update_status VerticalSpriteScrollComponent::postUpdate() {
	//paint the sprite
	iPoint finalPosition(0, 0);
	finalPosition.x = parent->transform.position.x + offset.x;
	finalPosition.y = parent->transform.position.y + offset.y;

	finalPosition.x += rect.h*(scrollingOffset - 1);
	App->renderer->blit(texture, finalPosition, &rect, speedCamera, 1);
	finalPosition.x += rect.h;
	for (int i = 0; i < numberToCover; ++i) {
		App->renderer->blit(texture, finalPosition, &rect, speedCamera, 1);
		finalPosition.x += rect.h;
	}
	return UPDATE_CONTINUE;
}

bool VerticalSpriteScrollComponent::cleanUp() {
	App->textures->unload(texture);
	return true;
}

IComponent * VerticalSpriteScrollComponent::makeClone() {
	VerticalSpriteScrollComponent* clone = new VerticalSpriteScrollComponent(getID(), name);
	clone->rect = rect;
	clone->offset = offset;
	clone->speedCamera = speedCamera;
	clone->scrollingOffset = scrollingOffset;
	clone->numberToCover = numberToCover;
	return clone;
}