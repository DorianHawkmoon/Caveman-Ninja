#include "RectangleCollider.h"
#include "Application.h"
#include "ModuleRender.h"
#include "CircleCollider.h"
#include "SDL\SDL_rect.h"
#include "Transform.h"
#include "LineCollider.h"

RectangleCollider::RectangleCollider(const fPoint& position, const iPoint& rectangle, float rotation, TypeCollider type) : Collider(position, type), rect(rectangle), rotation(rotation) {}

void RectangleCollider::paintCollider() const {
	//get global
	Transform global = getGlobalTransform();
	//global.position += position;
	global.rotation += rotation;
	SDL_Color color;
	color.b = 255;
	color.r = 255;
	color.g = 200;
	color.a = 100;

	if (global.rotation == 0) {
		iPoint pos(static_cast<int>(global.position.x), static_cast<int>(global.position.y));

		App->renderer->paintRectangle(color, pos, rect);
	} else {
		App->renderer->paintRectangle(color, global, rect);
	}
}

void RectangleCollider::paintCollider(const iPoint & pivot) const {
	//get global
	Transform global = getGlobalTransform();
	global.rotation += rotation;
	SDL_Color color;
	color.b = 255;
	color.r = 255;
	color.g = 200;
	color.a = 100;

	if (global.rotation == 0) {
		iPoint pos(static_cast<int>(global.position.x), static_cast<int>(global.position.y));

		App->renderer->paintRectangle(color, pos, rect);
	} else {
		App->renderer->paintRectangle(color, global, rect, pivot);
	}
}


Collider * RectangleCollider::clone() {
	RectangleCollider* result = new RectangleCollider(position, rect, rotation, type);
	return Collider::clone(result);
}


bool RectangleCollider::checkCollisionRotated(const RectangleCollider * other, const Transform & otherTrans, const Transform & self) const {
	//Code from Ignacio Astorga
	// Obtiene los puntos de los rectángulos
	std::vector<fPoint> thisPoints = getPoints(self.rotation);
	std::vector<fPoint> otherPoints = other->getPoints(otherTrans.rotation);

	// Calcula los ejes de proyección
	std::vector<fPoint> axis = std::vector<fPoint>(4);
	axis[0] = thisPoints[1] - thisPoints[0];
	axis[1] = thisPoints[3] - thisPoints[0];
	axis[2] = otherPoints[1] - otherPoints[0];
	axis[3] = otherPoints[3] - otherPoints[0];

	// Comprueba los casos especiales, utilizando perpendiculares
	if (rect.x == 0.0f) {
		axis[0] = fPoint(-axis[1].y, axis[1].x);
	}
	if (rect.y == 0.0f) {
		axis[1] = fPoint(axis[0].y, -axis[0].x);
	}
	if (other->rect.x == 0.0f) {
		axis[2] = fPoint(-axis[3].y, axis[3].x);
	}
	if (other->rect.y == 0.0f) {
		axis[3] = fPoint(axis[2].y, -axis[2].x);
	}

	// Recorre los ejes
	bool collides = false;
	for (unsigned int i = 0; i < 4; ++i) {
		// Calcula la proyección de todos los puntos de cada rectángulo sobre el eje
		// Hace el producto escalar para obtener un valor normalizado con el que comparar
		std::vector<float> thisProyections = std::vector<float>(4);
		std::vector<float> otherProyections = std::vector<float>(4);
		for (unsigned int j = 0; j < 4; ++j) {
			float factor;
			fPoint proyection;

			factor = thisPoints[j].x * axis[i].x + thisPoints[j].y * axis[i].y;
			factor /= pow(axis[i].x, 2) + pow(axis[i].y, 2);
			proyection = axis[i] * factor;
			thisProyections[j] = proyection.x * axis[i].x + proyection.y * axis[i].y;

			factor = otherPoints[j].x * axis[i].x + otherPoints[j].y * axis[i].y;
			factor /= pow(axis[i].x, 2) + pow(axis[i].y, 2);
			proyection = axis[i] * factor;
			otherProyections[j] = proyection.x * axis[i].x + proyection.y * axis[i].y;
		}

		// Calcula la menor y mayor proyección de cada rectángulo
		float thisMin, thisMax;
		thisMin = thisMax = thisProyections[0];
		float otherMin, otherMax;
		otherMin = otherMax = otherProyections[0];
		for (unsigned int j = 1; j < 4; ++j) {
			if (thisProyections[j] < thisMin)
				thisMin = thisProyections[j];
			else if (thisProyections[j] > thisMax)
				thisMax = thisProyections[j];

			if (otherProyections[j] < otherMin)
				otherMin = otherProyections[j];
			else if (otherProyections[j] > otherMax)
				otherMax = otherProyections[j];
		}

		// Comprueba si hay solapamiento en el eje
		if (thisMin < otherMax && otherMax < thisMax)
			collides = true;
		else if (otherMin < thisMax && thisMax < otherMax)
			collides = true;
		else {
			collides = false;
			break;
		}
	}

	return collides;
}

bool RectangleCollider::checkSpecificCollision(const Collider * self) const {
	return  self->checkCollision(this);
}

bool RectangleCollider::checkCollision(const RectangleCollider * other) const {
	Transform transOther = other->getGlobalTransform();
	transOther.rotation += other->rotation;
	Transform transThis = this->getGlobalTransform();
	transThis.rotation += rotation;

	if (transThis.rotation == 0 && transOther.rotation == 0) {
		fPoint global = transOther.position;
		SDL_Rect otherRect = {static_cast<int>(global.x),
			static_cast<int>(global.y),
			static_cast<int>(other->rect.x),
			static_cast<int>(other->rect.y)};

		global = transThis.position;
		SDL_Rect thisRect = {static_cast<int>(global.x),
			static_cast<int>(global.y),
			static_cast<int>(rect.x),
			static_cast<int>(rect.y)};
		
		return SDL_HasIntersection(&otherRect, &thisRect) == SDL_TRUE;
	} else {
		return checkCollisionRotated(other, transOther, transThis);
	}
}

bool RectangleCollider::checkCollision(const CircleCollider * other) const {
	fPoint globalCircle = other->getGlobalPoint();
	if (rotation == 0) {
		//Closest point on collision box
		iPoint closestPoint;

		iPoint circle(static_cast<int>(globalCircle.x), 
					static_cast<int>(globalCircle.y));

		Transform rectTransform = getGlobalTransform();

		fPoint global = rectTransform.position;
		SDL_Rect positionRect = {static_cast<int>(global.x),
			static_cast<int>(global.y),
			static_cast<int>(rect.x),
			static_cast<int>(rect.y)};

		//Find closest x offset
		if (circle.x < positionRect.x) {
			closestPoint.x = static_cast<int>(positionRect.x);
		} else if (circle.x > positionRect.x + positionRect.w) { 
			closestPoint.x = static_cast<int>(positionRect.x + positionRect.w);
		} else {
			closestPoint.x = circle.x;
		}

		//Find closest y offset
		if (circle.y < positionRect.y) {
			closestPoint.y = static_cast<int>(positionRect.y);
		} else if (circle.y > positionRect.y + positionRect.h) {
			closestPoint.y = static_cast<int>(positionRect.y + positionRect.h);
		} else {
			closestPoint.y = circle.y;
		}

		//If the closest point is inside the circle
		if (closestPoint.distanceTo(circle) < other->radius) {
			return true;
		}
		//If the shapes have not collided
		return false;


	} else {
		float angle = -(float) (rotation * M_PI / 180.0f);
		fPoint thisCenter = getCenter();
		fPoint circleCenter = globalCircle;

		// Calcula la posición del círculo como si el retángulo no estuviese rotado
		fPoint newCirclePosition;
		newCirclePosition.x = cos(angle) * (circleCenter.x - thisCenter.x) - sin(angle) * (circleCenter.y - thisCenter.y) + thisCenter.x;
		newCirclePosition.y = sin(angle) * (circleCenter.x - thisCenter.x) + cos(angle) * (circleCenter.y - thisCenter.y) + thisCenter.y;

		// Encuentra la x del rectángulo más próxima al nuevo centro
		float closestX;
		if (newCirclePosition.x  < thisCenter.x - rect.x * 1.0f / 2)
			closestX = thisCenter.x - rect.x * 1.0f / 2;
		else if (newCirclePosition.x  > thisCenter.x + rect.x * 1.0f / 2)
			closestX = thisCenter.x + rect.x * 1.0f / 2;
		else
			closestX = newCirclePosition.x;

		// Encuentra la y del rectángulo más próxima al nuevo centro
		float closestY;
		if (newCirclePosition.y  < thisCenter.y - rect.y * 1.0f / 2)
			closestY = thisCenter.y - rect.y * 1.0f / 2;
		else if (newCirclePosition.y  > thisCenter.y + rect.y * 1.0f / 2)
			closestY = thisCenter.y + rect.y * 1.0f / 2;
		else
			closestY = newCirclePosition.y;

		// Si tanto la x como la y más cercanas son el propio centro del círculo, el centro esta dentro y hay colisión
		if (closestX == newCirclePosition.x && closestY == newCirclePosition.y)
			return true;

		// Determina si la distancia al punto más cercano es inferior al radio
		return fPoint(closestX, closestY).distanceTo(newCirclePosition) < other->radius;
	}
}

bool RectangleCollider::checkCollision(const LineCollider * other) const { return other->checkCollision(this); }


std::vector<fPoint> RectangleCollider::getPoints(float totalRotation) const {
	//TODO testear bien, es posible que el metodo de linecollider sea el bueno incluso para aquí?
	fPoint center = position;
	if (parentTransform != nullptr) {
		center += parentTransform->getGlobalTransform().position;
	}
	float pointOffsetX = rect.x / 2.0f;
	float pointOffsetY = rect.y / 2.0f;
	center.x += pointOffsetX;
	center.y += pointOffsetY;
	std::vector<fPoint> points = {
		center + fPoint(-pointOffsetX, -pointOffsetY).rotate(totalRotation),
		center + fPoint(+pointOffsetX, -pointOffsetY).rotate(totalRotation),
		center + fPoint(+pointOffsetX, +pointOffsetY).rotate(totalRotation),
		center + fPoint(-pointOffsetX, +pointOffsetY).rotate(totalRotation)};
	return points;
}

fPoint RectangleCollider::getCenter() const {
	fPoint positionCenter = position;
	if (parentTransform != nullptr) {
		positionCenter += parentTransform->getGlobalTransform().position;
	}
	return positionCenter;
}