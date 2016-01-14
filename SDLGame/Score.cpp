#include "Score.h"
#include <sstream>

Score::Score() : score(0), highScore(0) {}

Score::~Score() {}

int Score::getScore() const {
	return score;
}

void Score::addScore(int amount) {
	score += amount;
	if (score > highScore) {
		highScore = score;
	}
}

std::string Score::getScoreText() const {
	std::stringstream ss;
	ss << score;
	return ss.str();
}

std::string Score::getHighestScoreText() const {
	std::stringstream ss;
	ss << highScore;
	return ss.str();
}

void Score::resetScore() {
	score = 0;
}

inline int Score::getHighestScore() const {
	return highScore;
}
