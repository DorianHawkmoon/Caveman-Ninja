#include "GravityComponent.h"
#include "RectangleCollider.h"
#include "LifeComponent.h"
#include "Application.h"
#include "ModuleCollision.h"
#include "MotionComponent.h"
#include "ColliderInteraction.h"
#include "ModuleAudio.h"
#include "CircleCollider.h"

GravityComponent::GravityComponent(const std::string& nameComponent, Collider *collider) : IComponent(nameComponent), gravity(0), collision(collider), cleaned(true) {}

GravityComponent::~GravityComponent() {
	//just in case
	cleanUp();
	if (gravityCollider != nullptr) {
		delete gravityCollider;
	}
	if (collision != nullptr) {
		delete collision;
	}
}

IComponent * GravityComponent::makeClone() const {
	GravityComponent* result = new GravityComponent(this->getID(), collision->clone());
	result->gravity = gravity;
	return result;
}

bool GravityComponent::start() {
	if (cleaned) {
		loadSound();
		motion = static_cast<MotionComponent*>(parent->getComponent("motion"));
	
		collision->addListener(parent);
		collision->parentTransform = parent->transform;
		collision->parent = parent;
		App->collisions->addCollider(collision);		

		fPoint positionGravity = collision->position;
		//positionGravity.y += 5;
		gravityCollider = new CircleCollider(positionGravity, 5, TypeCollider::GRAVITY);
		gravityCollider->addListener(parent);
		gravityCollider->parentTransform = parent->transform;
		gravityCollider->parent = parent;
		//App->collisions->addCollider(gravityCollider);

		life = static_cast<LifeComponent*>(parent->getComponent("life"));

		cleaned = false;
	}
	return true;
}

update_status GravityComponent::preUpdate() {
	if (toClean) {
		cleanUp();
		toClean = false;
	}
	return UPDATE_CONTINUE;
}

update_status GravityComponent::update() {
	if (motion != nullptr) {
		//apply force gravity
		motion->velocity.y += 1.0f * gravity *(App->timer->getDeltaFrame() / 1000.0f);

		//if falling, check tolerance under the entity
		if (motion->velocity.y > 0) {
			std::list<Collider*> collisions=App->collisions->checkCollisions(gravityCollider);
			//si la lista está vacia, está cayendo, setear FALL
			//si no lo esta ... nada, las colisiones especificas se mirara en onCollision
			//TODO  tener en cuenta el jump down (mover la tolerancia? checkear la tolerancia en jump?) -> variable y cuando la distancia recorrida sea mayor que la tolerancia, considerar la gravedad normal
				
			if (collisions.size() > 0) {
				//si está en salto, no tenerlo en cuenta!!
				JumpType jumping = parent->controller.stateJump;
				if (jumping == JumpType::FALL) {
					//not if is dead
					if (life!=nullptr && life->isAlive()) {
						playSound();
					}
					parent->controller.stateJump = JumpType::NONE;
				}
				//for (auto it = collisions.begin(); it != collisions.end(); ++it) {
					this->onCollisionEnter(collision, collisions.front());
				//}
			} else {
				//check with another circle for tolerance of making fall, same to the other method
				parent->controller.stateJump = JumpType::FALL;
			}
		}

		//control de gravedad máxima
		if (motion->velocity.y > maxVelocity) {
			motion->velocity.y = maxVelocity;
		}
	}
	return UPDATE_CONTINUE;
}

update_status GravityComponent::postUpdate() {
	if (DEBUG_COLLISIONS) {
		gravityCollider->paintCollider();
	}
	
	return UPDATE_CONTINUE;
}


void GravityComponent::onCollisionEnter(const Collider * self, const Collider * another) {
	//is from my entity?
	if (self != collision){
		return;
	}

	//wall/ground type?
	if (!App->collisions->interaction.permission(another->type,TypeCollider::GRAVITY)) {
		return;
	}

	//if is falling (and not jumping or being throwed)
	if (motion->velocity.y > 0) { //TODO comprobar que no sea también saltando hacia abajo
		//put the entity over the collision again
		int count = 0;
		motion->velocity.y = 0;
		while (collision->checkCollision(another) && ++count < 500) {
			parent->transform->position.y--;
		}
	}
}

void GravityComponent::onCollisionStay(const Collider * self, const Collider * another) {
	onCollisionEnter(self, another);
}

bool GravityComponent::isFalling() {
	if (cleaned) {
		return false;
	}
	//TODO 10,10 sería la tolerancia -> la posición tendría que ser desde bottom left y no top left
	std::list<Collider*> collisions = App->collisions->checkCollisions(gravityCollider);
	//TODO  tener en cuenta el jump down (mover la tolerancia? checkear la tolerancia en jump?) -> variable y cuando la distancia recorrida sea mayor que la tolerancia, considerar la gravedad normal
	return collisions.size() == 0; //me cuento a mi mismo
}

bool GravityComponent::cleanUp() {
	if (!cleaned) {
		motion = nullptr;
		life = nullptr;
		if (gravityCollider != nullptr) {
			App->collisions->removeCollider(gravityCollider);
			delete gravityCollider;
			gravityCollider = nullptr;
		}
		App->collisions->removeCollider(collision);
		cleaned = true;
	}
	return false;
}
