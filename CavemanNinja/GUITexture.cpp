#include "GUITexture.h"
#include "Application.h"
#include "ModuleRender.h"
#include "Animation.h"


GUI::GUITexture::~GUITexture() {
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
	result.position = getPosition(states);
	//result.position += temp.rotate(states.rotation);
	//result.position += offset;
	result.pivot = transform.pivot;

	// Determina el frame que pintar
	SDL_Rect* renderArea = nullptr;
	if (animation != nullptr) {
		renderArea = &(animation->GetCurrentFrame());
	}

	//result.rotation += rotation;
	App->renderer->paintGUI(texture, result, renderArea);
}
