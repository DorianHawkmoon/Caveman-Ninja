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
		GUITexture() : GUIComponent(), texture(nullptr), animation(nullptr), offset(0,0) {}
		virtual ~GUITexture();

		void draw(const GUITransform& states) const;

	protected:
		SDL_Texture* texture;
		Animation* animation;
		fPoint offset;

		fPoint getPosition(const GUITransform& transformParent) const;
	};
}

#endif // !GUI_TEXTURE_H
