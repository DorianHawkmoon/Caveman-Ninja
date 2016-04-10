#pragma once
#ifndef DROP_ITEM_COMPONENT_H
#define DROP_ITEM_COMPONENT_H

#include "IComponent.h"
class Entity;

class DropItemComponent : public IComponent {
public:
	DropItemComponent(const std::string& name, const Entity* item);
	virtual ~DropItemComponent();

	IComponent* makeClone() const;

	const Entity* item; //owned and cannot be modified
};

#endif // !DROP_ITEM_COMPONENT_H

