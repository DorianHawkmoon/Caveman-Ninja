#include "OnCollisionWeaponComponent.h"
#include "CollisionComponent.h"
#include "Entity.h"
#include "ModuleParticles.h"
#include "Particle.h"
#include "DamageComponent.h"
#include "Application.h"
#include "ModuleTextures.h"

OnCollisionWeaponComponent::OnCollisionWeaponComponent(const std::string& name, const std::string& nameTexture, const Animation& normal, const Animation& strong) : DestroyOnCollisionComponent(name), strongParticle(strong), normalParticle(normal), nameTexture(nameTexture), cleaned(true) {
	normalParticle.life = 1000000;
	normalParticle.delay = 0;
	strongParticle.life = 1000000;
	strongParticle.delay = 0;
}

OnCollisionWeaponComponent::~OnCollisionWeaponComponent() {}

bool OnCollisionWeaponComponent::start() {
	//load the graphic and give it to the animations
	if (cleaned) {
		texture = App->textures->load(nameTexture.c_str());
		if (texture != nullptr) {
			cleaned = false;
			strongParticle.graphics = texture;
			normalParticle.graphics = texture;
		} else {
			cleaned = true;
		}
		return !cleaned;
	} else {
		return true;
	}
}

void OnCollisionWeaponComponent::onCollisionEnter(const Collider * self, const Collider * another) {
	//check its me
	CollisionComponent* collider = static_cast<CollisionComponent*>(parent->getComponent("collider"));
	if (self != collider->getCollider()) {
		return;
	}

	//destroy myself
	parent->destroy();

	//check my strong to set a particle
	DamageComponent* damage = static_cast<DamageComponent*>(parent->getComponent("damage"));
	fPoint position=self->getGlobalTransform().position;
	/*position.x += self->getSize().x;
	position.y += self->getSize().y;*/
	fPoint velocity = fPoint(0,0);
	if (damage != nullptr && damage->strong) {
		App->particles->addParticle(strongParticle,position,velocity, strongParticle.delay);
	} else {
		App->particles->addParticle(normalParticle, position, velocity, normalParticle.delay);
	}

}

update_status OnCollisionWeaponComponent::preUpdate() {
	if (toClean) {
		cleanUp();
		toClean = false;
	}
	return UPDATE_CONTINUE;
}

bool OnCollisionWeaponComponent::cleanUp() {
	if (!cleaned) {
		App->textures->unload(texture);
		strongParticle.graphics = nullptr;
		normalParticle.graphics = nullptr;
		cleaned = true;
	}
	return true;
}
