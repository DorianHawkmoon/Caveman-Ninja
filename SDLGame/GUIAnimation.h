#pragma once
#ifndef GUI_ANIMATION_H
#define GUI_ANIMATION_H

#include "GUITexture.h"
#include "StateMachine.h"
#include "Animation.h"
#include <string>

namespace GUI {

	class GUIAnimation : public GUI::GUITexture {
	public:
		GUIAnimation(const std::string& nameTexture);
		virtual ~GUIAnimation();

		update_status update();


	private:
		std::string nameTexture;
		StateMachine<Animation>* state;
		SDL_Texture* texture;

		bool cleaned;
	};
}
#endif // !GUI_ANIMATION_H