#pragma once
#ifndef IA_COMPONENT_H
#define IA_COMPONENT_H

#include "IComponent.h"
class IBehaviour;


class IAComponent : public IComponent {
public:
	IAComponent(const std::string& name);
	virtual ~IAComponent();

	virtual bool start();

	update_status preUpdate();
	virtual update_status delegatedPreUpdate() { 
		return UPDATE_CONTINUE;
	}
	virtual bool cleanUp();

	virtual IComponent* makeClone() const;
	
public:
	unsigned int ticks;

private:
	unsigned int ticksPassed;
protected:
	bool ticked;
};

#endif // !IA_COMPONENT_H

