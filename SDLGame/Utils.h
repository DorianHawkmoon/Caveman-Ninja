#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <random>

namespace Utils {
	template <typename T> int sgn(T val) {
		return (T(0) < val) - (val < T(0));
	}

	//inclusive ranges!
	static int range(int max, int min = 0) {
		std::random_device rd; // obtain a random number from hardware
		std::mt19937 eng(rd()); // seed the generator, the type is mersenne twister pseudo-random generator
		std::uniform_int_distribution<> distr(min, max); // define the range
		return distr(eng);
	}
}
#endif // !UTILS_H
