#pragma once
#ifndef IA_COMPONENT_H
#define IA_COMPONENT_H

#include <functional>
#include "IComponent.h"
#include "Application.h"
#include "ModuleTimer.h"

struct doNothing{
	void operator()(Entity* entity, bool ticked) {}
};

class IAComponent : public IComponent {
public:
	IAComponent(const std::string& name);
	virtual ~IAComponent() {}

	virtual bool start();

	virtual update_status preUpdate();

	virtual update_status update();

	virtual update_status postUpdate();

	IComponent* makeClone();

	void clearFunction(std::function<void(Entity*, bool)>& function);
	
public:
	unsigned int ticks;
	std::function<void(Entity*, bool)> functionUpdate;
	std::function<void(Entity*, bool)> functionPreUpdate;
	std::function<void(Entity*, bool)> functionPostUpdate;

private:
	unsigned int ticksPassed;
	bool ticked;
};

#endif // !IA_COMPONENT_H

