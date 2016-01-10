#include "OnCollisionTrigger.h"

OnCollisionTrigger::OnCollisionTrigger(const std::string & name, const std::function<void()>& functionCheck) : IComponent(name) {
	this->functionCheck = functionCheck;
}

void OnCollisionTrigger::onCollisionEnter(const Collider * self, const Collider * another) {
	functionCheck();
}

IComponent * OnCollisionTrigger::makeClone() const {
	return new OnCollisionTrigger(getID(), functionCheck);
}
