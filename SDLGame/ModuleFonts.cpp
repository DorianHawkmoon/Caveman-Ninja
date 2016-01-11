#include "ModuleFonts.h"
#include <algorithm>


ModuleFonts::~ModuleFonts() {
	TTF_Quit();
}

bool ModuleFonts::init() {
	LOG("init Fonts library");
	bool ret = true;

	if (TTF_Init()) {
		LOG("Could not initialize Fonts lib. TTF_init: %s", TTF_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleFonts::cleanUp() {
	LOG("Freeing fonts and Font library");

	for (std::list<LoadedFont*>::iterator it = fonts.begin(); it != fonts.end(); ++it) {
		delete (*it);
	}

	fonts.clear();
	return true;
}

TTF_Font * const ModuleFonts::load(const char * path, int size) {
	TTF_Font* font = nullptr;
	std::string finalPath = "fonts/";
	finalPath.append(path);

	//search for the texture
	auto it = std::find_if(fonts.begin(), fonts.end(),
		[&path](LoadedFont* text) { return text->name.compare(path) == 0; });

	//if found
	if (it != fonts.end()) {
		(*it)->count++; //increment counter
		font = (*it)->font;

	} else {
		//if not load it
		font = TTF_OpenFont(finalPath.c_str(), size);
		if (font == nullptr) {
			LOG("Could not load font with path: %s. TTF_OpenFont: %s", path, TTF_GetError());
		} else {
				LoadedFont* newFont = new LoadedFont(path, font, size);
				fonts.push_back(newFont);
		}
	}

	return font;
}

void ModuleFonts::unload(TTF_Font * font, int size) {
	//search for the texture
	auto it = std::find_if(fonts.begin(), fonts.end(),
		[&font, size](LoadedFont* loaded) { return loaded->font==font && loaded->size==size; });

	if (it != fonts.end()) {
		LoadedFont* loaded = *it;
		if (loaded->count > 1) {
			loaded->count--;
		} else {
			delete loaded;
			fonts.erase(it);
		}
	}
}
