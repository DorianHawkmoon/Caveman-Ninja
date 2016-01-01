#include "Gravity.h"

IComponent * Gravity::makeClone() {
	Gravity* result = new Gravity(this->getID());
	result->gravity = gravity;
	return result;
}
