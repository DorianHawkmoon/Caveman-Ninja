#pragma once
#ifndef GUI_SPRITE_H
#define GUI_SPRITE_H

#include "GUITexture.h"
#include <string>

namespace GUI {
	class GUISprite : public GUI::GUITexture {
	public:
		GUISprite(const std::string& nameTexture);
		virtual ~GUISprite();

		virtual bool start();
		virtual bool cleanUp();

		inline bool isSelectable() const {
			return false;
		}

		virtual void handleEvent(const SDL_Event& event) {}

	private:
		const std::string nameTexture;
	};

}
#endif // !GUI_SPRITE_H

