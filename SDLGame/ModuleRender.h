#pragma once
#ifndef MODULE_RENDER_H
#define MODULE_RENDER_H

#include "Module.h"
#include "SDL/SDL.h"
#include "Point.h"
#include "Collider.h"
#include "Camera.h"
struct SDL_Texture;
struct SDL_Renderer;

class ModuleRender : public Module {
public:
	ModuleRender(bool started = true);
	virtual ~ModuleRender();

	bool init();
	bool start();
	update_status preUpdate();
	update_status update();
	update_status postUpdate();
	bool cleanUp();

	bool blit(SDL_Texture* texture, const iPoint& position, const SDL_Rect* sectionTexture,  const iPoint& offsetImage, float speed = 1.0f, const SDL_RendererFlip& flip = SDL_FLIP_NONE);
	bool paintCollision(const Collider* collision);
	bool paintRectangle(const SDL_Color& color, const iPoint& position, const iPoint& rect, float speed=1.0f);
	bool paintCircle(const SDL_Color& color, const fPoint& position, float radius, float speed = 1.0f);
	//TODO pintar pixeles

public:
	SDL_Texture* circle;
	SDL_Renderer* renderer;
	Camera camera;
};


#endif // !MODULE_RENDER_H
