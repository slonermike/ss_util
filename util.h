// Contains very basic utilities, such as macros and type definitions.
// Nothing here should be compiled.  This is included in nearly everything
// and should all be pre-compiler stuff, with the exception of what's included
// through assert.h

#ifndef __UTIL_H
#define __UTIL_H

#pragma once

#include "error_reporting.h"

#include <assert.h>
#include <math.h>
//#include <intsafe.h>

// use __LOC__ in a #pragma message() to show the file in which it occurs.
#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : Warning Msg: "

#ifndef _DEBUG
#define ASSERTS_AS_ERROR_REPORTS
#endif // _DEBUG

//#define ASSERTS_AS_ERROR_REPORTS

#ifdef _DEBUG
	#define debug_printf printf
#else
	#define debug_printf
#endif

// All the Assert calls will get compiled out in release.
#ifndef ASSERTS_AS_ERROR_REPORTS
	#define Assert(a) assert(a)
	#define Assert_compile_time(a) typedef char compile_assert[a ? 1 : -1]
#define Assert_once(a)						\
	if (!a) {								\
		static bool triggered = false;		\
		if (!triggered) {					\
			Assert(a);						\
			triggered = true;				\
		}									\
	}
#else
	#define Assert(a) if (!(a)) assert_report("ASSERT: " __LOC__ #a)
	#define Assert_compile_time(a)
	#define Assert_once(a) Assert(a)
#endif

/*
#define Assert_error(a,format,...) do {	\
	Assert(a);							\
	if (!a)								\
	error_report(format, ...);			\
} while (false)
*/
#define Assert_error(a, msg) Assert(a)

#define Assert_return(a) if (!(a)) {	\
	Assert(a);						\
	return;							\
}

#define Assert_break(a) if (!(a)) {	\
	Assert(a);						\
	break;							\
}

#define Assert_return_value(a, b) if (!(a)) {	\
	Assert(a);								\
	return b;								\
}
#define Assert_continue(a) if (!(a)) {	\
	Assert(a);							\
	continue;							\
}

typedef unsigned char ubyte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed long int64;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;
typedef float float32;
typedef double float64;

// Type conversions
#define i2fl(n) ((float)(n))
#define fl2i(n) ((int)(n))
#define d2fl(n) ((float)(n))

// Mega macros!
#define SWAP(a,b,T) T t; t = a; a = b; b = t;
#define SWAP_SECONDARY(a, b) t = a; a = b; b = t;
#define SQUARED(a) ((a)*(a))
#define MAX(a,b) ((a > b) ? a : b)
#define MIN(a,b) ((a < b) ? a : b)
#define LERP(a,b,pct) (a + ((b - a) * pct))
#define CAP(a,min,max)								\
do {												\
	a = (a < min) ? min : ((a > max) ? max : a);	\
} while (0)

#define fl_equals(a,b,t) (fabs(a - b) < t)
#define fl_floor(n) floor(n)
#define fl_abs(n) fabs(n)
#define fl_sign(n) ((n < 0.f) ? -1.0f : 1.0f)
#define fl_cap(a,min,max) (a > max ? max : (a < min ? min : a))

// trig
#define fl_cos(n)	cos(n)
#define fl_sin(n)	sin(n)
#define fl_tan(n)	tan(n)
#define fl_acos(n)	acos(n)
#define fl_asin(n)	asin(n)
#define fl_atan(n)	atan(n)

#define fl_is_nan(n) (n != n)

// Fun with constants!
#define MS_PER_SEC			(1000)
#ifndef PI
#define PI (3.14159f)
#define PI_OVER_2 (1.57079633f)
#define PI_2 (6.28318531f)
#endif

#define TWO_PI (6.28318531f)
#define COS_45	(0.707106781f)
#define COS_10	(0.98480775301f)
#define COS_5	(0.99619469809f)
#define RADIANS_PER_DEGREE (0.0174532925f)
#define DEGREES_PER_RADIAN (57.2957795f)

#define fl_deg_to_rad(d) (d * RADIANS_PER_DEGREE)
#define fl_rad_to_deg(r) (r * DEGREES_PER_RADIAN)

#ifndef NULL
#define NULL 0
#endif //NULL

#define FLAG(a) (1 << a)
#define SET_FLAG(f, bits) do {	\
	bits = bits | FLAG(f);		\
} while (0)
#define FLAG_IS_SET(f, bits) ((FLAG(f) & bits) != 0)

// These are handy for putting into templated lists--pools, vectors, etc.
struct string_container32 {
	char val[32];
};
struct string_container64 {
	char val[64];
};
struct string_container128 {
	char val[128];
};

// actual function definitions.
void mem_swap(void *dest, void *src, int size);

#endif //__UTIL_H