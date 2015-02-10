#ifndef __MATRIX_H
#define __MATRIX_H

#pragma once

// Much of this was taken from Gamasutra's "C++ Data Structures
// for Rigid-Body Physics (07/02/1999)
// http://www.gamasutra.com/features/19990702/data_structures_02.htm
// and adapted to fit my needs.

#include <cmath>
#include "vector.h"

class matrix {
public:
	vector2 rvec;			// Identity points right
	vector2 uvec;			// Identity points vertically

	matrix() {
		set_identity();
	}
	matrix(vector2 _rvec, vector2 _uvec) {
		uvec = _uvec;
		rvec = _rvec;
	}

// Slone (1/19/2010): Removing this for two reasons.  a) It should probably take the rvec. b) it puts me in places
// where I accidentally assign a vector to a matrix that I don't actually want that.

// 	matrix(vector2 _uvec) {
// 		make_uvec(_uvec);
// 	}

	const bool operator == (const matrix &m) const {
		return (rvec == m.rvec) && (uvec == m.uvec);
	}
	const bool operator != (const matrix &m) const {
		return (m != *this);
	}
	const matrix& operator = ( const matrix &m ) {
		rvec = m.rvec;
		uvec = m.uvec;
		return *this;
	}
	const matrix& operator += (const matrix &m) {
		rvec += m.rvec;
		uvec += m.uvec;
		return *this;
	}
	const matrix& operator -= (const matrix &m) {
		rvec -= m.rvec;
		uvec -= m.rvec;
		return *this;
	}
	const matrix& operator *= (const float s) {
		rvec *= s;
		uvec *= s;
		return *this;
	}

	const matrix& operator *= ( const matrix& m ) {
		matrix temp = *this;
		rvec = temp * m.rvec;
		uvec = temp * m.uvec;
		return *this;
	}

	inline void make_uvec(vector2 _uvec) {
		uvec = _uvec;
		rvec = _uvec.rvec();
	}

	inline void make_rvec(vector2 _rvec) {
		rvec = _rvec;
		uvec = _rvec.lvec();
	}

	inline void transpose() {
		vector2 new_rvec = vector2(rvec.x, uvec.x);
		vector2 new_uvec = vector2(rvec.y, uvec.y);
		rvec = new_rvec;
		uvec = new_uvec;
	}

	inline matrix transpose_copy() {
		return matrix(vector2(rvec.x, uvec.x), vector2(rvec.y, uvec.y));
	}

	inline float determinant() const {
		return (rvec.x * uvec.y) - (rvec.y * uvec.x);
	}

	const vector2& operator [] (long i) const {
		Assert(i < 2 && i >= 0);
		return *((&uvec) + i);
	}

	void rotate(float theta) {
		float sin = sinf(theta);
		float cos = cosf(theta);
		matrix rotation(vector2(cos, sin), vector2(-sin, cos));
		matrix old_orient = *this;
		*this = rotation * old_orient;
	}

	inline void set_identity() {
		rvec = vector2(1.0f, 0.0f);
		uvec = vector2(0.0f, 1.0f);
	}

	inline void make_heading(float heading) {
		set_identity();
		rotate(heading);
	}

	inline void invert() {
		make_heading(-extract_heading());
		Assert(uvec.is_normalized() && rvec.is_normalized());
	}

	// Something in me doesn't like this...
	// but I can't think (or don't know of)
	// of a better solution at the moment.
	//
	// Copy the matrix into a 4x4 transformation matrix
	// for openGL.
	inline void get_44_matrix(float *dest) const {
		for (uint i = 0; i < 16; i++) {
			dest[i] = 0.0f;
		}
		dest[0] = rvec.x;
		dest[1] = rvec.y;
		dest[4] = uvec.x;
		dest[5] = uvec.y;
		dest[10] = 1.0f;
		dest[15] = 1.0f;
	}

	// Get the heading angle of the matrix.
	//
	inline float extract_heading() const {
		return atan2(rvec.y, rvec.x);
	}

	inline void fix() {
		uvec.normalize();
		rvec.x = uvec.y;
		rvec.y = -uvec.x;
	}

	const matrix operator + (const matrix &m) const {
		return matrix(rvec + m.rvec, uvec + m.uvec);
	}
	const matrix operator - (const matrix &m) const {
		return matrix(rvec - m.rvec, uvec - m.uvec);
	}

	// scalar post-multiply
	const matrix operator * (const float &s) const {
		return matrix(rvec * s, uvec * s);
	}

	// pre-multiply by a scalar
	friend inline const matrix operator * (const float s, const matrix &m) {
		return m * s;
	}

	// MGS: note to self--swapping the internal functionality of these two functions
	// determines whether they are column vectors or row vectors.  In this case
	// they are column vectors, as was in the original text.
	//
	// post-multiplying a column-based matrix by a vector yields the same results
	// as pre-multiplying a row-based matrix by a vector.

	// post-multiply by a vector
	const vector2 operator * (const vector2 &v) const {
		return rvec * v.x + uvec * v.y;
	}

	// pre-multiply by a vector
	inline friend const vector2 operator * (const vector2 &v, const matrix &m) {
		return vector2(m.rvec.dot(v), m.uvec.dot(v));
	}

	// post-multiply by a matrix
	const matrix operator * (const matrix &m) const {
		matrix return_val(*this * m.rvec, *this * m.uvec);

		// I don't like doing this here, but don't have time to track down wacky
		// tiny floating point errors that quickly become enormous floating point errors.
		return_val.fix();
		return return_val;
	}
};

const matrix IDENTITY_MATRIX(vector2(1.0f, 0.0f), vector2(0.0f, 1.0f));

void rotate_around_point(const vector2 &pos, const vector2 &orient, const vector2 &rotate_around, const matrix &rotation, vector2 &pos_out, matrix &orient_out);
void rotate_around_point( const vector2 &pos, const vector2 &rotate_around, const matrix &rotation, vector2 &pos_out);

void world_to_local(const vector2 &pos_in, const matrix &orient_in, const vector2 &local_origin, const matrix &local_origin_orient, vector2 &pos_out, matrix &orient_out);
void local_to_world(const vector2 &pos_in, const matrix &orient_in, const vector2 &local_origin, const matrix &local_origin_orient, vector2 &pos_out, matrix &orient_out);

void world_to_local(const vector2 &pos_in, const vector2 &local_origin, const matrix &local_origin_orient, vector2 &pos_out);
void local_to_world( const vector2 &pos_in, const vector2 &local_origin, const matrix &local_origin_orient, vector2 &pos_out );

void scale_pos(const vector2 &pos_in, const vector2 &scale, vector2 &pos_out);
void scale_pos_and_orient(const vector2 &pos_in, const matrix &orient_in, const vector2 &scale, vector2 &pos_out, matrix &orient_out);

void lerp_matrix(const matrix &from_orient, const matrix &to_orient, const float pct, matrix &orient_out);

#endif