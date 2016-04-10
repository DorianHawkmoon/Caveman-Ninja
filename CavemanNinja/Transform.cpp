#include "Transform.h"



Transform::Transform() {
	setToZero();
	node = nullptr;
}


Transform::~Transform() {}


// Utils ------------------------------------------------

bool Transform::IsZero() const {
	return position.isZero()  && flip == SDL_FLIP_NONE && rotation == 0;
}

Transform & Transform::setToZero() {
	flip = SDL_FLIP_NONE;
	position.setToZero();
	rotation = 0;
	return(*this);
}
