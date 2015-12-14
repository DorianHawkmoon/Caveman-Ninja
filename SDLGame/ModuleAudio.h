#pragma once
#ifndef MODULE_AUDIO_H
#define MODULE_AUDIO_H

#include "Module.h"
#include "SDL_mixer\SDL_mixer.h" //included here because declaring the struct makes an error (line 131 of SDL_mixer)
#include <vector>

#define DEFAULT_MUSIC_FADE_TIME 2.0f

class ModuleAudio :	public Module {
public:
	ModuleAudio(bool started=true);
	virtual ~ModuleAudio();

	bool init();
	bool cleanUp();

	bool const playMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);
	unsigned int const loadEffect(const char* path);
	bool playEffect(unsigned int fx, int repeat = 0);

private:
	Mix_Music* const ModuleAudio::loadMusic(const char* path);

private:
	Mix_Music* music;
	std::vector<Mix_Chunk*> effects;
};


#endif // !MODULE_AUDIO_H
