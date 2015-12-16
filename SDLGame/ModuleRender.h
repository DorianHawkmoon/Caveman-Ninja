#pragma once
#ifndef MODULE_RENDER_H
#define MODULE_RENDER_H

#include "Module.h"
#include "SDL/SDL_rect.h"
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
	bool blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, float speed=1.0f);
	//TODO pintar rectangulo
	//TODO pintar pixeles

public:
	SDL_Renderer* renderer = nullptr;
	SDL_Rect camera;
};


#endif // !MODULE_RENDER_H
