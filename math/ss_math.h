#ifndef __SS_MATH_H
#define __SS_MATH_H

#pragma once

#include <cmath>

// Type conversions
#define i2fl(n) ((float)(n))
#define fl2i(n) ((int)(n))
#define d2fl(n) ((float)(n))

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

inline float clamp_radians_positive(float radians)
{
	if (radians > PI_2) {
		radians -= fl_floor(radians / PI_2) * PI_2;
	} else if (radians < 0) {
		radians += (fl_floor(radians / -PI_2) + 1.0f) * PI_2;
	}

	return radians;
}

inline float clamp_radians_pos_neg(float radians)
{
	radians = clamp_radians_positive(radians);
	if (radians > PI) {
		radians -= PI_2;
	}
	return radians;
}

float lerp_radians(float start_radians, float end_radians, const float pct);

#endif // __SS_MATH_H