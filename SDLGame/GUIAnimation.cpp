#include "GUIAnimation.h"
#include "Application.h"
#include "ModuleTextures.h"


GUI::GUIAnimation::GUIAnimation(const std::string& nameTexture, StateMachine<Animation>* stateMachine) : GUI::GUITexture(), nameTexture(nameTexture), state(stateMachine) {
	texture = App->textures->load(nameTexture.c_str());
}

GUI::GUIAnimation::~GUIAnimation() {
	App->textures->unload(texture);
	texture = nullptr;
	if (state != nullptr) {
		delete state;
	}
}

update_status GUI::GUIAnimation::update() {
	state->getState()->getValue()->update();
	state->update();
	animation = state->getState()->getValue();
	return UPDATE_CONTINUE;
}
