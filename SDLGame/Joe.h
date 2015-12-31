#pragma once
#ifndef JOE_H
#define JOE_H

#include "Animation.h"
class Entity;

struct Joe {
	static Entity* makeJoe();

private:
	static void makeAnimations(Entity* entity);
};

#endif // JOE_H