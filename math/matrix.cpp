#include "matrix.h"

void rotate_around_point( const vector2 &pos, const matrix &orient, const vector2 &rotate_around, const matrix &rotation, vector2 &pos_out, matrix &orient_out )
{
	matrix rot_inverse = rotation;
	rot_inverse.invert();

	pos_out = pos;
	pos_out -= rotate_around;
	pos_out = pos_out * rot_inverse;
	pos_out += rotate_around;
	orient_out = orient;

	orient_out.rotate(rotation.extract_heading());
}

void rotate_around_point( const vector2 &pos, const vector2 &rotate_around, const matrix &rotation, vector2 &pos_out)
{
	matrix rot_inverse = rotation;
	rot_inverse.invert();

	pos_out = pos;
	pos_out -= rotate_around;
	pos_out = pos_out * rot_inverse;
	pos_out += rotate_around;
}

void world_to_local( const vector2 &pos_in, const matrix &orient_in, const vector2 &local_origin, const matrix &local_origin_orient, vector2 &pos_out, matrix &orient_out )
{
	pos_out = pos_in - local_origin;
	pos_out = pos_out * local_origin_orient;
	matrix inv = local_origin_orient;
	inv.invert();
	orient_out = orient_in * inv;
}

void world_to_local(const vector2 &pos_in, const vector2 &local_origin, const matrix &local_origin_orient, vector2 &pos_out)
{
	pos_out = pos_in - local_origin;
	pos_out = pos_out * local_origin_orient;
}

void local_to_world( const vector2 &pos_in, const matrix &orient_in, const vector2 &local_origin, const matrix &local_origin_orient, vector2 &pos_out, matrix &orient_out )
{
	matrix rotation = local_origin_orient;
	rotation.invert();
	pos_out = pos_in * rotation;
	pos_out += local_origin;
	orient_out = orient_in * local_origin_orient;
}

void local_to_world( const vector2 &pos_in, const vector2 &local_origin, const matrix &local_origin_orient, vector2 &pos_out )
{
	matrix rotation = local_origin_orient;
	rotation.invert();
	pos_out = pos_in * rotation;
	pos_out += local_origin;
}

void scale_pos( const vector2 &pos_in, const vector2 &scale, vector2 &pos_out )
{
	pos_out = vector2(pos_in.x * scale.x, pos_in.y * scale.y);
}

void scale_pos_and_orient( const vector2 &pos_in, const matrix &orient_in, const vector2 &scale, vector2 &pos_out, matrix &orient_out )
{
	pos_out = vector2(pos_in.x * scale.x, pos_in.y * scale.y);
	vector2 rvec = orient_in.rvec;
	if (scale.x < 0) {
		rvec.x *= -1.0f;
	}
	if (scale.y < 0) {
		rvec.y *= -1.0f;
	}
	orient_out.make_rvec(rvec);
}

void lerp_matrix( const matrix &from_orient, const matrix &to_orient, const float pct, matrix &orient_out )
{
	float start_angle = from_orient.extract_heading();
	float to_angle = to_orient.extract_heading();
	float lerped_angle = lerp_radians(start_angle, to_angle, pct);
	orient_out.make_heading(lerped_angle);
}
