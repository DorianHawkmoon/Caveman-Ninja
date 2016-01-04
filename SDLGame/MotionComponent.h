#pragma once
#ifndef MOTION_COMPONENT_H
#define MOTION_COMPONENT_H

#include "IComponent.h"
#include "Point.h"
#include "Entity.h"

class MotionComponent :
	public IComponent {
public:
	MotionComponent(std::string nameComponent);
	virtual ~MotionComponent();

	update_status update();

	IComponent* makeClone();

public:
	fPoint velocity;
};


#endif // !MOTION_COMPONENT_H
