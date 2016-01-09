#include "GravityComponent.h"
#include "RectangleCollider.h"
#include "Application.h"
#include "ModuleCollision.h"
#include "CollisionComponent.h"
#include "MotionComponent.h"
#include "ColliderInteraction.h"

IComponent * GravityComponent::makeClone() {
	GravityComponent* result = new GravityComponent(this->getID());
	result->gravity = gravity;
	return result;
}

bool GravityComponent::start() {
	motion = static_cast<MotionComponent*>(parent->getComponent("motion"));
	collision = static_cast<CollisionComponent*>(parent->getComponent("collider"));

	const Collider* collider = (collision)->getCollider();
	iPoint size = collider->getSize();
	fPoint offset;
	offset.y += size.y;
	offset.x += size.x / 2;

	 gravityCollider = new RectangleCollider(offset, iPoint(5, 10), 0, TypeCollider::GRAVITY);
	 gravityCollider->addListener(parent);
	 gravityCollider->parentTransform = parent->transform;
	 App->collisions->addCollider(gravityCollider);

	return true;
}

update_status GravityComponent::update() {
	if (motion != nullptr) {
		//apply force gravity
		motion->velocity.y += 1.0f * gravity *(App->timer->getDeltaFrame() / 1000.0f);

		//if falling, check tolerance under the entity
		if (motion->velocity.y > 0) {
			//TODO 10,10 sería la tolerancia -> la posición tendría que ser desde bottom center y no top left

			std::list<Collider*> collisions=App->collisions->checkCollisions(gravityCollider);
			//si la lista está vacia, está cayendo, setear FALL
			//si no lo esta ... nada, las colisiones especificas se mirara en onCollision
			//TODO  tener en cuenta el jump down (mover la tolerancia? checkear la tolerancia en jump?) -> variable y cuando la distancia recorrida sea mayor que la tolerancia, considerar la gravedad normal
			if (collisions.size() > 0) {
				//si está en salto, no tenerlo en cuenta!!
				JumpType jumping = parent->controller.stateJump;
				if (jumping!=JumpType::JUMP && jumping!=JumpType::DOUBLE_JUMP && jumping!=JumpType::JUMP_DOWN) {
					parent->controller.stateJump = JumpType::NONE;
				}
			} else {
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
	gravityCollider->paintCollider();
	return UPDATE_CONTINUE;
}


void GravityComponent::onCollisionEnter(const Collider * self, const Collider * another) {
	//is from my entity?
	if (self != collision->getCollider()){
		return;
	}

	//wall/ground type?
	if (!App->collisions->interaction.permission(another->type,TypeCollider::GRAVITY)) {
		return;
	}

	//if is falling (and not jumping or being throwed)
	if (motion->velocity.y > 0) {
		//put the entity over the collision again, get the center point of the collider
		const Collider* coll = collision->getCollider();
		
		int count = 0;

		Transform trans = parent->transform->getGlobalTransform();
		iPoint size = coll->getSize();
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

bool GravityComponent::isFalling() {
	//TODO 10,10 sería la tolerancia -> la posición tendría que ser desde bottom left y no top left
	std::list<Collider*> collisions = App->collisions->checkCollisions(gravityCollider);
	//TODO  tener en cuenta el jump down (mover la tolerancia? checkear la tolerancia en jump?) -> variable y cuando la distancia recorrida sea mayor que la tolerancia, considerar la gravedad normal
	return collisions.size() ==0; //me cuento a mi mismo
}
