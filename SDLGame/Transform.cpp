#include "Transform.h"



Transform::Transform() {
	setToZero();
	node = nullptr;
}


Transform::~Transform() {}


// Utils ------------------------------------------------

bool Transform::IsZero() const {
	return position.isZero() && speed == 0 && flip == SDL_FLIP_NONE; // && scale.isZero() && rotation == 0 && ;
}

Transform & Transform::setToZero() {
	speed = 0;
	flip = SDL_FLIP_NONE;
	position.setToZero();
	//scale.setToZero();
	//rotation = 0;
	return(*this);
}
