#include "LifeComponent.h"

IComponent * LifeComponent::makeClone() {
	LifeComponent* lifeC = new LifeComponent(getID(),maxLife);
	lifeC->actualLife = actualLife;
	return lifeC;
}

void LifeComponent::modifyActualLife(int modify) {
	actualLife += modify;
	if (actualLife > maxLife) {
		actualLife = maxLife;
	}
}

void LifeComponent::setActualLife(int life) {
	actualLife = life;
	if (actualLife > maxLife) {
		actualLife = maxLife;
	}
}

int LifeComponent::getMaxLife() const {
	return maxLife;
}

inline bool LifeComponent::isDead() const {
	return actualLife <= 0;
}
