#include "rand.h"

#include <ctime>

static bool Rand_inited = false;
bool rand_inited()
{
	return Rand_inited;
}

void rand_sys_init( uint seed /*= UINT_MAX*/ )
{
	// set the seed.
	if (seed != UINT_MAX) {
		srand ( (unsigned int)seed );
	} else {
		srand ( (uint)time(NULL) );
	}

	Rand_inited = true;
}