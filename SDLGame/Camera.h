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

	int getX() const {
		return camera.x;
	}

	int getY() const {
		return camera.y;
	}

	inline SDL_Rect getWindowsSize() {
		SDL_Rect sizeWindows;
		sizeWindows.x = 0;
		sizeWindows.y = 0;
		sizeWindows.w = camera.w;
		sizeWindows.h = camera.h;
		return sizeWindows;
	}

	SDL_Rect getViewArea() const {
		SDL_Rect area;
		area.x = camera.x;
		area.y = camera.y;
		area.w = camera.w;
		area.h = camera.h;
		return area;
	}

private:
	SDL_Rect camera;
};


#endif // !CAMERA_H
