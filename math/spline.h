#ifndef __SPLINE_H
#define __SPLINE_H

#pragma once

#include "vector.h"

// a stupid-simple spline with just two points and tangents at each.
//
// Unlike the full spline, this can be allocated on the stack without
// allocations from the heap.
//
class spline_simple {
public:
	spline_simple() {
		point1 = ZERO_VECTOR;
		point2 = ZERO_VECTOR;
		tan1 = RIGHT_VECTOR;
		tan2 = RIGHT_VECTOR;
		color1 = 0xFFFFFFFF;
		color2 = 0xFFFFFFFF;
		balance1 = balance2 = 1.0f;
		width1 = width2 = 1.0f;
	}
	vector2 point1;
	vector2 point2;

	vector2 tan1;
	vector2 tan2;

	uint color1;
	uint color2;

	float balance1;
	float balance2;

	float width1;
	float width2;

	void set_tangents(const vector2 &_tan1, const vector2 &_tan2);

	void get_point(vector2 &pt_out, float u_val, float lat_offset = 0.0f) const;
	uint get_color(float u_val, float lerp_scalar = 1.0f) const;
	float get_approximate_length() const;
	void get_approximate_tangent( vector2 &tangent_out, float u_val );
	void copy( const spline_simple & src );
};

class spline{
	private:
		int m_num_points;
		int m_max_points;
		float m_tension;
		vector2 *m_points;
		vector2 *m_tangents;
		uint *m_colors;

		float * m_lengths;	// approximate.  the last one is of length zero.
		float m_length;

		vector2 expanded_point_min, expanded_point_max;

		struct {
			bool length_locked : 1;
		} m_spline_flags;

	public:
		spline(int max_points, int num_points = 0, bool has_color = false);

		~spline();

		static float interpolate_one_dimension(float u, float Pk1, float Pk2, float dP1, float dP2);

		void spline_get_tangent(vector2 &tangent_out, int pt_index) const;

		void set_tension(float tension);
		void add_point(vector2 new_pt);
		void lock_length(bool locked) {m_spline_flags.length_locked = locked;}

		void get_point(vector2 &pt_out, int pt_index, float u_val, float lat_offset = 0.0f) const;
		void get_point(vector2 &pt_out, float t_val) const;
		void get_point_offset(vector2 &pt_out, float t_val, float offset) const;
		uint get_color(float t_val) const;

		void set_point_pos(int p_num, const vector2 &new_pos);
		void set_point_tangent(int p_num, const vector2 &new_tan);
		void set_point_color(int p_num, uint color);
		int get_num_points() const;
		int get_max_points() const;
		bool is_full() {return m_max_points == m_num_points;}

		float get_approximate_length() const {return m_length;}
		float get_approximate_segment_length(int seg_num) const;
		void get_approximate_tangent(vector2 &tangent_out, float t_val) const;
};

#endif //__SPLINE_H