#include "spline.h"

#include <cmath>
#include <memory>

#include "../gr.h"

//function definitions for spline class

spline::spline(int max_points, int num_points /* = 0 */, bool has_color /* = false */)
{
	// have to have at least 3 points.
	max_points = MAX(max_points, 3);
	num_points = MIN(num_points, max_points);

	m_max_points = max_points;
	m_num_points = num_points;
	m_tension = 1.f;
	m_length = 0.0f;
	m_points = new vector2[m_max_points];
	m_lengths = new float[m_max_points];
	m_tangents = new vector2[m_max_points];
	if (has_color) {
		m_colors = new uint[m_max_points];
	} else {
		m_colors = NULL;
	}

	memset(m_lengths, 0, sizeof(float) * num_points);
	memset(&m_spline_flags, 0, sizeof(m_spline_flags));

	if (m_colors) {
		for (int i = 0; i < m_num_points; i++) {
			m_colors[i] = 0xFFFFFFFF;
		}
	}
}

spline::~spline()
{
	if (m_points) {
		delete [] m_points;
	}
	if (m_lengths) {
		delete [] m_lengths;
	}
	if (m_tangents) {
		delete [] m_tangents;
	}
	if (m_colors) {
		delete [] m_colors;
	}
}

void spline::set_tension( float tension )
{
	m_tension = tension;
}

void spline::add_point( vector2 new_pt )
{
	if(m_num_points >= m_max_points)
		return;

	if (m_num_points >= 1) {
		m_lengths[m_num_points-1] = new_pt.dist(m_points[m_num_points-1]);
		m_length += m_lengths[m_num_points-1];
	}

	m_lengths[m_num_points] = 0.0f;
	m_points[m_num_points] = new_pt;
	m_num_points++;

	//set up the first virtual point
	if(m_num_points == 2)
	{
		expanded_point_min.x = m_points[0].x + m_points[0].x - m_points[1].x;
		expanded_point_min.y = m_points[0].y + m_points[0].y - m_points[1].y;
	}

	if(m_num_points >= 2)
	{
		expanded_point_max.x = m_points[m_num_points-1].x + m_points[m_num_points-1].x - m_points[m_num_points-2].x;
		expanded_point_max.y = m_points[m_num_points-1].y + m_points[m_num_points-1].y - m_points[m_num_points-2].y;
	}
}

void spline::spline_get_tangent( vector2 &tangent_out, int pt_index ) const
{
	Assert_return(pt_index >= 0 && pt_index < m_num_points);

	if (m_tangents[pt_index] != ZERO_VECTOR) {
		tangent_out = m_tangents[pt_index];
		return;
	}

	vector2 Pk1, Pk2;

	// if we exceed boundary conditions, or if there aren't enough points to determine
	// a slope, or if dP hasn't been initialized, return null
	if(m_num_points < 2 || pt_index > m_num_points-1 || pt_index < 0)
		return;

	//if it's the first point, use the virtual min point
	if(pt_index == 0)
		Pk1 = expanded_point_min;
	else
		Pk1 = (m_points[pt_index-1]);

	//if it's the last point, use the virtual max point
	if(pt_index == m_num_points-1)
		Pk2 = expanded_point_max;
	else
		Pk2 = (m_points[pt_index+1]);

	tangent_out = Pk2 - Pk1;
	tangent_out *= m_tension;
}

float spline::interpolate_one_dimension( float u, float Pk1, float Pk2, float dP1, float dP2 )
{
	float u2 = SQUARED(u);
	float u3 = u * u2;		//u cubed

	//blending function expanded
	return (2*u3 - 3*u2 + 1)*Pk1 +
		(-2*u3 + 3*u2)*Pk2 +
		(u3 - 2*u2 + u)*dP1 +
		(u3 - u2)*dP2;
}

void spline::get_point(vector2 &pt_out, int pt_index, float u_val, float lat_offset /* = 0.0f */) const
{
	if (pt_index >= m_num_points-1) {
		vector2 endpt_tan;
		spline_get_tangent(endpt_tan, m_num_points-1);
		pt_out = m_points[m_num_points-1] + (endpt_tan.rvec().copy_normalize() * lat_offset);
	} else if(pt_index < 0) {
		return;
	} else {
		vector2 pk1 = m_points[pt_index];
		vector2 pk2 = m_points[pt_index+1];
		vector2 slope1, slope2;

		spline_get_tangent(slope1, pt_index);
		spline_get_tangent(slope2, pt_index+1);

		if (lat_offset != 0.0f) {
			pk1 += slope1.rvec().copy_normalize() * lat_offset;
			pk2 += slope2.rvec().copy_normalize() * lat_offset;
		}

		pt_out.x = interpolate_one_dimension(u_val, pk1.x, pk2.x, slope1.x, slope2.x);
		pt_out.y = interpolate_one_dimension(u_val, pk1.y, pk2.y, slope1.y, slope2.y);
	}
}

void spline::set_point_tangent(int p_num, const vector2 &new_tan)
{
	Assert_return(p_num >= 0 && p_num < m_num_points);
	
	m_tangents[p_num] = new_tan;
}

void spline::set_point_color( int p_num, uint color )
{
	Assert_return(m_colors && p_num >= 0 && p_num < m_num_points);

	m_colors[p_num] = color;
}

void spline::set_point_pos(int pt_num, const vector2 &new_pos)
{
	if(pt_num < m_num_points)
	{
		m_points[pt_num] = new_pos;
	}

	if((pt_num == 0 || pt_num == 1) && m_num_points >= 2)
	{
		expanded_point_min = m_points[0] + (m_points[0] - m_points[1]);
	}

	if((pt_num == m_num_points-1 || pt_num == m_num_points-2) && m_num_points >= 2)
	{
		expanded_point_max = m_points[m_num_points-1] + m_points[m_num_points-1] - m_points[m_num_points-2];
	}

	if (m_num_points > 1) {
		// if it's not the last point, calculate its length.
		if (pt_num < m_num_points-1) {
			if (!m_spline_flags.length_locked) {
				m_length -= m_lengths[pt_num];
			}
			m_lengths[pt_num] = m_points[pt_num].dist(m_points[pt_num+1]);
			if (!m_spline_flags.length_locked) {
				m_length += m_lengths[pt_num];
			}
		}

		// If it's not the first point, calculate the previous one's length.
		if (pt_num > 0) {
			if (!m_spline_flags.length_locked) {
				m_length -= m_lengths[pt_num-1];
			}

			m_lengths[pt_num-1] = m_points[pt_num-1].dist(m_points[pt_num]);

			if (!m_spline_flags.length_locked) {
				m_length += m_lengths[pt_num-1];
			}
		}
	}
}

void spline::get_point( vector2 &pt_out, float t_val ) const
{
	CAP(t_val, 0, 1.0f);

	float expanded_t_val = t_val * i2fl(m_num_points-1);
	int pt_num = (int)fl_floor(expanded_t_val);
	float u = expanded_t_val - i2fl(pt_num);

	CAP(pt_num, 0, m_num_points-1);
	if (u == 0.0f) {
		pt_out = m_points[pt_num];
	} else {
		get_point(pt_out, pt_num, u);
	}
}


void spline::get_point_offset( vector2 &pt_out, float t_val, float offset ) const
{
	CAP(t_val, 0, 1.0f);

	float expanded_t_val = t_val * i2fl(m_num_points-1);
	int pt_num = (int)fl_floor(expanded_t_val);
	float u = expanded_t_val - i2fl(pt_num);

	CAP(pt_num, 0, m_num_points-1);
	get_point(pt_out, pt_num, u, offset);
}

int spline::get_num_points() const
{
	return m_num_points;
}

float spline::get_approximate_segment_length( int seg_num ) const
{
	if (seg_num < 0 || seg_num >= m_num_points) {
		return 0.0f;
	} else {
		return m_lengths[seg_num];
	}
}

// Hacky, inefficient shiz.  There's probably a "correct" way to do this that I'm not doing.
//
void spline::get_approximate_tangent( vector2 &tangent_out, float t_val ) const
{
	CAP(t_val, 0.0f, 1.0f);
	if (t_val == 0.0f) {
		spline_get_tangent(tangent_out, 0);
	} else if (t_val == 1.0f) {
		spline_get_tangent(tangent_out, m_num_points-1);
	} else {
		float prev_t = t_val - 0.001f;
		float next_t = t_val + 0.001f;
		CAP(prev_t, 0.0f, 1.0f);
		CAP(next_t, 0.0f, 1.0f);
		vector2 pos1, pos2;
		get_point(pos1, prev_t);
		get_point(pos2, next_t);
		if (pos1 == pos2) {
			tangent_out = UP_VECTOR;
		} else {
			tangent_out = pos2 - pos1;
		}
	}
}

int spline::get_max_points() const
{
	return m_max_points;
}

uint spline::get_color( float t_val ) const
{
	if (m_colors == NULL) {
		return 0xFFFFFFFF;
	}

	CAP(t_val, 0, 1.0f);

	float expanded_t_val = t_val * i2fl(m_num_points-1);
	int pt_num = (int)fl_floor(expanded_t_val);
	int next_pt_num = pt_num + 1;
	float u = expanded_t_val - i2fl(pt_num);

	CAP(pt_num, 0, m_num_points-1);
	CAP(next_pt_num, 0, m_num_points-1);

	return color_lerp(m_colors[pt_num], m_colors[next_pt_num], u);
}

void spline_simple::get_point( vector2 &pt_out, float u_val, float lat_offset /*= 0.0f*/ ) const
{
	CAP(u_val, 0.0f, 1.0f);
	
	vector2 pt1 = point1;
	vector2 pt2 = point2;

	vector2 tan_start = tan1;
	vector2 tan_end = tan2;

	if (lat_offset != 0.0f) {
		float width_mult = LERP(width1, width2, u_val);

		pt1 += tan1.rvec().copy_normalize() * lat_offset * width_mult;
		pt2 += tan2.rvec().copy_normalize() * lat_offset * width_mult;

		// Multiply the left side by the balance.
		if (lat_offset < 0) {
			tan_start *= balance1;
			tan_end *= balance2;
		}
	}

	if (u_val == 0.0f) {
		pt_out = pt1;
	} else if (u_val == 1.0f) {
		pt_out = pt2;
	} else {
		pt_out.x = spline::interpolate_one_dimension(u_val, pt1.x, pt2.x, tan_start.x, tan_end.x);
		pt_out.y = spline::interpolate_one_dimension(u_val, pt1.y, pt2.y, tan_start.y, tan_end.y);
	}
}

// VERY rough approximation of length.  Just the distance from one point to the next.
//
float spline_simple::get_approximate_length() const
{
	return point1.dist(point2);
}

// Get the interpolated color along the spline.
//
// lerp_scalar: percentage along the spline where it reaches color2.
//
uint spline_simple::get_color( float u_val, float lerp_scalar /*= 1.0f*/ ) const
{
	Assert(lerp_scalar >= 0.0f && lerp_scalar <= 1.0f);
	if (lerp_scalar <= 0.0f) {
		return color2;
	}

	u_val /= lerp_scalar;
	CAP(u_val, 0.0f, 1.0f);
	return color_lerp(color1, color2, u_val);
}

// TODO: Hacky, inefficient shiz.  There's probably a "correct" way to do this that I'm not doing.
// Note: the output vector is not necessarily a unit vector.
//
void spline_simple::get_approximate_tangent( vector2 &tangent_out, float u_val )
{
	CAP(u_val, 0.0f, 1.0f);
	if (u_val == 0.0f) {
		tangent_out = tan1;
	} else if (u_val == 1.0f) {
		tangent_out = tan2;
	} else {
		float prev_u = u_val - 0.001f;
		float next_u = u_val + 0.001f;
		CAP(prev_u, 0.0f, 1.0f);
		CAP(next_u, 0.0f, 1.0f);
		vector2 pos1, pos2;
		get_point(pos1, prev_u);
		get_point(pos2, next_u);
		if (pos1 == pos2) {
			tangent_out = UP_VECTOR;
		} else {
			tangent_out = pos2 - pos1;
		}
	}
}

void spline_simple::set_tangents( const vector2 &_tan1, const vector2 &_tan2 )
{
	// FIXME: I've not decided if it's good for this to silently fail.  There are a
	// lot of cases where these can get set to zero.
	if (_tan1 != ZERO_VECTOR)
		tan1 = _tan1;
	if (_tan2 != ZERO_VECTOR)
		tan2 = _tan2;
}

void spline_simple::copy( const spline_simple & src )
{
	memcpy(this, &src, sizeof(spline_simple));
}
