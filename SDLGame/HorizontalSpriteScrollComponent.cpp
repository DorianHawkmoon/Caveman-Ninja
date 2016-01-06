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
	LOG("destroy one component");
	//unload just in case
	cleanUp();
}

bool HorizontalSpriteScrollComponent::start() {
	texture = App->textures->load(name.c_str());
	texture = App->textures->load(name.c_str());

	float widthWindow = (float)App->renderer->camera.getWindowsSize().w;
	int widthSprite = static_cast<int>(rect.w * SCREEN_SIZE);
	float result = widthWindow / widthSprite;
	numberToCover = static_cast<int>(ceil(result));
	numberToCover++;
	return texture != nullptr;
}

update_status HorizontalSpriteScrollComponent::update() {
	SDL_Rect cam = App->renderer->camera.getViewArea();
	int xCam = static_cast<int>(cam.x * speedCamera); //actual position of camera
									  
	//actual absolute position of sprite
	fPoint fpos = parent->transform->getGlobalTransform().position;
	int xPosition = static_cast<int>(fpos.x + rect.w*scrollingOffset);
	int xPositionWidth = static_cast<int>(fpos.x + rect.w*(scrollingOffset+1));

	xPosition = (int) xPosition * SCREEN_SIZE - xCam; //coordenada local
	xPositionWidth = (int) xPositionWidth * SCREEN_SIZE - xCam; //coordenada local

	if (xPositionWidth < 0) {
		scrollingOffset++;
	} else if (xPosition >= 0 ) {
		scrollingOffset--;
	}
	return UPDATE_CONTINUE;
}

update_status HorizontalSpriteScrollComponent::postUpdate() {
	//paint the sprite
	Transform global = parent->transform->getGlobalTransform();
	global.position.x += offset.x;
	global.position.y += offset.y;

	global.position.x += rect.w*(scrollingOffset-1);
	App->renderer->blit(texture, global, &rect, speedCamera);
	global.position.x += rect.w;
	for (int i = 0; i < numberToCover; ++i) {
		App->renderer->blit(texture, global, &rect, speedCamera);
		global.position.x += rect.w;
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
	clone->offset = offset;
	clone->offset = offset;
	clone->speedCamera = speedCamera;
	clone->scrollingOffset = scrollingOffset;
	clone->numberToCover = numberToCover;
	return clone;
}