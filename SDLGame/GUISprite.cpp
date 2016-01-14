#include "GUISprite.h"

#include "Application.h"
#include "ModuleTextures.h"

GUI::GUISprite::GUISprite(const std::string & nameTexture) : nameTexture(nameTexture) {}

GUI::GUISprite::~GUISprite() {
}

bool GUI::GUISprite::start() {
	texture = App->textures->load(nameTexture.c_str());
	return texture != nullptr;
}

bool GUI::GUISprite::cleanUp() {
	App->textures->unload(texture);
	texture = nullptr;
	return true;
}
