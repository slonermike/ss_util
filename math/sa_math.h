#ifndef __SA_MATH_H
#define __SA_MATH_H

#pragma once

#include "vector.h"
#include "matrix.h"

void rotate_around_point( const vector2 &pos, const matrix &orient, const vector2 &rotate_around, const matrix &rotation, vector2 &pos_out, matrix &orient_out );
void rotate_around_point( const vector2 &pos, const vector2 &rotate_around, const matrix &rotation, vector2 &pos_out);

void world_to_local(const vector2 &pos_in, const matrix &orient_in, const vector2 &local_origin, const matrix &local_origin_orient, vector2 &pos_out, matrix &orient_out);
void local_to_world(const vector2 &pos_in, const matrix &orient_in, const vector2 &local_origin, const matrix &local_origin_orient, vector2 &pos_out, matrix &orient_out);

void world_to_local(const vector2 &pos_in, const vector2 &local_origin, const matrix &local_origin_orient, vector2 &pos_out);
void local_to_world( const vector2 &pos_in, const vector2 &local_origin, const matrix &local_origin_orient, vector2 &pos_out );

void scale_pos(const vector2 &pos_in, const vector2 &scale, vector2 &pos_out);
void scale_pos_and_orient(const vector2 &pos_in, const matrix &orient_in, const vector2 &scale, vector2 &pos_out, matrix &orient_out);

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
vector2 lerp_heading(const vector2 &cur_heading, const vector2 &target_heading, float pct);

void lerp_matrix(const matrix &from_orient, const matrix &to_orient, const float pct, matrix &orient_out);

#endif // __SA_MATH_H