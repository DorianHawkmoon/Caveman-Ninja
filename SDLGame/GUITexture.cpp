#include "GUITexture.h"
#include "Application.h"
#include "ModuleRender.h"
#include "Animation.h"


GUI::GUITexture::~GUITexture() {
	if (animation != nullptr) {
		delete animation;
	}
	if (texture != nullptr) {
		App->textures->unload(texture);
	}
}

void GUI::GUITexture::draw(const GUITransform & states) const {
	if (texture == nullptr) {
		return;
	}

	GUITransform result = states;

	// Determina la posición de la imagen en pantalla
	fPoint temp = offset;
	result.position = getPosition(states);
	result.position += temp.rotate(states.rotation);
	result.position += offset;

	// Determina la rotación de la imagen en pantalla
	float rotation = states.rotation;
	SDL_Point pivot;
	pivot.x = 0;
	pivot.y = 0;

	// Determina el frame que pintar
	SDL_Rect* renderArea = nullptr;
	if (animation != nullptr) {
		renderArea = &(animation->GetCurrentFrame());
	}

	result.rotation += rotation;
	App->renderer->paintGUI(texture, result, renderArea);
}

fPoint GUI::GUITexture::getPosition(const GUITransform& transformParent) const {
	SDL_Rect viewArea = App->renderer->camera.getWindowsSize();
	fPoint position = transformParent.position;

	if(transform.location==ABSOLUTE){
		position += transform.position;
	}else {
		//start centered as default
		position += fPoint((float) viewArea.w / (2 * SCREEN_SIZE), (float) viewArea.h / (2 * SCREEN_SIZE));

		if ((transform.location & TOP) > 0) {
			position -= fPoint(0.0f, (float) viewArea.h / (2 * SCREEN_SIZE));
		}
		if ((transform.location & BOTTOM) > 0) {
			position += fPoint(0.0f, (float) viewArea.h / (2 * SCREEN_SIZE));
		}
		if ((transform.location & LEFT) > 0) {
			position -= fPoint((float) viewArea.w / (2 * SCREEN_SIZE), 0.0f);
		}
		if ((transform.location & RIGHT) > 0) {
			position += fPoint((float) viewArea.w / (2 * SCREEN_SIZE), 0.0f);
		}
	}

	position += offset;
	return position;
}
