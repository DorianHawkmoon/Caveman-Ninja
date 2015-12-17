#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include "SDL/SDL_rect.h"

class Animation {
public:
	float speed;
	int loop;
	std::vector<SDL_Rect> frames;

	~Animation() {
		frames.clear();
	}

private:
	float current_frame;

public:
	Animation() : frames(), speed(1.0f), current_frame(0.0f), loop(0) {}

	SDL_Rect& GetCurrentFrame() {
		current_frame += speed;
		if (current_frame >= frames.size() & (loop > 0)) {
			current_frame = 0.0f;
			--loop;
		} else if(current_frame >= frames.size()) {
			current_frame -= speed;
		}
		return frames[(int) current_frame];
	}
};

#endif // !ANIMATION_H
