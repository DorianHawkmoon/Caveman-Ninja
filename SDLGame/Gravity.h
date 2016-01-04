#pragma once
#ifndef GRAVITY_H
#define GRAVITY_H

#include "IComponent.h"
#include "MotionComponent.h"
#include <string>
#include "Entity.h"
#include "Application.h"
#include "ModuleTimer.h"

class Gravity :	public IComponent {
public:
	Gravity(std::string nameComponent) : IComponent(nameComponent), gravity(0) {}
	virtual ~Gravity() {}

	IComponent* makeClone();

	update_status update();
	
	bool isFalling() const;

public:
	float gravity;
};

#endif // !GRAVITY_H

