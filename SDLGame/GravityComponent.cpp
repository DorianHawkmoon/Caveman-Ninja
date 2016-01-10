#include "GravityComponent.h"
#include "RectangleCollider.h"
#include "Application.h"
#include "ModuleCollision.h"
#include "MotionComponent.h"
#include "ColliderInteraction.h"

GravityComponent::GravityComponent(std::string nameComponent, Collider *collider) : IComponent(nameComponent), gravity(0), collision(collider), cleaned(true) {}

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

IComponent * GravityComponent::makeClone() {
	GravityComponent* result = new GravityComponent(this->getID(), collision->clone());
	result->gravity = gravity;
	return result;
}

bool GravityComponent::start() {
	if (cleaned) {
		motion = static_cast<MotionComponent*>(parent->getComponent("motion"));
	
		collision->addListener(parent);
		collision->parentTransform = parent->transform;
		collision->parent = parent;
		App->collisions->addCollider(collision);


		iPoint size = collision->getSize();
		fPoint offset(0, 0);
		offset.y += size.y;
		offset.x += size.x / 2;

		

		gravityCollider = new RectangleCollider(offset, iPoint(5, 10), 0, TypeCollider::GRAVITY);
		gravityCollider->addListener(parent);
		gravityCollider->parentTransform = parent->transform;
		gravityCollider->parent = parent;
		App->collisions->addCollider(gravityCollider);

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
			//TODO 10,10 ser�a la tolerancia -> la posici�n tendr�a que ser desde bottom center y no top left

			std::list<Collider*> collisions=App->collisions->checkCollisions(gravityCollider);
			//si la lista est� vacia, est� cayendo, setear FALL
			//si no lo esta ... nada, las colisiones especificas se mirara en onCollision
			//TODO  tener en cuenta el jump down (mover la tolerancia? checkear la tolerancia en jump?) -> variable y cuando la distancia recorrida sea mayor que la tolerancia, considerar la gravedad normal
			if (collisions.size() > 0) {
				//si est� en salto, no tenerlo en cuenta!!
				JumpType jumping = parent->controller.stateJump;
				if (jumping!=JumpType::JUMP && jumping!=JumpType::DOUBLE_JUMP && jumping!=JumpType::JUMP_DOWN) {
					parent->controller.stateJump = JumpType::NONE;
				}
			} else {
				parent->controller.stateJump = JumpType::FALL;
			}
		}

		//control de gravedad m�xima
		if (motion->velocity.y > maxVelocity) {
			motion->velocity.y = maxVelocity;
		}
	}
	return UPDATE_CONTINUE;
}

update_status GravityComponent::postUpdate() {
	#ifdef DEBUG
		gravityCollider->paintCollider();
	#endif
	
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
	if (motion->velocity.y > 0) {
		//put the entity over the collision again, get the center point of the collider
		int count = 0;

		Transform trans = parent->transform->getGlobalTransform();
		iPoint size = collision->getSize();
		fPoint position=trans.position;
		position.y += size.y-2;
		position.x += size.x / 2;
		RectangleCollider point = RectangleCollider(position, iPoint(2, 2), 0, TypeCollider::NONE_COLLIDER);

		while (point.checkCollision(another) && ++count < 500) {
			motion->velocity.y = 0;
			parent->transform->position.y--;
			point.position.y--;
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
	//TODO 10,10 ser�a la tolerancia -> la posici�n tendr�a que ser desde bottom left y no top left
	std::list<Collider*> collisions = App->collisions->checkCollisions(gravityCollider);
	//TODO  tener en cuenta el jump down (mover la tolerancia? checkear la tolerancia en jump?) -> variable y cuando la distancia recorrida sea mayor que la tolerancia, considerar la gravedad normal
	return collisions.size() == 0; //me cuento a mi mismo
}

bool GravityComponent::cleanUp() {
	if (!cleaned) {
		motion = nullptr;
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
