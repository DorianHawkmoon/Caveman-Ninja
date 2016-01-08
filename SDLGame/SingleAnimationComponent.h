#pragma once
#ifndef SINGLE_ANIMATION_COMPONENT_H
#define SINGLE_ANIMATION_COMPONENT_H

#include "IComponent.h"
#include <string>
#include "Animation.h"
struct SDL_Texture;

class SingleAnimationComponent : public IComponent {
public:
	SingleAnimationComponent(const std::string& nameComponent, const std::string& texture, Animation& animation);
	virtual ~SingleAnimationComponent();

	bool start();

	update_status update();
	update_status postUpdate();

	bool cleanUp();

	IComponent* makeClone();

private:
	std::string nameTexture;
	SDL_Texture* texture;
	Animation anim;
};

#endif // !SINGLE_ANIMATION_COMPONENT_H

