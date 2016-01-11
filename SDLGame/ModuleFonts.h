#pragma once
#ifndef MODULE_FONTS_H
#define MODULE_FONTS_H

#include "Module.h"
#include "SDL_ttf/SDL_ttf.h"
#include <string>
#include <list>

struct LoadedFont {
	LoadedFont( const std::string& name, TTF_Font* font, int size) : name(name), font(font), count(1), size(size) {}
	~LoadedFont() {
		TTF_CloseFont(font);
	}

	const std::string name;
	int size;
	int count;
	TTF_Font* font;
};

class ModuleFonts : public Module {
public:
	ModuleFonts(bool started = true) : Module(started), fonts() {}
	virtual ~ModuleFonts();

	bool init();
	bool cleanUp();

	TTF_Font* const load(const char* path, int size);
	void unload(TTF_Font* texture, int size);

private:
	std::list<LoadedFont*> fonts;
};

#endif // !MODULE_FONTS_H

