#include "Camera.h"

SDL_Rect Camera::getWindowsSize() const {
	SDL_Rect sizeWindows;
	sizeWindows.x = 0;
	sizeWindows.y = 0;
	sizeWindows.w = camera.w;
	sizeWindows.h = camera.h;
	return sizeWindows;
}

SDL_Rect Camera::getViewArea(float speed) const {
	SDL_Rect area;
	area.x = static_cast<int>(camera.x*speed);
	area.y = static_cast<int>(camera.y*speed);
	area.w = camera.w;
	area.h = camera.h;
	return area;
}
