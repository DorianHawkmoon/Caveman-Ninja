#include "ModuleAudio.h"
#include "Globals.h"
#include "SDL\SDL.h"
#include <string>
#include "SDL_mixer\SDL_mixer.h"

ModuleAudio::ModuleAudio(bool started): Module(started) {}

// Destructor
ModuleAudio::~ModuleAudio() {
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

// Called before render is available
bool ModuleAudio::init() {
	LOG("init Mixer library");
	bool ret = true;

	// load support for the MP3 and OGG
	int flags = MIX_INIT_MP3 | MIX_INIT_OGG;
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
bool ModuleAudio::cleanUp() {
	LOG("Freeing music and effects music library");

	if (music != nullptr) {
		Mix_FreeMusic(music);
	}

	for (std::vector<Mix_Chunk*>::iterator it = effects.begin(); it != effects.end(); ++it)
		Mix_FreeChunk(*it);

	effects.clear();
	Mix_CloseAudio();
	return true;
}

bool const ModuleAudio::playMusic(const char * path, float fade_time) {
	bool result = true;

	if (music != nullptr) {
		if (fade_time > 0.0f) {
			Mix_FadeOutMusic((int) (fade_time * 1000.0f));
		} else {
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = loadMusic(path);

	if (music == nullptr) {
		LOG("Cannot load music %s.\n");
		result = false;

	} else {
		if (fade_time > 0.0f) {
			if (Mix_FadeInMusic(music, -1, (int) (fade_time * 1000.0f)) < 0) {
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				result = false;
			}

		} else {
			if (Mix_PlayMusic(music, -1) < 0) {
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				result  = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return result;
}

unsigned int const ModuleAudio::loadEffect(const char * path) {
	unsigned int result = 0;
	std::string finalPath = "audio/effects/";
	finalPath.append(path);
	Mix_Chunk* effect= Mix_LoadWAV(finalPath.c_str());
	if (effect == nullptr) {
		LOG("Could not load music with path: %s. Mixer_load: %s", path, Mix_GetError());

	}else {
		effects.push_back(effect);
		result= effects.size() - 1;
	}
	return result;
}

bool ModuleAudio::playEffect(unsigned int fx, int repeat) {
	bool result = false;

	if (fx < effects.size()) {
		Mix_PlayChannel(-1, effects[fx], repeat);
		result = true;
	}

	return result;
}

Mix_Music* const ModuleAudio::loadMusic(const char* path) {
	std::string finalPath = "audio/music/";
	finalPath.append(path);
	Mix_Music* musicLoaded = Mix_LoadMUS(finalPath.c_str());
	
	if (musicLoaded == nullptr) {
		LOG("Could not load music with path: %s. Mixer_load: %s", path, Mix_GetError());
	}

	return musicLoaded;
}
