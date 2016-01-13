#include "DataItemComponent.h"


DataItemComponent::DataItemComponent(const std::string & name) :IComponent(name), points(0), life(0) {}

DataItemComponent::~DataItemComponent() {}

IComponent * DataItemComponent::makeClone() const {
	DataItemComponent* result=new DataItemComponent(getID());
	result->type = type;
	result->life = life;
	result->points = points;
	return result;
}
