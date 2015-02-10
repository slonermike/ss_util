#ifndef __RAND_H
#define __RAND_H

#pragma once

#include <cstdlib>
#include <climits>

#include "../util.h"
#include "ss_math.h"

bool rand_inited();
void rand_sys_init(uint seed = UINT_MAX);

// Standard rand() only occupies up to 15 bits.  This will
// occupy 31 bits.
// the 16th bit will never be occupied here, but i frankly don't care.
// INT_RAND_MAX is defined accordingly.
// Random number between 0 and INT_RAND_MAX
#define INT_RAND_MAX			(0x7fff7fff)
inline int int_rand()
{
	Assert(rand_inited());
	return ((rand() << 16) | rand());
}

inline int int_rand_range(int min, int max)
{
	if (min == max) {
		return min;
	}
	Assert(min < max);
	int rand_num = int_rand();
	return (rand_num % (max - min + 1)) + min;
}

// Random number between zero and one.
inline float fl_rand()
{
	return ((float)rand())/RAND_MAX;
}

inline float fl_rand_range(float min, float max)
{
	return (fl_rand() * (max - min)) + min;
}

inline bool fl_rand_chance(float chance)
{
	return fl_rand() <= chance;
}

#endif // __RAND_H