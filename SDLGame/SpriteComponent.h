#pragma once
#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include "IComponent.h"
#include <string>
struct SDL_Texture;

class SpriteComponent : public IComponent {
public:
	SpriteComponent(std::string nameComponent, std::string nameTexture) : IComponent(nameComponent), name(nameTexture) {}
	~SpriteComponent();



	bool start() {};
	update_status preUpdate() {};
	update_status update() {};
	update_status postUpdate() {};
	bool cleanUp() {};

private:
	std::string name;
	SDL_Texture* texture;
};


#endif // !SPRITE_COMPONENT_H
