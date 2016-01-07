#pragma once
#ifndef DISAPPEAR_OUT_CAMERA_H
#define DISAPPEAR_OUT_CAMERA_H

#include "IComponent.h"
#include <list>
#include <string>
class Condition;

class DisappearOutCamera : public IComponent {
public:
	DisappearOutCamera(const std::string& name);
	virtual ~DisappearOutCamera();

	virtual update_status update();

	void addCondition(const Condition * condition);

	IComponent* makeClone();

private:
	/**
	* Condiciones
	*/
	std::list<Condition*> conditions; //conditions owned

	bool checkCondition();
	bool outsideCamera();
};

#endif // !DISAPPEAR_OUT_CAMERA_H

