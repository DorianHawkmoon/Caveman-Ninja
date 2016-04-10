#pragma once
#ifndef GUI_SCORE_H
#define GUI_SCORE_H

#include "GUILabel.h"
class Score;
namespace GUI {
	class GUIScore : public GUILabel {
	public:
		GUIScore(const Score* score, const SDL_Color& color, const std::string& font, const GUILocation& location, int size = 12);
		virtual ~GUIScore();

		update_status update();

	private:
		const Score* score;
		int previousValue;
	};
}

#endif // !GUI_SCORE_H
