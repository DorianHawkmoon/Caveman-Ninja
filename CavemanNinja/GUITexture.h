#pragma once
#ifndef GUI_TEXTURE_H
#define GUI_TEXTURE_H

#include "GUIComponent.h"
#include "Point.h"
#include "ModuleTextures.h"
class Animation;

namespace GUI {
	class GUITexture : public GUI::GUIComponent {
	public:
		GUITexture() : GUIComponent(), texture(nullptr), animation(nullptr) {}
		virtual ~GUITexture();

		void draw(const GUITransform& states) const;

	protected:
		SDL_Texture* texture;
		Animation* animation;
	};
}

#endif // !GUI_TEXTURE_H
