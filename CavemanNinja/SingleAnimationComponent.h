#pragma once
#ifndef SINGLE_ANIMATION_COMPONENT_H
#define SINGLE_ANIMATION_COMPONENT_H

#include "IComponent.h"
#include <string>
#include "Animation.h"
struct SDL_Texture;

class SingleAnimationComponent : public IComponent {
public:
	SingleAnimationComponent(const std::string& nameComponent, const std::string& texture, const Animation& animation);
	virtual ~SingleAnimationComponent();

	virtual bool start();
	virtual update_status preUpdate();
	virtual update_status update();
	virtual update_status postUpdate();

	virtual bool cleanUp();

	virtual IComponent* makeClone() const;

private:
	std::string nameTexture;
	SDL_Texture* texture;
	Animation anim;

	bool cleaned;
};

#endif // !SINGLE_ANIMATION_COMPONENT_H

