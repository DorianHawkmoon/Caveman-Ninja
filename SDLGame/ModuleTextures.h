#pragma once
#ifndef MODULE_TEXTURES_H
#define MODULE_TEXTURES_H

#include "Module.h"
#include <list>

struct SDL_Texture;

class ModuleTextures : public Module {
public:
	ModuleTextures(bool started = true);
	virtual ~ModuleTextures();

	bool init();
	bool cleanUp();

	SDL_Texture* const load(const char* path);
	void unload(SDL_Texture* texture);

private:
	std::list<SDL_Texture*> textures;
};

#endif // !MODULE_TEXTURES_H

