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
		std::random_device rd;
		std::default_random_engine gen(rd());
		std::uniform_int_distribution<> dis(min, max);

		int value = dis(gen);
		return value;
	}
}
#endif // !UTILS_H
