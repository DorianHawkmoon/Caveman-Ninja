#include "GUILifeBar.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "LifeComponent.h"
#include "ModuleRender.h"


GUI::GUILifeBar::GUILifeBar(const std::string & nameTexture, const LifeComponent* life) : pointColor(GUI::PointColor::COUNT_COLOR), nameTexture(nameTexture), life(life), longLife(18) {

}

GUI::GUILifeBar::~GUILifeBar() {

}

bool GUI::GUILifeBar::start() {
	//load texture
	texture = App->textures->load(nameTexture.c_str());
	return true;

}

bool GUI::GUILifeBar::cleanUp() {
	App->textures->unload(texture);
	return true;
}

update_status GUI::GUILifeBar::update() {
	int actualLife = life->getActualLife();
	int maxLife = life->getMaxLife();

	float percent=(float)actualLife / (float)maxLife;
	if (percent < 0.3f) {
		actualColor = PointColor::RED;
	} else if (percent < 0.7f) {
		actualColor = PointColor::YELLOW;
	} else {
		actualColor = PointColor::GREEN;
	}

	coloredPoint = static_cast<int>(longLife*percent)-1;

	return UPDATE_CONTINUE;
}

void GUI::GUILifeBar::draw(const GUITransform & states) const {
	if (texture == nullptr) {
		return;
	}

	GUITransform result = states;

	// Determina la posición de la imagen en pantalla
	fPoint temp = fPoint(0,0);
	result.position = getPosition(states);
	result.position += temp.rotate(states.rotation);

	SDL_Rect actual = pointColor[PointColor::EMPTY];
	for (unsigned int i = longLife; i > 0; --i) {
		if (i < coloredPoint) {
			actual = pointColor[actualColor];
		}
		App->renderer->paintGUI(texture, result, &actual);
		result.position.x -= actual.w * SCREEN_SIZE;;
	}
}

fPoint GUI::GUILifeBar::getPosition(const GUITransform & transformParent) const {
	fPoint position = transformParent.position;

	if (transform.location == ABSOLUTE) {
		position += transform.position;
	} else {
		iPoint sizeParent = getParent()->getSize();
		//start centered as default
		position += fPoint(sizeParent.x * 0.5f, sizeParent.y * 0.5f);

		if ((transform.location & TOP) > 0) {
			position -= fPoint(0.0f, sizeParent.y * 0.5f);
		}
		if ((transform.location & BOTTOM) > 0) {
			position += fPoint(0.0f, sizeParent.y * 0.5f);
		}
		if ((transform.location & LEFT) > 0) {
			position -= fPoint(sizeParent.x * 0.5f, 0.0f);
		}
		if ((transform.location & RIGHT) > 0) {
			position += fPoint(sizeParent.x * 0.5f, 0.0f);
		}
	}

	//offset is given in base units, instead of parent size, given in its final size
	//position += offset*SCREEN_SIZE;

	return position;
}
