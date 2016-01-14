#include "GUIScore.h"
#include "Score.h"

GUI::GUIScore::GUIScore(const Score * score, const SDL_Color & color, const std::string & font, const GUILocation & location, int size): 
	GUILabel(score->getScoreText(), color, font, location, size), score(score), previousValue(score->getScore()) {

}

GUI::GUIScore::~GUIScore() {}

update_status GUI::GUIScore::update() {
	if (previousValue != score->getScore()) {
		previousValue = score->getScore();
		setText(score->getScoreText());
	}
	return UPDATE_CONTINUE;
}
