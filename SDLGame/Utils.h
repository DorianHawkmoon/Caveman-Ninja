#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <ctime>

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

//inclusive ranges!
static float range(float max, float min = 0) {
	std::srand(static_cast<int>(std::time(0))); // use current time as seed for random generator
	float random = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
	return min + random * (max - min);
}

#endif // !UTILS_H
