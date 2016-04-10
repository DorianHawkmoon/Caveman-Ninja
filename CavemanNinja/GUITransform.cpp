#include "GUITransform.h"


GUITransform::GUITransform() {
	setToZero();
}

GUITransform::~GUITransform() {}


// Utils ------------------------------------------------

bool GUITransform::IsZero() const {
	return position.isZero() && rotation == 0;
}

GUITransform & GUITransform::setToZero() {
	position.setToZero();
	rotation = 0;
	location = ABSOLUTE;
	pivot = ABSOLUTE;
	return(*this);
}
