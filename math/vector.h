#ifndef __VECTOR_H
#define __VECTOR_H

#pragma once

// Much of this was taken from Gamasutra's "C++ Data Structures
// for Rigid-Body Physics (07/02/1999)
// http://www.gamasutra.com/features/19990702/data_structures_02.htm
// and adapted to fit my needs.

#include <cmath>

#include "sa_math.h"
#include "../util.h"

#define VECTOR_NORMALIZE_TOLERANCE		(0.01f)

class vector2 {
public:
	float x;
	float y;
	vector2();
	vector2(float _x, float _y);
	inline float dist_squared(vector2 pos) const {
		return (SQUARED(x - pos.x) + SQUARED(y - pos.y));
	}
	inline float dist(vector2 pos) const {
		return sqrt(dist_squared(pos));
	}
	inline void floor_vals() {
		x = fl_floor(x);
		y = fl_floor(y);
	}
	inline void mirror() {
		SWAP(x, y, float);
	}
	inline bool is_nan() const {
		return fl_is_nan(x) || fl_is_nan(y);
	}
	// Allow indexing.
	float& operator [] (const long i) {
		return *((&x) + i);
	}
	const bool operator == (const vector2 &v) const {
		return (v.x == x) && (v.y == y);
	}
	const bool operator != (const vector2 &v) const {
		return (v.x != x) || (v.y != y);
	}
	const vector2 operator - () const {
		return vector2(-x, -y);
	}
	const vector2& operator = (const vector2 &v) {
		x = v.x;
		y = v.y;
		return *this;
	}
	const vector2& operator += (const vector2 &v) {
		x += v.x;
		y += v.y;
		return *this;
	}
	const vector2& operator -= (const vector2 &v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}
	const vector2& operator *= (const float c) {
		x *= c;
		y *= c;
		return *this;
	}
	const vector2& operator /= (const float c) {
		const float r = 1 / c;
		x *= r;
		y *= r;
		return *this;
	}
	const vector2 operator + (const vector2 &v) const {
		return vector2(x + v.x, y + v.y);
	}
	const vector2 operator - (const vector2 &v) const {
		return vector2(x - v.x, y - v.y);
	}

	// post-multiplication by scalar
	const vector2 operator * (const float c) const {
		return vector2(x * c, y * c);
	}

	// pre-multiplication by scalar
	friend inline const vector2 operator * (const float c, const vector2 &v) {
		return v * c;
	}

	const vector2 operator / (float c) const {
		c = 1 / c;
		return vector2(c * x, c * y);
	}

	// Returns perpendicular vector to the right (assuming this is a uvec).
	const vector2 rvec() const {
		return vector2(y, -x);
	}

	// Returns the perpendicular vector to the left
	const vector2 lvec() const {
		return vector2(-y, x);
	}

	const float dot( const vector2 &v) const {
		return (x*v.x) + (y*v.y);
	}

	// operator for dot
	const float operator * (const vector2 &v) const {
		return dot(v);
	}

	const inline float mag_squared() const {
		return this->dot(*this);
	}
	const inline float mag() const {
		return d2fl(sqrt(mag_squared()));
	}
	const inline vector2 copy_normalize() const {
		Assert(!(*this/mag()).is_nan());
		return (*this) / mag();
	}
	const inline vector2 copy_normalize_safe(const vector2 &default_val = vector2(1.0f,0.0f)) const {
		vector2 new_vec = *this;
		new_vec.normalize_safe(default_val);
		return new_vec;
	}
	inline void normalize() {
		Assert(!(*this/mag()).is_nan());
		(*this) /= mag();
	}
	const inline bool equals( const vector2& v, const float tolerance) const {
		return (fabs(x-v.x)) < tolerance && (fabs(y-v.y) < tolerance);
	}
	inline bool is_normalized() const {
		return fl_equals(mag_squared(), 1.0f, VECTOR_NORMALIZE_TOLERANCE);
	}
	inline float extract_heading() const {
		return (x != 0 || y != 0) ? atan2(y, x) : 0.0f;
	}
	inline void make_heading(float heading) {
		x = fl_cos(heading);
		y = fl_sin(heading);
	}
	void rotate(const float theta);
	void normalize_safe( const vector2 &default_val = vector2(1.0f, 0.0f) );
};

vector2 lerp_heading(const vector2 &cur_heading, const vector2 &target_heading, float pct);

const vector2 ZERO_VECTOR(0.0f, 0.0f);
const vector2 SCALE_ONE(1.0f, 1.0f);

// we're in 2d, so depending on perspective, i may use one or the other.
const vector2 FORWARD_VECTOR(0.0f, 1.0f);
const vector2 UP_VECTOR(0.0f, 1.0f);

const vector2 RIGHT_VECTOR(1.0f, 0.0f);

#endif // __VECTOR_H