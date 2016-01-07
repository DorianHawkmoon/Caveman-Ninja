#pragma once
#ifndef LIFE_COMPONENT_H
#define LIFE_COMPONENT_H

#include "IComponent.h"
class LifeComponent : public IComponent {
public:
	LifeComponent(const std::string& name, int maxLife) :IComponent(name), actualLife(maxLife), maxLife(maxLife) {}
	virtual ~LifeComponent() {}

	IComponent* makeClone();

	void modifyActualLife(int modify);

	inline int getActualLife() const {
		return actualLife;
	}

	void setActualLife(int life);

	int getMaxLife() const;

	bool isDead() const;

private:
	int actualLife;
	int maxLife;
};


#endif // !LIFE_COMPONENT_H