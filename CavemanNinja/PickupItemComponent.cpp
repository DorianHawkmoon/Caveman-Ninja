#include "PickupItemComponent.h"
#include "Entity.h"
#include "Collider.h"
#include "Items.h"
#include "DataItemComponent.h"
#include "LifeComponent.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleParticles.h"
#include "ScoreComponent.h"
#include "Particle.h"
#include "Transform.h"

PickupItemComponent::PickupItemComponent(const std::string & name) :IComponent(name) {}

PickupItemComponent::~PickupItemComponent() {}

bool PickupItemComponent::start() {
	bool result=true;
	life = static_cast<LifeComponent*>(parent->getComponent("life"));
	score = static_cast<ScoreComponent*>(parent->getComponent("score"));
	eatSound = App->audio->loadEffect("eatFood.wav");
	return result;
}

void PickupItemComponent::onCollisionEnter(const Collider * self, const Collider * another) {
	//check if is the item
	if (another->type != TypeCollider::ITEM) {
		return;
	}

	//get the data of the entity
	DataItemComponent* data = static_cast<DataItemComponent*>(another->parent->getComponent("data"));
	if (data == nullptr) {
		return;
	}

	switch (data->type) {
		case TypeItem::SMALL_FOOD:
		case TypeItem::MEDIUM_FOOD:
		case TypeItem::BIG_FOOD:
			if (life != nullptr) {
				life->modifyActualLife(data->life);
			}
			if (score != nullptr) {
				score->score.addScore(data->points);
				//add particle score
				Particle p = Particle("pickups_score.png",data->effectScore);
				p.delay = 0;
				p.life = 1000;
				
				fPoint position = parent->transform->getGlobalTransform().position;
				fPoint velocity = {0,-10};
				App->particles->addParticle(p, position, velocity);
			}

			//play sound
			App->audio->playEffect(eatSound);
			break;

		case TypeItem::WEAPON:
			
			break;
	}
}

bool PickupItemComponent::cleanUp() {
	life = nullptr;
	score = nullptr;
	return true;
}

IComponent * PickupItemComponent::makeClone() const {
	return new PickupItemComponent(getID());
}
