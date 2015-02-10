#include "bbox.h"

#include <cstdio>

#define LINE_TOLERANCE			(0.00001f)

// for readability's sake.
inline void fl_min_max(float a, float b, float &min, float &max)
{
	if (a > b) {
		max = a;
		min = b;
	} else {
		max = b;
		min = a;
	}
}

static bool bbox_overlap_aux(const vector2 corners1[4], const vector2 corners2[4], float box1_heading)
{
	vector2 b1_rot[4];
	vector2 b2_rot[4];

	for (uint i = 0; i < 4; i++) {
		b1_rot[i] = corners1[i];
		b2_rot[i] = corners2[i];

		b1_rot[i].rotate(-1.0f * box1_heading);
		b2_rot[i].rotate(-1.0f * box1_heading);
	}

	// Make certain that box 1 is now axis aligned.
	Assert(fl_equals(b1_rot[0].y, b1_rot[1].y, 0.01f));
	Assert(fl_equals(b1_rot[1].x, b1_rot[2].x, 0.01f));
	Assert(fl_equals(b1_rot[2].y, b1_rot[3].y, 0.01f));
	Assert(fl_equals(b1_rot[3].x, b1_rot[0].x, 0.01f));

	vector2 b1_min(MIN(b1_rot[0].x, b1_rot[1].x), MIN(b1_rot[0].y, b1_rot[3].y));
	vector2 b1_max(MAX(b1_rot[0].x, b1_rot[1].x), MAX(b1_rot[0].y, b1_rot[3].y));

	vector2 b2_min = b2_rot[0];
	vector2 b2_max = b2_rot[0];

	// Find the extremities of the second box.
	for (uint i = 1; i < 4; i++) {
		if (b2_rot[i].x < b2_min.x) {
			b2_min.x = b2_rot[i].x;
		}
		if (b2_rot[i].x > b2_max.x) {
			b2_max.x = b2_rot[i].x;
		}

		if (b2_rot[i].y < b2_min.y) {
			b2_min.y = b2_rot[i].y;
		}
		if (b2_rot[i].y > b2_max.y) {
			b2_max.y = b2_rot[i].y;
		}
	}

	// Check for overlap in the x axis.
	bool x_overlaps = false;

	if (b1_min.x > b2_min.x && b1_min.x < b2_max.x) {
		x_overlaps = true;
	}

	if (!x_overlaps && b1_max.x > b2_min.x && b1_max.x < b2_max.x) {
		x_overlaps = true;
	}

	if (!x_overlaps && b2_min.x > b1_min.x && b2_min.x < b1_max.x) {
		x_overlaps = true;
	}

	if (!x_overlaps && b2_max.x > b1_min.x && b2_max.x < b1_max.x) {
		x_overlaps = true;
	}

	// The x-axis is a separating axis.  No overlap!  Return false.
	if (x_overlaps == false) {
		return false;
	}

	bool y_overlaps = false;

	if (b1_min.y > b2_min.y && b1_min.y < b2_max.y) {
		y_overlaps = true;
	}

	if (!y_overlaps && b1_max.y > b2_min.y && b1_max.y < b2_max.y) {
		y_overlaps = true;
	}

	if (!y_overlaps && b2_min.y > b1_min.y && b2_min.y < b1_max.y) {
		y_overlaps = true;
	}

	if (!y_overlaps && b2_max.y > b1_min.y && b2_max.y < b1_max.y) {
		y_overlaps = true;
	}

	// The y-axis is a separating axis.  No overlap!  Return false.
	if (y_overlaps == false) {
		return false;
	}

	// No separating axis was found, return true.
	return true;
}

// Separating axis theorum in 2d.
bool bbox_overlap(const bbox_oriented &bbox1, const bbox_oriented &bbox2)
{
	float heading1 = bbox1.orient.extract_heading();
	float heading2 = bbox2.orient.extract_heading();

	// copy in the locations of the corners starting at the bottom left
	// and moving counter-clockwise.
	vector2 box1_corners[4];
	vector2 box2_corners[4];

	vector2 box1_half_size = bbox1.size / 2;
	vector2 box2_half_size = bbox2.size / 2;

	box1_corners[0] = bbox1.center - (box1_half_size.x * bbox1.orient.rvec) - (box1_half_size.y * bbox1.orient.uvec);
	box2_corners[0] = bbox2.center - (box2_half_size.x * bbox2.orient.rvec) - (box2_half_size.y * bbox2.orient.uvec);

	box1_corners[1] = bbox1.center + (box1_half_size.x * bbox1.orient.rvec) - (box1_half_size.y * bbox1.orient.uvec);
	box2_corners[1] = bbox2.center + (box2_half_size.x * bbox2.orient.rvec) - (box2_half_size.y * bbox2.orient.uvec);

	box1_corners[2] = bbox1.center + (box1_half_size.x * bbox1.orient.rvec) + (box1_half_size.y * bbox1.orient.uvec);
	box2_corners[2] = bbox2.center + (box2_half_size.x * bbox2.orient.rvec) + (box2_half_size.y * bbox2.orient.uvec);

	box1_corners[3] = bbox1.center - (box1_half_size.x * bbox1.orient.rvec) + (box1_half_size.y * bbox1.orient.uvec);
	box2_corners[3] = bbox2.center - (box2_half_size.x * bbox2.orient.rvec) + (box2_half_size.y * bbox2.orient.uvec);

	bool found_separating_axis = !bbox_overlap_aux(box1_corners, box2_corners, heading1);
	if (found_separating_axis) {
		return false;
	}

	found_separating_axis = !bbox_overlap_aux(box2_corners, box1_corners, heading2);
	if (found_separating_axis) {
		return false;
	}

	/*no separating axis found,
	the two boxes overlap */

	return true;

}

bool bbox_overlap(const bbox_aligned &bbox1, const bbox_aligned &bbox2)
{
	float x1min, x1max, y1min, y1max;
	fl_min_max(bbox1.bbmin.x, bbox1.bbmax.x, x1min, x1max);
	fl_min_max(bbox1.bbmin.y, bbox1.bbmax.y, y1min, y1max);

	float x2min, x2max, y2min, y2max;
	fl_min_max(bbox2.bbmin.x, bbox2.bbmax.x, x2min, x2max);
	fl_min_max(bbox2.bbmin.y, bbox2.bbmax.y, y2min, y2max);

	if (x1max < x2min) {
		return false;
	}
	if (x2max < x1min) {
		return false;
	}

	if (y1max < y2min) {
		return false;
	}
	if (y2max < y1min) {
		return false;
	}

	return true;
}

void bbox_convert_to_oriented(const bbox_aligned &source, bbox_oriented &dest)
{
	dest.center = (source.bbmin + source.bbmax) * 0.5f;
	dest.size = source.bbmax - source.bbmin;
	dest.orient = IDENTITY_MATRIX;
}

bool bbox_overlap(const bbox_oriented &o_box, const bbox_aligned &a_box)
{
	Assert_return_value(a_box.bbmin.x <= a_box.bbmax.x && a_box.bbmin.y <= a_box.bbmax.y, false);

	bbox_oriented converted_box;
	bbox_convert_to_oriented(a_box, converted_box);

	return bbox_overlap(converted_box, o_box);
}

bool bbox_intersects_point(const bbox_aligned &bbox, const vector2 &pt)
{
	if (pt.x > bbox.bbmax.x || pt.x < bbox.bbmin.x) {
		return false;
	}

	if (pt.y > bbox.bbmax.y || pt.y < bbox.bbmin.y) {
		return false;
	}

	return true;
}

bool bbox_intersects_circle(const bbox_aligned &bbox, const bcircle &circle)
{
	float radius = circle.radius;
	vector2 center = circle.center;

	// make sure the min and max are correct.
	Assert(bbox.bbmin.x <= bbox.bbmax.x);
	Assert(bbox.bbmin.y <= bbox.bbmax.y);

	// check if the point is inside the box
	if (bbox_intersects_point(bbox, center)) {
		return true;
	}

	// check the corners.
	float radius_sq = SQUARED(radius);

	// if it's beyond the bottom left
	if (center.x < bbox.bbmin.x && center.y < bbox.bbmin.y) {
		if (center.dist_squared(bbox.bbmin) < radius_sq) {
			return true;
		} else {
			return false;
		}
	}

	// if it's beyond the top right
	if (center.x > bbox.bbmax.x && center.y > bbox.bbmax.y) {
		if (center.dist_squared(bbox.bbmax) < radius_sq) {
			return true;
		} else {
			return false;
		}
	}

	vector2 top_left(bbox.bbmin.x, bbox.bbmax.y), bottom_right(bbox.bbmax.x, bbox.bbmin.y);

	// if it's beyond the top left
	if (center.x < top_left.x && center.y > top_left.y) {
		if (center.dist_squared(top_left) < radius_sq) {
			return true;
		} else {
			return false;
		}
	}

	// if it's beyond the bottom right
	if (center.x > bottom_right.x && center.y < bottom_right.y) {
		if (center.dist_squared(bottom_right) < radius_sq) {
			return true;
		} else {
			return false;
		}
	}

	// not inside, not touching a corner, check the sides.
	// second check for a basic separating axis (cicle touching edge)
	bbox_aligned circle_box;
	circle_box.bbmin.x = center.x - radius;
	circle_box.bbmax.x = center.x + radius;
	circle_box.bbmin.y = center.y - radius;
	circle_box.bbmax.y = center.y + radius;
	if (bbox_overlap(circle_box, bbox)) {
		return true;
	}

	// not inside, not touching an edge, not touching a corner, must be outside.
	return false;
}

bool bbox_intersects_circle(const bbox_oriented &bbox, const bcircle &circle)
{
	bcircle transformed_circle;

	// translate so the box center is relatively zero.
	transformed_circle.center = circle.center - bbox.center;
	transformed_circle.radius = circle.radius;

	// unrotate according to the box.
	float box_heading = bbox.orient.extract_heading();
	transformed_circle.center.rotate(-1.0f * box_heading);

	bbox_aligned aabb;
	aabb.bbmax = bbox.size * 0.5f;
	aabb.bbmin = -aabb.bbmax;

	return bbox_intersects_circle(aabb, transformed_circle);
}

bool lines_coincide(const line &a, const line &b)
{
	// check if parallel
	if (a.dir.dot(b.dir) != 1.0f) {
		return false;
	}

	// check for 100% overlap
	vector2 pt_to_pt = a.start - b.start;
	float dot = fl_abs(a.dir.dot(pt_to_pt.copy_normalize()));
	if (pt_to_pt == ZERO_VECTOR || fl_equals(dot, 1.0f, LINE_TOLERANCE)) {
		return true;
	} else {
		return false;
	}
}

bool line_intersects_line(const line &_a, const line &_b, vector2 *ix_point_out /*= NULL*/)
{
	line a = _a;
	line b = _b;

	Assert(a.dir.is_normalized());
	Assert(b.dir.is_normalized());

	// parallel--no intersection.
	float a_dot_b = a.dir.dot(b.dir);
	if (fl_equals(a_dot_b, 1.0f, LINE_TOLERANCE)) {
		// check for 100% overlap
		vector2 pt_to_pt = a.start - b.start;
		float dot = fl_abs(a.dir.dot(pt_to_pt.copy_normalize()));
		if (pt_to_pt == ZERO_VECTOR || fl_equals(dot, 1.0f, 0.000001f)) {
			if (ix_point_out) {
				*ix_point_out = a.start;
			}
			return true;
		} else {
			return false;
		}
	} else if (ix_point_out == NULL) {
		return true;
	}

	bool mirrored = false;
	bool a_vertical = a.dir.x == 0.0f;
	bool b_vertical = b.dir.x == 0.0f;
	bool a_horiz = a.dir.y == 0.0f;
	bool b_horiz = b.dir.y == 0.0f;

	// in this case, we can't escape the verticality by rotating.
	if (a_vertical && b_horiz) {
		if (ix_point_out) {
			*ix_point_out = vector2(a.start.x, b.start.y);
		}
		return true;
	} else if (a_horiz && b_vertical) {
		if (ix_point_out) {
			*ix_point_out = vector2(b.start.x, a.start.y);
		}
		return true;
	}

	// shouldn't both be vertical--they'd be parallel!
	Assert_return_value(!a_vertical || !b_vertical, false);

	// mirror it, so we don't end up with an infinity value.
	if (a_vertical || b_vertical) {
		mirrored = true;
		a.dir.mirror();
		a.start.mirror();
		b.dir.mirror();
		b.start.mirror();
	}

	// todo - this could hit if one is totally perpendicular, and the other is totally horizontal.
	// need to handle that case.
	Assert_return_value(a.dir.x != 0.0f && b.dir.x != 0.0f, false);

	// slope = rise/run
	float m1 = a.dir.y / a.dir.x;
	float m2 = b.dir.y / b.dir.x;

	float y_int1, y_int2;
	if (!a.y_intercept(y_int1) || !b.y_intercept(y_int2)) {
		Assert_return_value(0, false);
	}

	float intersect_x = (y_int2 - y_int1) / (m1 - m2);
	if (ix_point_out) {
		// try to avoid crazy float precision errors by using a non-vertical line.
		if (fl_equals(a.dir.x, 0, LINE_TOLERANCE)) {
			*ix_point_out = vector2(intersect_x, (m2*intersect_x) + y_int2);
		} else {
			*ix_point_out = vector2(intersect_x, (m1*intersect_x) + y_int1);
		}
		if (mirrored) {
			ix_point_out->mirror();
		}
	}

	return true;
}

bool segment_intersects_segment( const line_segment &_a, const line_segment &_b, vector2 *ix_point_out /*= NULL*/ )
{
	if (_a.a == _a.b || _b.a == _b.b) {
		// todo - add pt intersects segment here.
		return false;
	}

	line_segment a = _a, b = _b;
	a.correct();
	b.correct();

	line line_a;
	line_a.start = a.a;
	line_a.dir = (a.b - a.a).copy_normalize();

	line line_b;
	line_b.start = b.a;
	line_b.dir = (b.b - b.a).copy_normalize();

	vector2 ix_pt;
	// for lines that overlap...
	if (lines_coincide(line_a, line_b)) {
		bool ix = false;

		// for vertical lines
		if (a.a.x == a.b.x) {
			a.a.mirror();
			a.b.mirror();
			b.b.mirror();
			b.a.mirror();
		}

		if (a.a.x >= b.a.x && a.a.x <= b.b.x) {
			ix = true;
			ix_pt = _a.a;
		} else if (a.b.x >= b.a.x && a.b.x <= b.b.x) {
			ix = true;
			ix_pt = _a.b;
		} else if (b.a.x >= a.a.x && b.a.x <= a.b.x) {
			ix = true;
			ix_pt = _b.a;
		} else if (b.b.x >= a.a.x && b.b.x <= a.b.x) {
			ix = true;
			ix_pt = _b.b;
		}

		if (ix && ix_point_out) {
			*ix_point_out = ix_pt;
		}
		return ix;
	}

	if (!line_intersects_line(line_a, line_b, &ix_pt)) {
		return false;
	}

	// check if it's outside the first line
	{
		if (line_a.dir.x == 0.0f) {
			// above the top of segment a
			if (ix_pt.y > a.b.y) {
				return false;

			// below the bottom of segment a
			} else if (ix_pt.y < a.a.y) {
				return false;
			}
		} else {
			// beyond the right of segment a
			if (ix_pt.x > a.b.x) {
				return false;

			// beyond the left of segment a
			} else if (ix_pt.x < a.a.x) {
				return false;
			}
		}
	}

	// check if it's outside the second line
	{
		if (line_b.dir.x == 0.0f) {
			// above the top of segment b
			if (ix_pt.y > b.b.y) {
				return false;

				// below the bottom of segment b
			} else if (ix_pt.y < b.a.y) {
				return false;
			}
		} else {
			// beyond the right of segment b
			if (ix_pt.x > b.b.x) {
				return false;

				// beyond the left of segment b
			} else if (ix_pt.x < b.a.x) {
				return false;
			}
		}
	}

	if (ix_point_out) {
		*ix_point_out = ix_pt;
	}
	return true;
}

bool ray_intersects_segment( const line &_ray, const line_segment &_seg, vector2 *ix_point_out /*= NULL*/ )
{
	if (_seg.a == _seg.b) {
		// todo - add pt intersects line here.
		return false;
	}

	line_segment seg = _seg;
	line ray = _ray;
	// this will make a.x less than b.x.
	seg.correct();

	// we're going to work along the x axis, so if it's vertical, mirror it.
	bool mirrored = false;
	if (seg.a.x == seg.b.x) {
		mirrored = true;
		seg.a.mirror();
		seg.b.mirror();
		ray.dir.mirror();
		ray.start.mirror();
	}

	line seg_line;
	seg_line.start = seg.a;
	seg_line.dir = (seg.b - seg.a).copy_normalize();

	// if they totally overlap, then we're good...maybe.
	if (lines_coincide(seg_line, ray)) {
		vector2 ray_to_a = seg.a - ray.start;
		vector2 ray_to_b = seg.b - ray.start;
		bool hits = false;
		if (ray_to_a.dot(ray.dir) > 0.0f) {
			*ix_point_out = seg.a;
			hits = true;
		} else if (ray_to_b.dot(ray.dir) > 0.0f) {
			*ix_point_out = seg.b;
			hits = true;
		}

		if (mirrored) {
			ix_point_out->mirror();
		}

		return hits;
	}

	vector2 line_ix_pt;
	bool line_ix = line_intersects_line(seg_line, ray, &line_ix_pt);
	if (!line_ix) {
		return false;
	}

	// check if it's behind the ray's direction.
	vector2 start_to_ix = line_ix_pt - ray.start;
	if (start_to_ix.dot(ray.dir) < 0) {
		return false;
	}

	if (line_ix_pt.x < seg.a.x || line_ix_pt.x > seg.b.x) {
		return false;
	} else {
		*ix_point_out = line_ix_pt;
		if (mirrored) {
			ix_point_out->mirror();
		}
		return true;
	}
}

bool bbox_intersects_ray(const bbox_oriented &bbox, const line &ray)
{
	return false;
}

bool circle_overlap(const bcircle &circle1, const bcircle &circle2)
{
	if (circle1.radius == 0 || circle2.radius == 0) {
		return false;
	}
	
	float mag_sq = circle1.center.dist_squared(circle2.center);
	return mag_sq < SQUARED(circle1.radius + circle2.radius);
}

void bbox_aligned::move(const vector2 &delta)
{
	bbmin += delta;
	bbmax += delta;
}

void bbox_aligned::copy(const bounding_area *src)
{
	Assert_return(src);
	if (src->get_type() == BAT_UNIFIED) {
		const bounding_area_unified *uni = (const bounding_area_unified*)src;
		Assert_return(uni->inner_type == BAT_BOX_ORIENTED);
		bbox_oriented bbo;
		bbo.copy(uni);
		copy(&bbo);
	} else if (src->get_type() == BAT_BOX_ORIENTED) {
		bbox_oriented *bbo = (bbox_oriented*)src;
		vector2 corners[4];
		vector2 half_vert = 0.5f * bbo->orient.rvec * bbo->size.x;
		vector2 half_horiz = 0.5f * bbo->orient.uvec * bbo->size.y;
		corners[0] = bbo->center + half_vert + half_horiz;
		corners[1] = bbo->center + half_vert - half_horiz;
		corners[2] = bbo->center - half_vert - half_horiz;
		corners[3] = bbo->center - half_vert + half_horiz;

		// find the extremities.
		bbmax = corners[0];
		bbmin = corners[2];
		for (int i = 0; i < 4; i++) {
			bbmin.x = MIN(bbmin.x, corners[i].x);
			bbmax.x = MAX(bbmax.x, corners[i].x);

			bbmin.y = MIN(bbmin.y, corners[i].y);
			bbmax.y = MAX(bbmax.y, corners[i].y);
		}
	} else {
		Assert_return(src->get_type() == BAT_BOX_ALIGNED);
		copy(*((const bbox_aligned*)src));
	}
}

int bbox_aligned::collides(const line_segment &seg, vector2 *first_hit_out/* = NULL*/, vector2 *second_hit_out/* = NULL*/) const
{
	// i could do this more optimally, but i almost never used bbox aligned, so
	// it wouldn't make much difference.
	bbox_oriented box;
	box.copy(*this);
	return box.collides(seg, first_hit_out, second_hit_out);
}

bool bbox_aligned::collides(const bounding_area &area) const
{
	switch (area.get_type()) {
		case BAT_CIRCLE:
		{
			const bcircle &circle = (const bcircle &)(area);
			return bbox_intersects_circle(*this, circle);
		}
		case BAT_BOX_ORIENTED:
		{
			const bbox_oriented &box = (const bbox_oriented &)(area);
			return bbox_overlap(box, *this);
		}
		case BAT_BOX_ALIGNED:
		{
			bbox_aligned &box = (bbox_aligned &)(area);
			box.correct_corners();
			return bbox_overlap(*this, box);
		}
		case BAT_UNIFIED:
		{
			const bounding_area_unified *uni = (const bounding_area_unified*)&area;
			if (uni->inner_type == BAT_BOX_ORIENTED) {
				bbox_oriented bbox;
				bbox.copy(uni);
				return bbox_overlap(bbox, *this);
			} else if (uni->inner_type == BAT_CIRCLE) {
				bcircle circle;
				circle.copy(uni);
				return bbox_intersects_circle(*this, circle);
			}
		}
		default:
		{
			Assert(0);
			return false;
		}
	}
}

void bbox_oriented::move(const vector2 &delta)
{
	center += delta;
}

void bbox_oriented::copy(const bounding_area *src)
{
	Assert_return(src);
	if (src->get_type() == BAT_UNIFIED) {
		const bounding_area_unified *uni = (const bounding_area_unified*)src;
		Assert_return(uni->inner_type == BAT_BOX_ORIENTED);
		center = uni->center;
		size = uni->size;
		orient = uni->orient;
	} else {
		Assert_return(src->get_type() == BAT_BOX_ALIGNED || src->get_type() == BAT_BOX_ORIENTED);
		if (src->get_type() == BAT_BOX_ALIGNED) {
			copy(*(const bbox_aligned*)src);
		} else if (src->get_type() == BAT_BOX_ORIENTED) {
			copy(*(const bbox_oriented*)src);
		}
	}
}

int bbox_oriented::collides(const line_segment &seg, vector2 *first_hit_out /* = NULL */, vector2 *second_hit_out /* = NULL */) const
{
	line_segment segs[4];
	for (int i = 0; i < 4; i++) {
		segs[i].a = center;
		segs[i].b = center;
	}

	// left side.
	segs[0].a -= orient.uvec * size.y * 0.5f;
	segs[0].b += orient.uvec * size.y * 0.5f;
	segs[0].a -= orient.rvec * size.x * 0.5f;
	segs[0].b -= orient.rvec * size.x * 0.5f;

	// right side.
	segs[1].a -= orient.uvec * size.y * 0.5f;
	segs[1].b += orient.uvec * size.y * 0.5f;
	segs[1].a += orient.rvec * size.x * 0.5f;
	segs[1].b += orient.rvec * size.x * 0.5f;

	// top
	segs[2].a += orient.uvec * size.y * 0.5f;
	segs[2].b += orient.uvec * size.y * 0.5f;
	segs[2].a -= orient.rvec * size.x * 0.5f;
	segs[2].b += orient.rvec * size.x * 0.5f;

	// bottom
	segs[3].a -= orient.uvec * size.y * 0.5f;
	segs[3].b -= orient.uvec * size.y * 0.5f;
	segs[3].a -= orient.rvec * size.x * 0.5f;
	segs[3].b += orient.rvec * size.x * 0.5f;

	vector2 hit1;
	vector2 hit2;
	vector2 *out = &hit1;
	int num_hits = 0;
	for (int i = 0; i < 4; i++) {
		if (segment_intersects_segment(segs[i], seg, out)) {
			num_hits++;
			if (num_hits == 1) {
				out = &hit2;
			} else if (num_hits == 2) {
				break;
			}
		}
	}

	if (num_hits <= 0) {
		return 0;
	} else if (num_hits == 1) {
		if (first_hit_out) {
			*first_hit_out = hit1;
		}
		return 1;
	} else {
		Assert_return_value(num_hits == 2, 0);
		if (first_hit_out || second_hit_out) {
			// find the one closer to the source, and that's our first.
			float dist_sq1 = hit1.dist(seg.a);
			float dist_sq2 = hit2.dist(seg.a);
			if (dist_sq1 <= dist_sq2) {
				if (first_hit_out) {
					*first_hit_out = hit1;
				}
				if (second_hit_out) {
					*second_hit_out = hit2;
				}
			} else {
				if (first_hit_out) {
					*first_hit_out = hit2;
				}
				if (second_hit_out) {
					*second_hit_out = hit1;
				}
			}
		}
		return 2;
	}
}

bool bbox_oriented::collides(const bounding_area &area) const
{
	switch (area.get_type()) {
		case BAT_CIRCLE:
		{
			const bcircle &circle = (const bcircle &)(area);
			return bbox_intersects_circle(*this, circle);
		}
		case BAT_BOX_ORIENTED:
		{
			const bbox_oriented &box = (const bbox_oriented &)(area);
			return bbox_overlap(*this, box);
		}
		case BAT_BOX_ALIGNED:
		{
			bbox_aligned &box = (bbox_aligned &)(area);
			box.correct_corners();
			return bbox_overlap(*this, box);
		}
		case BAT_UNIFIED:
		{
			const bounding_area_unified *uni = (const bounding_area_unified*)&area;
			if (uni->inner_type == BAT_BOX_ORIENTED) {
				bbox_oriented bbox;
				bbox.copy(uni);
				return bbox_overlap(bbox, *this);
			} else if (uni->inner_type == BAT_CIRCLE) {
				bcircle circle;
				circle.copy(uni);
				return bbox_intersects_circle(*this, circle);
			}
		}
		default:
		{
			Assert(0);
			return false;
		}
	}
}

void bcircle::move(const vector2 &delta)
{
	center += delta;
}

void bcircle::copy(const bounding_area *src)
{
	Assert_return(src);
	if (src->get_type() == BAT_CIRCLE) {
		copy(*(const bcircle*)src);
	} else {
		Assert_return(src->get_type() == BAT_UNIFIED);
		const bounding_area_unified *uni = (const bounding_area_unified*)src;
		Assert_return(uni->inner_type == BAT_CIRCLE);
		center = uni->center;
		radius = uni->size.x;
		orient = uni->orient;
	}
}

int bcircle::collides(const line_segment &seg, vector2 *first_hit_out /* = NULL */, vector2 *second_hit_out /* = NULL */) const
{
	// todo - not yet implemented!
	return 0;
}

bool bcircle::collides(const bounding_area &area) const
{
	switch (area.get_type()) {
		case BAT_CIRCLE:
		{
			const bcircle &circle = (const bcircle &)(area);
			return circle_overlap(*this, circle);
		}
		case BAT_BOX_ORIENTED:
		{
			const bbox_oriented &box = (const bbox_oriented &)(area);
			return bbox_intersects_circle(box, *this);
		}
		case BAT_BOX_ALIGNED:
		{
			bbox_aligned &box = (bbox_aligned &)(area);
			box.correct_corners();
			return bbox_intersects_circle(box, *this);
		}
		case BAT_UNIFIED:
		{
			const bounding_area_unified *uni = (const bounding_area_unified*)&area;
			if (uni->inner_type == BAT_BOX_ORIENTED) {
				bbox_oriented bbox;
				bbox.copy(uni);
				return bbox_intersects_circle(bbox, *this);
			} else if (uni->inner_type == BAT_CIRCLE) {
				bcircle circle;
				circle.copy(uni);
				return circle_overlap(circle, *this);
			}
		}
		default:
		{
			Assert(0);
			return false;
		}
	}
}

void bbox_aligned::copy(const bbox_aligned &source)
{
	bbmin = source.bbmin;
	bbmax = source.bbmax;
	correct_corners();
}

// Make it so the min is less than the max.
void bbox_aligned::correct_corners()
{
	if (bbmin.x > bbmax.x) {
		SWAP(bbmin.x, bbmax.x, float);
	}

	if (bbmin.y > bbmax.y) {
		SWAP(bbmin.y, bbmax.y, float);
	}
}

void bbox_aligned::expand( vector2 pos )
{
	if (pos.x < bbmin.x) {
		bbmin.x = pos.x;
	}
	if (pos.x > bbmax.x) {
		bbmax.x = pos.x;
	}

	if (pos.y < bbmin.y) {
		bbmin.y = pos.y;
	}
	if (pos.y > bbmax.y) {
		bbmax.y = pos.y;
	}
}

void bbox_oriented::copy(const bbox_oriented &source)
{
	center = source.center;
	size = source.size;
	orient = source.orient;
}

void bbox_oriented::copy(const bbox_aligned &source)
{
	bbox_oriented converted;
	bbox_convert_to_oriented(source, converted);
	copy(converted);
}

void bcircle::copy(const bcircle &source)
{
	center = source.center;
	radius = source.radius;
	orient = source.orient;
}

bool line::y_intercept( float &intercept_out )
{
	if (dir.x == 0.0f) {
		return false;
	}

	// y-y0 = m(x - x0)
	// where x = 0
	// y = y0-mx0
	intercept_out =  start.y - ((dir.y / dir.x) * start.x);
	return true;
}

void line_segment::correct()
{
	if (a.x > b.x) {
		SWAP(a, b, vector2);
	} else if (a.x == b.x && a.y > b.y) {
		// if it's vertical, make sure b is higher than a
		SWAP(a, b, vector2);
	}
}

void bounding_area_unified::copy( const bounding_area *src )
{
	Assert_return(src);
	bbox_oriented bba;
	if (src->get_type() == BAT_BOX_ORIENTED) {
		bba.copy(src);
		src = &bba;
	}
	switch (src->get_type()) {
		case BAT_CIRCLE:
		{
			const bcircle *circ = (const bcircle*)src;
			center = circ->center;
			size.x = size.y = circ->radius;
			orient = circ->orient;
			inner_type = BAT_CIRCLE;
		}
		break;
		case BAT_BOX_ORIENTED:
		{
			const bbox_oriented *box = (const bbox_oriented*)src;
			center = box->center;
			size = box->size;
			orient = box->orient;
			inner_type = BAT_BOX_ORIENTED;
		}
		break;
		case BAT_UNIFIED:
		{
			const bounding_area_unified *uni = (const bounding_area_unified *)src;
			center = uni->center;
			size = uni->size;
			orient = uni->orient;
			inner_type = uni->inner_type;
		}
		break;
		default:
			Assert(!"Unhandled bounding area type in bounding_area_unified::copy");
			break;
	}
}

int bounding_area_unified::collides( const line_segment &seg, vector2 *first_hit_out /*= NULL*/, vector2 *second_hit_out /*= NULL*/ ) const
{
	bounding_area *new_area = NULL;
	bbox_oriented bbo;
	bcircle circ;
	switch (inner_type) {
		case BAT_BOX_ORIENTED:
			bbo.copy(this);
			new_area = &bbo;
			break;
		case BAT_CIRCLE:
			circ.copy(this);
			new_area = &circ;
			break;
		default:
			Assert(!"Unknown shape in bounding_area_unified::collides");
			return 0;
	}

	Assert_return_value(new_area, 0);
	return new_area->collides(seg, first_hit_out, second_hit_out);
}

bool bounding_area_unified::collides( const bounding_area &area ) const
{
	bounding_area *my_area = NULL;
	bcircle circ;
	bbox_oriented bbo;
	if (inner_type == BAT_CIRCLE) {
		circ.copy(this);
		my_area = &circ;
	} else if (inner_type == BAT_BOX_ORIENTED) {
		bbo.copy(this);
		my_area = &bbo;
	} else {
		Assert_return_value(!"Unhandled shape in bounding_area_unified::collides", false);
	}

	Assert_return_value(my_area, false);

	return my_area->collides(area);
}

void bounding_area_unified::scale( const vector2 & size_delta, float min_size /*= 1.0f*/ )
{
	size += size_delta;
	size.x = MAX(min_size, size.x);
	size.y = MAX(min_size, size.y);
	if (inner_type == BAT_CIRCLE) {
		size.y = size.x;
	}
}

