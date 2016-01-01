#pragma once
#ifndef CONDITION_H
#define CONDITION_H

class Condition {
public:
	Condition() {};
	~Condition() {};

	virtual bool start() { return true; }
	virtual bool check() const = 0;
	virtual bool cleanUp() { return true; }
};

#endif // CONDITION_H