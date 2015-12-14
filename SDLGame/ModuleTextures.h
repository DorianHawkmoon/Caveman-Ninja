#pragma once
#ifndef MODULE_TEXTURES_H
#define MODULE_TEXTURES_H

#include "Module.h"
#include <list>

struct SDL_Texture;

class ModuleTextures : public Module {
public:
	ModuleTextures();
	virtual ~ModuleTextures();

	bool Init();
	bool CleanUp();

	SDL_Texture* const Load(const char* path);

private:
	std::list<SDL_Texture*> textures;
};

#endif // !MODULE_TEXTURES_H

