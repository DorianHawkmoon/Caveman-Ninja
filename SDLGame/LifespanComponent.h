#pragma once
#ifndef LIFESPAN_COMPONENT_H
#define LIFESPAN_COMPONENT_H

#include "IComponent.h"
#include <string>
class Timer;

class LifespanComponent : public IComponent {
public:
	LifespanComponent(const std::string& name, unsigned int lifespan);
	virtual ~LifespanComponent();

	virtual bool start();
	virtual update_status update();
	virtual IComponent* makeClone() const;

private:
	Timer* time;
	unsigned int lifespan;
};


#endif // !LIFESPAN_COMPONENT_H
