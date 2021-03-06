#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include "Point.h"
#include "SDL/SDL.h"
#include <cmath>
#include "IUpdater.h"
#include "Globals.h"

class Animation : public IUpdater{

public:
	Animation(int numberFrames) : numberFrames(numberFrames), sizeFrame(), offset(0,0), speed(1.0f), current_frame(0.0f), repeat(-1), flippedOffset(0,0) {

	}

	virtual ~Animation() {
	}

	bool start() { 
		current_frame = 0;
		actualRepeat = repeat;
		return true;
	};

	update_status update() {
		if (numberFrames <=1) {
			return UPDATE_CONTINUE;
		}

		if (actualRepeat >= 0) {
			if (current_frame + speed > numberFrames) {
				if (actualRepeat > 0) {
					--actualRepeat;
				}
				current_frame = (actualRepeat>0 || actualRepeat<0) ? 0.0f : current_frame;

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
		actualRepeat = repeat;
		return true;
	};

	SDL_Rect GetCurrentFrame() const {
		int frame = static_cast<int>(trunc(current_frame));
		return{sizeFrame.x + (sizeFrame.w*frame), sizeFrame.y, sizeFrame.w, sizeFrame.h};
	}

	bool isFinished() const {
		return actualRepeat == 0;
	}

	bool isInfinity() const {
		return actualRepeat == -1;
	}


private:
	float current_frame;
	int actualRepeat;

public:
	float speed;
	int repeat;
	SDL_Rect sizeFrame;
	int numberFrames;
	iPoint offset;
	iPoint flippedOffset;
};

#endif // !ANIMATION_H
