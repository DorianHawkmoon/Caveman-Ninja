#include "VerticalSpriteScrollComponent.h"

#include "HorizontalSpriteScrollComponent.h"
#include "Application.h"
#include "SDL/SDL.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "Camera.h"
#include "Entity.h"
#include <cmath>

VerticalSpriteScrollComponent::VerticalSpriteScrollComponent(const std::string& nameComponent, const std::string& nameTexture)
	: IComponent(nameComponent), name(nameTexture), scrollingOffset(0), offset(0, 0), speedCamera(1.0f),
	numberToCover(0), cleaned(true) {}


VerticalSpriteScrollComponent::~VerticalSpriteScrollComponent() {
	//unload just in case
	cleanUp();
}

bool VerticalSpriteScrollComponent::start() {
	if (cleaned) {
		texture = App->textures->load(name.c_str());
		int widthWindow = App->renderer->camera.getWindowsSize().w;
		int widthSprite = rect.h * SCREEN_SIZE;
		float result = ((float) widthWindow) / widthSprite;
		numberToCover = static_cast<int>(ceil(result));
		numberToCover++;
		cleaned = !(texture != nullptr);
		return texture != nullptr;
	} else {
		return true;
	}
}

update_status VerticalSpriteScrollComponent::preUpdate() {
	if (toClean) {
		cleanUp();
		toClean = false;
	}
	return UPDATE_CONTINUE;
}

update_status VerticalSpriteScrollComponent::update() {
	SDL_Rect cam = App->renderer->camera.getViewArea();
	int yCam = static_cast<int>(cam.y * speedCamera); //actual position of camera

	Transform global = parent->transform->getGlobalTransform();
									//actual absolute position of sprite
	int yPosition = static_cast<int>(global.position.x + rect.h*scrollingOffset);
	int yPositionWidth = static_cast<int>(global.position.x + rect.h*(scrollingOffset + 1));

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
	Transform global = parent->transform->getGlobalTransform();
	global.position.x += offset.x;
	global.position.y += offset.y;

	global.position.y += rect.h*(scrollingOffset - 1);
	App->renderer->blit(texture, global, &rect, speedCamera);
	global.position.y += rect.h;
	for (int i = 0; i < numberToCover; ++i) {
		App->renderer->blit(texture, global, &rect, speedCamera);
		global.position.y += rect.h;
	}
	return UPDATE_CONTINUE;
}

bool VerticalSpriteScrollComponent::cleanUp() {
	if (!cleaned) {
		App->textures->unload(texture);
		cleaned = true;
	}
	return true;
}

IComponent * VerticalSpriteScrollComponent::makeClone() const {
	VerticalSpriteScrollComponent* clone = new VerticalSpriteScrollComponent(getID(), name);
	clone->rect = rect;
	clone->offset = offset;
	clone->speedCamera = speedCamera;
	clone->scrollingOffset = scrollingOffset;
	clone->numberToCover = numberToCover;
	return clone;
}