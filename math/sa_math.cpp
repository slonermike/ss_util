#include "sa_math.h"

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

// lerps between start_radians and end_radians according to pct (0-1).
// will output a value between 0 and 2pi.
//
float lerp_radians( float start_radians, float end_radians, const float pct )
{
	start_radians = clamp_radians_positive(start_radians);
	end_radians = clamp_radians_positive(end_radians);

	float min = 0.0f;
	float max = 2 * PI;
	float half = fl_abs((max - min)*0.5f);//half the distance between min and max
	float retval = 0.0f;
	float diff = 0.0f;

	if((end_radians - start_radians) < -half){
		diff = ((max - start_radians) + end_radians) * pct;
		retval =  start_radians+diff;
	}
	else if((end_radians - start_radians) > half){
		diff = -((max - end_radians)+start_radians) * pct;
		retval =  start_radians+diff;
	}
	else retval =  start_radians+(end_radians-start_radians) * pct;

	// Debug.Log("Start: "  + start + "   End: " + end + "  Value: " + value + "  Half: " + half + "  Diff: " + diff + "  Retval: " + retval);
	return retval;
}

void lerp_matrix( const matrix &from_orient, const matrix &to_orient, const float pct, matrix &orient_out )
{
	float start_angle = from_orient.extract_heading();
	float to_angle = to_orient.extract_heading();
	float lerped_angle = lerp_radians(start_angle, to_angle, pct);
	orient_out.make_heading(lerped_angle);
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

vector2 lerp_heading( const vector2 &cur_heading, const vector2 &target_heading, float pct )
{
	float rads = lerp_radians(cur_heading.extract_heading(), target_heading.extract_heading(), pct);
	vector2 out_val;
	out_val.make_heading(rads);
	return out_val;
}
