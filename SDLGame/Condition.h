#pragma once
#ifndef CONDITION_H
#define CONDITION_H

class Condition {
public:
	Condition() {};
	virtual ~Condition() {};

	virtual bool start() { return true; }
	virtual bool check() const = 0;
	virtual bool cleanUp() { return true; }

	virtual Condition* clone() const = 0;
};

#endif // CONDITION_H