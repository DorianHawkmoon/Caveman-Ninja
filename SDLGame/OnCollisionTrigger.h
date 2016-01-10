#pragma once
#ifndef ON_COLLISION_TRIGGER_H
#define ON_COLLISION_TRIGGER_H

#include "IComponent.h"
#include <functional>

class OnCollisionTrigger : public IComponent {
public:
	OnCollisionTrigger(const std::string& name, const std::function<void()>& functionCheck);
	virtual ~OnCollisionTrigger() {}
	
	virtual void onCollisionEnter(const Collider* self, const Collider* another);

	IComponent* makeClone() const;

private:
	/**
	* variable donde guardar la función a ejecutar
	*/
	std::function<void()> functionCheck;
};


#endif // !ON_COLLISION_TRIGGER_H
