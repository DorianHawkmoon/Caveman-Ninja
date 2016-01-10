#pragma once
#ifndef DISAPPEAR_OUT_CAMERA_H
#define DISAPPEAR_OUT_CAMERA_H

#include "IComponent.h"
#include <list>
#include <string>
class Condition;
class CollisionComponent;

class DisappearOutCamera : public IComponent {
public:
	DisappearOutCamera(const std::string& name);
	virtual ~DisappearOutCamera();

	bool start();
	update_status preUpdate();
	virtual update_status update();
	void addCondition(const Condition * condition);

	bool cleanUp();
	IComponent* makeClone();

private:
	/**
	* Condiciones
	*/
	std::list<Condition*> conditions; //conditions owned

	bool checkCondition();
	bool outsideCamera();

	CollisionComponent* collider;
	bool cleaned;
};

#endif // !DISAPPEAR_OUT_CAMERA_H

