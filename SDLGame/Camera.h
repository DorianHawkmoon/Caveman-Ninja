#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "SDL\SDL_rect.h"
#include "Point.h"
#include "Globals.h"
struct Transform;

class Camera {
public:
	Camera(const iPoint& size) : size(size),cameraDebug(0,0) {

	}
	virtual ~Camera() {}

	void setCamera(const Transform* camera) {
		transform = camera;
	}

	inline void setX(int x) {
		cameraDebug.x = x;
	}
	inline void setY(int y) {
		cameraDebug.y = y;
	}

	int getX(float speed = 1.0f) const;
	int getY(float speed = 1.0f) const;

	SDL_Rect getWindowsSize() const;

	SDL_Rect getViewArea(float speed = 1.0f) const;

public:
	iPoint cameraDebug;

private:
	const Transform* transform;
	iPoint size;
};


#endif // !CAMERA_H
