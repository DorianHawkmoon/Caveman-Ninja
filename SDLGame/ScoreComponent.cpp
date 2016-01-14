#include "ScoreComponent.h"

ScoreComponent::ScoreComponent(const std::string & name) :IComponent(name) {}

ScoreComponent::~ScoreComponent() {}

IComponent * ScoreComponent::makeClone() const {
	return new ScoreComponent(getID());
}
