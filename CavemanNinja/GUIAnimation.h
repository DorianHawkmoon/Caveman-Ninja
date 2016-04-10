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
		GUIAnimation(const std::string& nameTexture, StateMachine<Animation>* stateMachine);
		virtual ~GUIAnimation();

		inline bool isSelectable() const {
			return false;
		}

		virtual void handleEvent(const SDL_Event& event) {}

		update_status update();


	private:
		const std::string nameTexture;
		StateMachine<Animation>* state;

		bool cleaned;
	};
}
#endif // !GUI_ANIMATION_H