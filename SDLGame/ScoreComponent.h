#pragma once
#ifndef SCORE_COMPONENT_H
#define SCORE_COMPONENT_H

#include "IComponent.h"
#include "Score.h"

class ScoreComponent : public IComponent {
public:
	ScoreComponent(const std::string& name);
	~ScoreComponent();

	IComponent* makeClone() const;

	Score score;
};


#endif // !SCORE_COMPONENT_H
