#pragma once
#ifndef LIFE_COMPONENT_H
#define LIFE_COMPONENT_H

#include "IComponent.h"
class LifeComponent : public IComponent {
public:
	LifeComponent(const std::string& name, int maxLife) :IComponent(name), actualLife(maxLife), maxLife(maxLife) {}
	virtual ~LifeComponent() {}

	virtual IComponent* makeClone() const;

	void modifyActualLife(int modify);
	int getActualLife() const;
	void setActualLife(int life);
	int getMaxLife() const;
	bool isAlive() const;

	int previousLife; //weird hack for portrait animation

private:
	int actualLife;
	int maxLife;
};


#endif // !LIFE_COMPONENT_H
