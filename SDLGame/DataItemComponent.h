#pragma once
#ifndef DATA_ITEM_COMPONENT_H
#define DATA_ITEM_COMPONENT_H

#include "IComponent.h"
#include "Items.h"

class DataItemComponent : public IComponent {
public:
	DataItemComponent(const std::string& name);
	virtual ~DataItemComponent();

	virtual IComponent* makeClone() const;

	TypeItem type;
	int points;
	int life;
};

#endif // !DATA_ITEM_COMPONENT_H

