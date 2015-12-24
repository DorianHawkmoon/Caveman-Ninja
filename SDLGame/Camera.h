#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "SDL\SDL_rect.h"
#include "Point.h"
#include "Globals.h"

class Camera {
public:
	Camera(int width, int height) : camera({0,0,width,height}) {

	}
	~Camera() {}

	inline void setX(int x) {
		camera.x = x;
	}

	inline void setY(int y) {
		camera.y = y;
	}

	inline int getX(float speed = 1.0f) const {
		return static_cast<int>(camera.x*speed);
	}

	inline int getY(float speed=1.0f) const {
		return static_cast<int>(camera.y*speed);
	}

	SDL_Rect getWindowsSize() const;

	SDL_Rect getViewArea(float speed = 1.0f) const;

private:
	SDL_Rect camera;
};


#endif // !CAMERA_H
