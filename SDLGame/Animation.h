#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include "SDL/SDL_rect.h"

class Animation {
public:
	//TODO make an animator with a state machine
	//TODO add an offset for every frame
	float speed;
	int loop;
	std::vector<SDL_Rect> frames;

	~Animation() {
		frames.clear();
	}

private:
	float current_frame;

public:
	Animation() : frames(), speed(1.0f), current_frame(0.0f), loop(-1) {}

	SDL_Rect& GetCurrentFrame() {
		current_frame += speed;
		//TODO improve loops
		if (current_frame >= frames.size() && (loop > 1 || loop<=-1)) {
			current_frame = 0.0f;
			--loop;
		} else if(current_frame >= frames.size()) {
			current_frame -= speed;
		}
		return frames[(int) current_frame];
	}
};

#endif // !ANIMATION_H
