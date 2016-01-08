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

	// Renderiza cada uno de los cuadrados que conforman la l�nea
	for (unsigned int i = 0; i < points.size() - 1; ++i) {	// Empieza desde el primero y acaba en el pen�ltimo
		createSegmentCollider(i).paintCollider(iPoint(0,0));
	}
}

bool LineCollider::checkCollision(const Collider * r) const {
	return r->checkSpecificCollision(this);
}

bool LineCollider::checkSpecificCollision(const Collider * self) const {
	return  self->checkCollision(this);
}

bool LineCollider::checkCollision(const RectangleCollider * other) const {
	bool result = true;
	// Para cada segmento, comprueba si colisiona con la l�nea
	for (unsigned int i = 0; i < points.size() - 1 && result; ++i) {	// Empieza desde el primero y acaba en el pen�ltimo
		result = createSegmentCollider(i).checkSpecificCollision(other);
	}
	return result;
}

bool LineCollider::checkCollision(const CircleCollider * other) const {
	// Por las propiedades del c�rculo, solo comprueba en el segmento que contiene la X del centro del c�rculo
	int leftBoundIndex = 0;
	bool found = false;
	fPoint center = other->getGlobalPoint();

	for (unsigned int i = 0; i < points.size() - 1 && !found; ++i)	// Empieza desde el primero y acaba en el pen�ltimo
		if (center.x <= getPointGlobalCoordinates(i + 1).x) {
			leftBoundIndex = i;
			found = true;
		}

	if (!found)	// Est� m�s a la derecha que el �ltimo punto, comprueba la colision con el �ltimo segmento
		leftBoundIndex = points.size() - 2;	// size - 2 es el �ndice del pen�ltimo elemento

	// Crea el rect�ngulo del segmento adecuado y comprueba la colision
	RectangleCollider rc = createSegmentCollider(leftBoundIndex);
	return rc.checkSpecificCollision(other);
}

bool LineCollider::checkCollision(const LineCollider * other) const {
	bool result = true;
	// Para cada segmento, comprueba si colisiona con la l�nea
	for (unsigned int i = 0; i < points.size() - 1 && result; ++i) {	// Empieza desde el primero y acaba en el pen�ltimo
		result = createSegmentCollider(i).checkSpecificCollision(other);
	}
	return result;	// Llegados a este punto, ning�n segmento colisiona con la l�nea
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
	// Las coordenadas ser�n locales que luego hacemos globales
	fPoint leftBound = points[leftBoundIndex];
	fPoint rightBound = points[leftBoundIndex + 1];
	fPoint segment = rightBound - leftBound;
	//fPoint center = leftBound + segment * (1.0f / 2.0f);

	// Calcula las dimensiones del segmento
	int width = static_cast<int>(leftBound.distanceTo(rightBound) + thickness);
	int height = thickness;

	Transform global= getGlobalTransform();
	// Calcula la pendiente del segmento
	float rotation = (float) (atan2(segment.y, segment.x) * 180 / PI);
	//y lo sumo al total
	rotation += global.rotation;

	// Crea y devuelve el rect�ngulo adecuado
	fPoint position = leftBound + global.position;
	return RectangleCollider(position, iPoint(width, height), rotation, type);
}
