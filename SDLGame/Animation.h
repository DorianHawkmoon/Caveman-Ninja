#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include "Point.h"
#include "SDL/SDL.h"
#include <math.h>
#include "IUpdater.h"
#include "Globals.h"

class Animation : public IUpdater{

public:
	Animation(int numberFrames) : numberFrames(numberFrames), sizeFrame(), offset(0,0), speed(1.0f), current_frame(0.0f), repeat(-1) {

	}

	virtual ~Animation() {
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


private:
	float current_frame;

public:
	float speed;
	int repeat;
	SDL_Rect sizeFrame;
	int numberFrames;
	iPoint offset;
};

#endif // !ANIMATION_H
