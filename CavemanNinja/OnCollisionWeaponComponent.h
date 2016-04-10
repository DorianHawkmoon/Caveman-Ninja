#pragma once
#ifndef ON_COLLISION_WEAPON_COMPONENT_H
#define ON_COLLISION_WEAPON_COMPONENT_H

#include "DestroyOnCollisionComponent.h"
#include "Particle.h"
class Collider;

class OnCollisionWeaponComponent : public DestroyOnCollisionComponent {
public:
	OnCollisionWeaponComponent(const std::string& name, const std::string& nameTexture, const Animation& normal, const Animation& strong);
	virtual ~OnCollisionWeaponComponent();

	virtual bool start();
	virtual void onCollisionEnter(const Collider* self, const Collider* another);
	virtual update_status preUpdate();
	virtual bool cleanUp();

private:
	const std::string nameTexture;
	SDL_Texture* texture;

	Particle normalParticle;
	Particle strongParticle;

	bool cleaned;
};


#endif // !ON_COLLISION_WEAPON_COMPONENT_H
