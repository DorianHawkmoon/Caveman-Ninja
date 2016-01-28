#pragma once
#ifndef JUMP_COMPONENT_H
#define JUMP_COMPONENT_H

#include "IComponent.h"
#include <string>
#include "Entity.h"

class JumpComponent : public IComponent {
public:
	JumpComponent(const std::string& nameComponent);
	virtual ~JumpComponent() {};

	virtual bool start();
	virtual update_status preUpdate();
	virtual update_status update();

	virtual bool cleanUp();

	virtual IComponent* makeClone() const;

public:
	float speed;
	float doubleSpeed;

private:
	TypeJump* jump;
	bool jumpAccelerated;
	bool cleaned;
};

#endif // !JUMP_COMPONENT_H

