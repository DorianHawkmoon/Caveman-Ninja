#include "ModuleTextures.h"
#include "SDL_image/SDL_image.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include <algorithm>

ModuleTextures::ModuleTextures(bool started): Module(started) {}


ModuleTextures::~ModuleTextures() {
	IMG_Quit();
}

bool ModuleTextures::init() {
	LOG("init Image library");
	bool ret = true;

	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if ((init & flags) != flags) {
		LOG("Could not initialize Image lib. IMG_init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleTextures::cleanUp() {
	LOG("Freeing textures and Image library");

	for (std::list<LoadedTexture*>::iterator it = textures.begin(); it != textures.end(); ++it) {
		delete (*it);
	}

	textures.clear();
	return true;
}

SDL_Texture* const ModuleTextures::load(const char* path) {
	//TODO llevar un conteo de quien tiene la textura para saber si descargarla o no
	SDL_Texture* texture = nullptr;
	std::string finalPath = "images/";
	finalPath.append(path);
	
	//search for the texture
	std::list<LoadedTexture*>::iterator it = std::find_if(textures.begin(), textures.end(),
		[&path](LoadedTexture* text) { return text->name.compare(path) == 0; });

		//if found
	if (it != textures.end()) {
		(*it)->count++; //increment counter
		texture = (*it)->texture;

	} else {
		//if not load it
		SDL_Surface* surface = IMG_Load(finalPath.c_str());
		if (surface == nullptr) {
			LOG("Could not load surface with path: %s. IMG_load: %s", path, IMG_GetError());
		} else {
			//create the texture and store it
			texture = SDL_CreateTextureFromSurface(App->renderer->renderer, surface);
			if (texture == nullptr) {
				LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
			} else {
				LoadedTexture* newTexture = new LoadedTexture(path, texture);
				textures.push_back(newTexture);
			}
		}
		SDL_FreeSurface(surface);
	}
	
	return texture;
}

void ModuleTextures::unload(SDL_Texture * texture) {
	//search for the texture
	std::list<LoadedTexture*>::iterator it = std::find_if(textures.begin(), textures.end(),
		[&texture](LoadedTexture* loaded) { return loaded->texture == texture; });

	if (it != textures.end()) {
		LoadedTexture* loaded = *it;
		if (loaded->count > 1) {
			loaded->count--;
		} else {
			delete loaded;
			textures.erase(it);
		}
	}
}
