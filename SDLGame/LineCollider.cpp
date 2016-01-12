#include "LineCollider.h"
#include "CircleCollider.h"
#include <algorithm>

bool comparePointsByX(fPoint a, fPoint b) { return a.x < b.x; }

LineCollider::LineCollider(fPoint& position, std::vector<fPoint>& points, TypeCollider type) : Collider(position, type), points(points) {
	// Ordena los puntos de menor a mayor X
	std::sort(this->points.begin(), this->points.end(), comparePointsByX);
}

LineCollider::~LineCollider() {}

void LineCollider::paintCollider() const {
	// Determina el color del collider
	SDL_Color color;
	color.r = 179;
	color.g = 179;
	color.b = 0;
	color.a = 128;

	// Renderiza cada uno de los cuadrados que conforman la línea
	for (unsigned int leftBoundIndex = 0; leftBoundIndex < points.size() - 1; ++leftBoundIndex) {	// Empieza desde el primero y acaba en el penúltimo

															// Calcula el centro del segmento
															// Las coordenadas serán locales que luego hacemos globales
		fPoint leftBound = points[leftBoundIndex];
		fPoint rightBound = points[leftBoundIndex + 1];
		fPoint segment = rightBound - leftBound;
		//fPoint center = leftBound + segment * (1.0f / 2.0f);

		// Calcula las dimensiones del segmento
		int width = static_cast<int>(leftBound.distanceTo(rightBound) + thickness);
		int height = thickness;

		Transform global = getGlobalTransform();
		// Calcula la pendiente del segmento
		float rotation = (float) (atan2(segment.y, segment.x) * 180 / PI);
		//y lo sumo al total
		rotation += global.rotation;

		// Crea y devuelve el rectángulo adecuado
		fPoint position = leftBound + global.position;
		(RectangleCollider(position, iPoint(width, height), rotation, type)).paintCollider(iPoint(0,0));

		//createSegmentCollider(i).paintCollider(iPoint(0,0));
	}
}

bool LineCollider::checkCollision(const Collider * r) const {
	return r->checkSpecificCollision(this);
}

bool LineCollider::checkSpecificCollision(const Collider * self) const {
	return  self->checkCollision(this);
}

bool LineCollider::checkCollision(const RectangleCollider * other) const {
	bool result = false;

	//hago precalculos
	Transform otherTrans = other->getGlobalTransform();
	std::vector<fPoint> otherPoints = other->getPoints(otherTrans.rotation); //we are friends!! :D

	// Para cada segmento, comprueba si colisiona con la línea
	for (unsigned int i = 0; i < points.size() - 1 && !result; ++i) {	// Empieza desde el primero y acaba en el penúltimo
		RectangleCollider segment = createSegmentCollider(i);
		result= checkCollisionRectangle(segment, other, otherTrans, otherPoints);
	}
	return result;
}

bool LineCollider::checkCollision(const CircleCollider * other) const {
	// Por las propiedades del círculo, solo comprueba en el segmento que contiene la X del centro del círculo
	int leftBoundIndex = 0;
	bool found = false;
	fPoint center = other->getGlobalPoint();

	for (unsigned int i = 0; i < points.size() - 1 && !found; ++i)	// Empieza desde el primero y acaba en el penúltimo
		if (center.x <= getPointGlobalCoordinates(i + 1).x) {
			leftBoundIndex = i;
			found = true;
		}

	if (!found) {	// Está más a la derecha que el último punto, comprueba la colision con el último segmento
		leftBoundIndex = points.size() - 2;	// size - 2 es el índice del penúltimo elemento
	}

	//rotate the point of circle in dependant of the segment
	//get the rotation
	//fPoint leftBound = points[leftBoundIndex];
	//fPoint rightBound = points[leftBoundIndex + 1];
	//fPoint segment = rightBound - leftBound;
	//// Calcula la pendiente del segmento
	//float rotation = (float) (atan2(segment.y, segment.x) * 180 / PI);
	////new center
	//center=center.rotate(rotation);

	//RectangleCollider rect = RectangleCollider(leftBound, {static_cast<int>(segment.x), thickness}, 0, NONE_COLLIDER);
	//CircleCollider newCircle = CircleCollider(center,other->radius, NONE_COLLIDER);
	//return rect.checkCollision(&newCircle);


	// Crea el rectángulo del segmento adecuado y comprueba la colision
	RectangleCollider rc = createSegmentCollider(leftBoundIndex);
	rc.parentTransform = this->parentTransform;
	return rc.checkCollision(other);
}

bool LineCollider::checkCollision(const LineCollider * other) const {
	bool result = true;
	// Para cada segmento, comprueba si colisiona con la línea
	for (unsigned int i = 0; i < points.size() - 1 && result; ++i) {	// Empieza desde el primero y acaba en el penúltimo
		result = createSegmentCollider(i).checkSpecificCollision(other);
	}
	return result;	// Llegados a este punto, ningún segmento colisiona con la línea
}

fPoint LineCollider::getPointGlobalCoordinates(unsigned int index) const {
	fPoint coordinates = points[index];
	Transform global = getGlobalTransform();
	coordinates.x += global.position.x;// *= transform->GetGlobalScale().x;
	coordinates.y += global.position.y; // *= transform->GetGlobalScale().y;
	return coordinates.rotate(global.rotation);
}

RectangleCollider LineCollider::createSegmentCollider(int leftBoundIndex) const {
	// Calcula el centro del segmento
	// Las coordenadas serán locales que luego hacemos globales
	fPoint leftBound = points[leftBoundIndex];
	fPoint rightBound = points[leftBoundIndex + 1];
	fPoint segment = rightBound - leftBound;
	//fPoint center = leftBound + segment * (1.0f / 2.0f);

	// Calcula las dimensiones del segmento
	int width = static_cast<int>(leftBound.distanceTo(rightBound)*2 + thickness);
	int height = thickness;

	Transform global= getGlobalTransform();
	// Calcula la pendiente del segmento
	float rotation = (float) (atan2(segment.y, segment.x) * 180 / PI);
	//y lo sumo al total
	rotation += global.rotation;

	// Crea y devuelve el rectángulo adecuado
	fPoint position = leftBound + global.position;
	return RectangleCollider(position, iPoint(width, height), rotation, type);
}

bool LineCollider::checkCollisionRectangle(const RectangleCollider& segment, const RectangleCollider* other, const Transform & otherTrans, const std::vector<fPoint> otherPoints) const {
	std::vector<fPoint> segmentPoints= getPoints(segment);

	// Calcula los ejes de proyección
	std::vector<fPoint> axis = std::vector<fPoint>(4);
	axis[0] = segmentPoints[1] - segmentPoints[0];
	axis[1] = segmentPoints[3] - segmentPoints[0];
	axis[2] = otherPoints[1] - otherPoints[0];
	axis[3] = otherPoints[3] - otherPoints[0];

	iPoint segmentRect = segment.getSize();
	iPoint otherRect = other->getSize();

	// Comprueba los casos especiales, utilizando perpendiculares
	if (segmentRect.x == 0.0f) {
		axis[0] = fPoint(-axis[1].y, axis[1].x);
	}
	if (segmentRect.y == 0.0f) {
		axis[1] = fPoint(axis[0].y, -axis[0].x);
	}
	if (otherRect.x == 0.0f) {
		axis[2] = fPoint(-axis[3].y, axis[3].x);
	}
	if (otherRect.y == 0.0f) {
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

			factor = segmentPoints[j].x * axis[i].x + segmentPoints[j].y * axis[i].y;
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
			if (thisProyections[j] < thisMin) {
				thisMin = thisProyections[j];
			} else if (thisProyections[j] > thisMax) {
				thisMax = thisProyections[j];
			}

			if (otherProyections[j] < otherMin) {
				otherMin = otherProyections[j];
			} else if (otherProyections[j] > otherMax) {
				otherMax = otherProyections[j];
			}
		}

		// Comprueba si hay solapamiento en el eje
		if (thisMin < otherMax && otherMax < thisMax) {
			collides = true;
		} else if (otherMin < thisMax && thisMax < otherMax) {
			collides = true;
		} else {
			collides = false;
			break;
		}
	}

	return collides;
}

std::vector<fPoint> LineCollider::getPoints(const RectangleCollider& segment) const {
	fPoint center = segment.position;
	float totalRotation = segment.rotation;
	if (parentTransform != nullptr) {
		center += parentTransform->getGlobalTransform().position;
	}
	float pointOffsetX = segment.rect.x / 2.0f;
	float pointOffsetY = segment.rect.y / 2.0f;
	center.x += pointOffsetX;
	center.y += pointOffsetY;
	std::vector<fPoint> points = {
		center + fPoint(-pointOffsetX, -pointOffsetY).rotate(totalRotation),
		center + fPoint(+pointOffsetX, -pointOffsetY).rotate(totalRotation),
		center + fPoint(+pointOffsetX, +pointOffsetY).rotate(totalRotation),
		center + fPoint(-pointOffsetX, +pointOffsetY).rotate(totalRotation)};
	return points;
}