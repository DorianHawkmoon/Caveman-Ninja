#pragma once
#ifndef CONDITION_H
#define CONDITION_H

class Condition {
public:
	Condition() {};
	~Condition() {};

	virtual bool check() const = 0;
};

#endif // CONDITION_H