#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include "Point.h"
#include "SDL/SDL.h"
#include <math.h>
#include "Globals.h"
//TODO check every include guards
class Animation {

public:
	Animation(int numberFrames) : numberFrames(numberFrames), sizeFrame(), offset(numberFrames), speed(1.0f), current_frame(0.0f), repeat(-1), flippedOffset(0,0) {
		for (int i = 0; i < numberFrames; ++i) {
			offset[i].x = 0;
			offset[i].y = 0;
		}
	}

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
				if (repeat > 0) {
					--repeat;
				}
				current_frame = (repeat>0 || repeat<0) ? 0.0f : current_frame;

			} else {
				current_frame += speed;
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

	SDL_Rect GetCurrentFrame() const {
		int frame = static_cast<int>(trunc(current_frame));
		return{sizeFrame.x + (sizeFrame.w*frame), sizeFrame.y, sizeFrame.w, sizeFrame.h};
	}

	const iPoint& getCurrentOffset() const {
		int frame = static_cast<int>(trunc(current_frame));
		return offset[frame];
	}


private:
	float current_frame;

public:
	//TODO add an offset for every frame
	float speed;
	int repeat;
	iPoint flippedOffset;
	SDL_Rect sizeFrame;
	int numberFrames;
	std::vector<iPoint> offset;
};

#endif // !ANIMATION_H
