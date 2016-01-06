#include "Camera.h"
#include "Transform.h"

int Camera::getX(float speed) const {
	float x = (transform == nullptr) ? 0 : transform->getGlobalTransform().position.x;
	x += cameraDebug.x;
	x *= SCREEN_SIZE;
	x -= size.x / 2.0f;
	return static_cast<int>(x*speed);
}

int Camera::getY(float speed) const {
	float y = 0;// (transform == nullptr) ? 0 : transform->getGlobalTransform().position.y;
	y += cameraDebug.y;
	y *= SCREEN_SIZE;
	return static_cast<int>(y*speed);
}

SDL_Rect Camera::getWindowsSize() const {
	SDL_Rect sizeWindows;
	sizeWindows.x = 0;
	sizeWindows.y = 0;
	sizeWindows.w = size.x;
	sizeWindows.h = size.y;
	return sizeWindows;
}

SDL_Rect Camera::getViewArea(float speed) const {
	SDL_Rect area;
	area.x = getX(speed);
	area.y = getY(speed);
	area.w = size.x;
	area.h = size.y;
	return area;
}