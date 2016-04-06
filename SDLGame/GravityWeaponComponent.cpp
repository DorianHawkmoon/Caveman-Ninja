#include "GravityWeaponComponent.h"
#include "Entity.h"
#include "MotionComponent.h"
#include "CollisionComponent.h"
#include "Application.h"
#include "ModuleTimer.h"

inline GravityWeaponComponent::GravityWeaponComponent(std::string nameComponent) : IComponent(nameComponent), gravity(0), timer() {
	timer = App->timer->getTimer();
}

GravityWeaponComponent::~GravityWeaponComponent() {
	App->timer->deleteTimer(timer);
}

IComponent * GravityWeaponComponent::makeClone() const {
	GravityWeaponComponent* result = new GravityWeaponComponent(this->getID());
	result->gravity = gravity;
	return result;
}

bool GravityWeaponComponent::start() {
	motion = static_cast<MotionComponent*>(parent->getComponent("motion"));
	collision = static_cast<CollisionComponent*>(parent->getComponent("collider"));
	timer->start();
	return true;
}

update_status GravityWeaponComponent::update() {
	if (motion != nullptr) {
		//apply force gravity
		if (timer->value() > 200) {
			motion->velocity.y += 1.0f * gravity *(App->timer->getDeltaFrame() / 1000.0f);
		}

		//control de gravedad máxima
		if (motion->velocity.y > maxVelocity) {
			motion->velocity.y = maxVelocity;
		}
	}
	return UPDATE_CONTINUE;
}
