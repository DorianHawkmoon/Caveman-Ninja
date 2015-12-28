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
	~MotionComponent();

	update_status preUpdate();
	//TODO: en update pilla el transform del entity (es miembro) y le actualiza la velocidad
	update_status update();
	update_status postUpdate();

	//TODO make clone
	IComponent* makeClone() {
		return nullptr;
	}

public:
	fPoint velocity;
};


#endif // !MOTION_COMPONENT_H
