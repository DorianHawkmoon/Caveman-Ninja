#pragma once
#ifndef MODULE_AUDIO_H
#define MODULE_AUDIO_H

#include "Module.h"
#include "SDL_mixer\SDL_mixer.h" //included here because declaring the struct makes an error (line 131 of SDL_mixer)
#include <list>

class ModuleAudio :	public Module {
public:
	ModuleAudio();
	virtual ~ModuleAudio();

	bool Init();
	bool CleanUp();

	Mix_Music* const LoadMusic(const char* path);
	Mix_Chunk* const LoadEffect(const char* path);

private:
	std::list<Mix_Music*> music;
	std::list<Mix_Chunk*> effects;
};


#endif // !MODULE_AUDIO_H
