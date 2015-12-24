#pragma once
#ifndef MODULE_TEXTURES_H
#define MODULE_TEXTURES_H

#include "Module.h"
#include <list>
#include <vector>
#include "SDL_image\SDL_image.h"
#include "SDL\SDL.h"

struct LoadedTexture {
	LoadedTexture(const std::string& name, SDL_Texture* texture) : name(name), texture(texture),count(1) {}
	LoadedTexture() {
		SDL_DestroyTexture(texture);
	}

	const std::string name;
	int count;
	SDL_Texture* texture;
};

class ModuleTextures : public Module {
public:
	ModuleTextures(bool started = true);
	virtual ~ModuleTextures();

	bool init();
	bool cleanUp();

	SDL_Texture* const load(const char* path);
	void unload(SDL_Texture* texture);

private:
	//TODO comprobar como se alloca dinamicamente string y si es mejor a punteros o no
	std::list<LoadedTexture*> textures;

};

#endif // !MODULE_TEXTURES_H

