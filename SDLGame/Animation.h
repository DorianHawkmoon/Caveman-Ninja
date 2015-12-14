#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include "SDL\SDL_rect.h"

class Animation {
public:
	float speed;
	std::vector<SDL_Rect> frames;

private:
	float current_frame;

public:
	Animation() : frames(), speed(1.0f), current_frame(0.0f) {}

	SDL_Rect& GetCurrentFrame() {
		current_frame += speed;
		if (current_frame >= frames.size())
			current_frame = 0.0f;
		return frames[(int) current_frame];
	}
};

#endif // !ANIMATION_H
