#pragma once
#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include "IComponent.h"
#include "Animation.h"
#include "StateMachine.h"
#include <string.h>

struct SDL_Texture;

class AnimationComponent : public IComponent {
public:
	AnimationComponent(const std::string& nameComponent, const std::string& texture, StateMachine<Animation>* stateMachine);
	virtual ~AnimationComponent();

	bool start();
	update_status preUpdate();
	update_status update();
	update_status postUpdate();

	bool cleanUp();

	IComponent* makeClone();

	const Animation* getActualAnimation() const;

private:
	std::string nameTexture;
	StateMachine<Animation>* state;
	SDL_Texture* texture;

	bool cleaned;
};

#endif // ANIMATION_COMPONENT_H