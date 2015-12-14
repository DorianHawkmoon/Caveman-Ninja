#include "ModuleAudio.h"
#include "Globals.h"
#include "SDL\SDL.h"
#include <string>
#include "SDL_mixer\SDL_mixer.h"

ModuleAudio::ModuleAudio() {}

// Destructor
ModuleAudio::~ModuleAudio() {
	Mix_Quit();
}

// Called before render is available
bool ModuleAudio::Init() {
	LOG("Init Mixer library");
	bool ret = true;

	// load support for the PNG image format
	int flags = MIX_INIT_MP3;
	int init = MIX_InitFlags(flags);

	if ((init & flags) != flags) {
		LOG("Could not initialize Mixer lib. Mix_init: %s", Mix_GetError());
		ret = false;
	}
	else if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
			LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
			ret = false;
		
	}

	return ret;
}

// Called before quitting
bool ModuleAudio::CleanUp() {
	LOG("Freeing music and effects music library");

	for (std::list<Mix_Music*>::iterator it = music.begin(); it != music.end(); ++it)
		Mix_FreeMusic(*it);

	music.clear();

	for (std::list<Mix_Chunk*>::iterator it = effects.begin(); it != effects.end(); ++it)
		Mix_FreeChunk(*it);

	effects.clear();
	return true;
}

Mix_Music* const ModuleAudio::LoadMusic(const char* path) {
	std::string finalPath = "audio/";
	finalPath.append(path);
	Mix_Music* musicLoaded = Mix_LoadMUS(finalPath.c_str());
	if (musicLoaded == nullptr) {
		LOG("Could not load music with path: %s. Mixer_Load: %s", path, Mix_GetError());
	}
	else {
		music.push_back(musicLoaded);
	}

	return musicLoaded;
}

Mix_Chunk* const ModuleAudio::LoadEffect(const char* path) {
	std::string finalPath = "audio/";
	finalPath.append(path);
	Mix_Chunk* effect= Mix_LoadWAV(finalPath.c_str());
	if (effect == nullptr) {
		LOG("Could not load music with path: %s. Mixer_Load: %s", path, Mix_GetError());
	}
	else {
		effects.push_back(effect);
	}

	return effect;
}
