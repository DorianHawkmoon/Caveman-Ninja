#include "Gravity.h"
#include "RectangleCollider.h"
#include "Application.h"
#include "ModuleCollision.h"
#include "CollisionComponent.h"

IComponent * Gravity::makeClone() {
	Gravity* result = new Gravity(this->getID());
	result->gravity = gravity;
	return result;
}

update_status Gravity::update() {
	MotionComponent* motion = static_cast<MotionComponent*>(parent->getComponent("motion"));
	if (motion != nullptr) {
		//apply force gravity
		motion->velocity.y += 1.0f * gravity *(App->timer->getDeltaFrame() / 1000.0f);

		//if falling, check tolerance under the entity
		if (motion->velocity.y > 0) {
			//TODO 10,10 sería la tolerancia -> la posición tendría que ser desde bottom left y no top left
			RectangleCollider rectangle = RectangleCollider(parent->transform->getGlobalTransform().position, iPoint(5,57), 0, TypeCollider::PLAYER);
			std::list<Collider*> collisions=App->collisions->checkCollisions(&rectangle);
			//si la lista está vacia, está cayendo, setear FALL
			//si no lo esta ... nada, las colisiones especificas se mirara en onCollision
			//TODO  tener en cuenta el jump down (mover la tolerancia? checkear la tolerancia en jump?) -> variable y cuando la distancia recorrida sea mayor que la tolerancia, considerar la gravedad normal
			if (collisions.size() > 0) {
				parent->controller.stateJump = JumpType::FALL;
			} else {
				parent->controller.stateJump = JumpType::NONE;
			}
		}

		//control de gravedad máxima
		if (motion->velocity.y > maxVelocity) {
			motion->velocity.y = maxVelocity;
		}
	}
	return UPDATE_CONTINUE;
}

void Gravity::onCollisionEnter(Collider * one, Collider * another) {
	CollisionComponent* collider= static_cast<CollisionComponent*>(parent->getComponent("collider"));
	//is from my entity?
	if (one != collider->getCollider()){
		return;
	}

	//wall/ground type?
	if (another->type != TypeCollider::GROUND && another->type != TypeCollider::FLOOR) {
		return;
	}

	//falling?
	MotionComponent* motion = static_cast<MotionComponent*>(parent->getComponent("motion"));
	if (motion->velocity.y < 0) {
		return;
	} else {
		//check if the other collider is under him
		//if yes, STOP!
		motion->velocity.y = 0;
		parent->controller.stateJump = JumpType::NONE;
	}

	//put the entity over the collision again
	int count = 0;
	do {
		parent->transform->position.y--;
	} while (one->checkCollision(another) && ++count < 100);

}

bool Gravity::isFalling() {
	//TODO 10,10 sería la tolerancia -> la posición tendría que ser desde bottom left y no top left
	RectangleCollider rectangle = RectangleCollider(parent->transform->getGlobalTransform().position, iPoint(5, 57), 0, TypeCollider::PLAYER);
	std::list<Collider*> collisions = App->collisions->checkCollisions(&rectangle);
	//TODO  tener en cuenta el jump down (mover la tolerancia? checkear la tolerancia en jump?) -> variable y cuando la distancia recorrida sea mayor que la tolerancia, considerar la gravedad normal
	return collisions.size() == 0;
}
