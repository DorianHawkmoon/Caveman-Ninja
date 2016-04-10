#pragma once
#ifndef SCORE_H
#define SCORE_H

#include <string>

class Score {
public:
	Score();
	~Score();

	int getScore() const;
	void addScore(int amount);

	std::string getScoreText() const ;
	std::string getHighestScoreText() const;

	void resetScore();
	int getHighestScore() const;

private:
	unsigned int score;
	unsigned int highScore;

};

#endif // !SCORE_H

