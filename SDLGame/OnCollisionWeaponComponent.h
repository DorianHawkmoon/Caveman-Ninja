#pragma once
#ifndef ON_COLLISION_WEAPON_COMPONENT_H
#define ON_COLLISION_WEAPON_COMPONENT_H

#include "DestroyOnCollisionComponent.h"
#include "Particle.h"
class OnCollisionWeaponComponent : public DestroyOnCollisionComponent {
public:
	OnCollisionWeaponComponent(const std::string& name, const std::string& nameTexture, const Animation& normal, const Animation& strong);
	virtual ~OnCollisionWeaponComponent();

	bool start();

	void onCollisionEnter(const Collider* self, const Collider* another); //destroy and add the animation (a copy)

	update_status preUpdate();

	bool cleanUp();

private:
	const std::string nameTexture;
	SDL_Texture* texture;

	Particle normalParticle;
	Particle strongParticle;

	bool cleaned;
};


#endif // !ON_COLLISION_WEAPON_COMPONENT_H
