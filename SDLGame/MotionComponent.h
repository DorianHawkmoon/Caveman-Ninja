#pragma once
#ifndef MOTION_COMPONENT_H
#define MOTION_COMPONENT_H

#include "IComponent.h"
#include "Point.h"
#include "Entity.h"

class MotionComponent :
	public IComponent {
public:
	MotionComponent(const std::string& nameComponent);
	virtual ~MotionComponent();

	virtual update_status update();

	virtual IComponent* makeClone() const;

public:
	fPoint velocity;
	float speed;
	float doubleSpeed;
};


#endif // !MOTION_COMPONENT_H
