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

// Mega macros!
#define SWAP(a,b,T) T t; t = a; a = b; b = t;
#define SWAP_SECONDARY(a, b) t = a; a = b; b = t;

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