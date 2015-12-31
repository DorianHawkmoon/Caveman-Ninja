#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include "Point.h"
#include "SDL/SDL_rect.h"
#include <math.h>
#include "Globals.h"
//TODO check every include guards
class Animation {

public:
	Animation(int numberFrames) : numberFrames(numberFrames), sizeFrame(), offset(numberFrames), speed(1.0f), current_frame(0.0f), repeat(-1) {}

	~Animation() {
		offset.clear();
	}

	bool start() { 
		current_frame = 0; 
		return true;
	};

	update_status update() {
		if (numberFrames <=1) {
			return UPDATE_CONTINUE;
		}

		if (repeat >= 0) {
			if (current_frame + speed > numberFrames) {
				current_frame = (repeat>0 || repeat<0) ? 0.0f : current_frame + speed;
			}

			if (repeat > 0) {
				--repeat;
			}
		} else {
			current_frame = (current_frame + speed > numberFrames) ? 0.0f : current_frame + speed;

		}

		return UPDATE_CONTINUE;
	};

	bool cleanUp() {
		current_frame = 0;
		return true;
	};

	SDL_Rect GetCurrentFrame() {
		int frame = static_cast<int>(trunc(current_frame));
		return{sizeFrame.x + (sizeFrame.w*frame), sizeFrame.y, sizeFrame.w, sizeFrame.h};
	}



private:
	float current_frame;

public:
	//TODO add an offset for every frame
	float speed;
	int repeat;
	SDL_Rect sizeFrame;
	int numberFrames;
	std::vector<iPoint> offset;
};

#endif // !ANIMATION_H
