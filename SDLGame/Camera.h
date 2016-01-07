#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "SDL\SDL_rect.h"
#include "Point.h"
#include "Globals.h"
struct Transform;

class Camera {
public:
	Camera(const iPoint& size) : size(size),offset(0,0) {

	}
	virtual ~Camera() {}

	void setCamera(const Transform* camera) {
		transform = camera;
	}

	int getX(float speed = 1.0f) const;
	int getY(float speed = 1.0f) const;

	SDL_Rect getWindowsSize() const;

	SDL_Rect getViewArea(float speed = 1.0f) const;

public:
	iPoint offset;
	iPoint leftLimit;
	iPoint rightLimit;

private:
	const Transform* transform;
	iPoint size;
};


#endif // !CAMERA_H
