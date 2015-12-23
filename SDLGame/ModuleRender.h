#pragma once
#ifndef MODULE_RENDER_H
#define MODULE_RENDER_H

#include "Module.h"
#include "SDL/SDL.h"
#include "Point.h"
#include "ICollider.h"
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

	//TODO entender que coño hace el método con camera
	//bool blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, float speed=1.0f);
	bool blit(SDL_Texture* texture, iPoint position, SDL_Rect* sectionTexture, float speed = 1.0f);
	bool paintCollision(const ICollider* collision);
	//TODO pintar rectangulo
	bool paintRectangle(const SDL_Color& color, const iPoint& position, const SDL_Rect& rect);
	//TODO pintar pixeles

public:
	SDL_Renderer* renderer;
	Camera camera;
};


#endif // !MODULE_RENDER_H
