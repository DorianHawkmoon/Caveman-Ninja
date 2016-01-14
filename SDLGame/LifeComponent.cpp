#include "LifeComponent.h"

IComponent * LifeComponent::makeClone() const {
	LifeComponent* lifeC = new LifeComponent(getID(),maxLife);
	lifeC->actualLife = actualLife;
	return lifeC;
}

void LifeComponent::modifyActualLife(int modify) {
	previousLife = actualLife;
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

bool LifeComponent::isAlive() const {
	return actualLife > 0;
}
