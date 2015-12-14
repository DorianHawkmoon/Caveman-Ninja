#pragma once
#ifndef MODULE_RENDER_H
#define MODULE_RENDER_H

#include "Module.h"
struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module {
public:
	ModuleRender();
	virtual ~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	bool Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section);

public:
	SDL_Renderer* renderer = nullptr;
};


#endif // !MODULE_RENDER_H
