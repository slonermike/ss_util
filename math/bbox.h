#ifndef __BBOX_H
#define __BBOX_H

#pragma once

#include "matrix.h"
#include "vector.h"

enum bounding_area_type {
	BAT_INVALID = -1,

	BAT_BOX_ALIGNED,
	BAT_BOX_ORIENTED,
	BAT_CIRCLE,
	BAT_UNIFIED,

	BAT_NUM_TYPES,
};

struct line {
	line() : start(ZERO_VECTOR), dir(UP_VECTOR){};
	line(vector2 pt, vector2 fvec) : start(pt), dir(fvec) {}
	bool y_intercept(float &intercept_out);
	vector2 start;
	vector2 dir;
};

struct line_segment {
	line_segment() : a(ZERO_VECTOR), b(RIGHT_VECTOR){};
	line_segment(vector2 start, vector2 end) : a(start), b(end) {}
	void correct();
	vector2 a;
	vector2 b;
};

struct bounding_area {
	virtual bool collides(const bounding_area &area) const = 0;
	virtual bounding_area_type get_type() const = 0;
	virtual vector2 get_center() const = 0;
	virtual int collides(const line_segment &seg, vector2 *first_hit_out = NULL, vector2 *second_hit_out = NULL) const = 0;
	virtual void move(const vector2 &delta) = 0;
	virtual void copy(const bounding_area *src) = 0;
	virtual void get_orient(matrix &orient_out) const = 0;
};

struct bbox_aligned : public bounding_area {
	
	bbox_aligned()
		: bounding_area(), bbmin(vector2()), bbmax(vector2()) {}
	
	bbox_aligned(const vector2 &min, const vector2 &max)
		: bounding_area(), bbmin(min), bbmax(max) {correct_corners();}

	vector2 bbmin;
	vector2 bbmax;

	virtual bool collides(const bounding_area &area) const;
	virtual int collides(const line_segment &seg, vector2 *first_hit_out = NULL, vector2 *second_hit_out = NULL) const;
	virtual bounding_area_type get_type() const {return BAT_BOX_ALIGNED;}
	virtual vector2 get_center() const {return (bbmin + bbmax) * 0.5f;}
	virtual void move(const vector2 &delta);
	virtual void copy(const bounding_area *src);
	virtual void get_orient(matrix &orient_out) const {orient_out = IDENTITY_MATRIX;}
	void copy(const bbox_aligned &source);
	void correct_corners();
	void expand( vector2 pos );
};

struct bbox_oriented : public bounding_area {

	bbox_oriented *prev, *next;

	bbox_oriented()
		: bounding_area(), center(vector2()), size(vector2()), orient(IDENTITY_MATRIX), prev(NULL), next(NULL) {}

	bbox_oriented(const vector2 &_center, const vector2 &_size, const matrix &_orient)
		: bounding_area(), center(_center), size(_size), orient(_orient), prev(NULL), next(NULL) {}

	vector2 center;
	vector2 size;
	matrix orient;

	virtual bool collides(const bounding_area &area) const;
	virtual int collides(const line_segment &seg, vector2 *first_hit_out = NULL, vector2 *second_hit_out = NULL) const;
	virtual bounding_area_type get_type() const {return BAT_BOX_ORIENTED;}
	virtual vector2 get_center() const {return center;}
	virtual void move(const vector2 &delta);
	virtual void copy(const bounding_area *src);
	virtual void get_orient(matrix &orient_out) const {orient_out = orient;}

	void copy(const bbox_aligned &source);
	void copy(const bbox_oriented &source);
};

struct bcircle : public bounding_area {

	bcircle *prev, *next;

	bcircle()
		: bounding_area(), center(vector2()), radius(0.0f), prev(NULL), next(NULL), orient(IDENTITY_MATRIX) {}
	
	bcircle(const vector2 &_center, const float _radius, const matrix &_orient = IDENTITY_MATRIX)
		: bounding_area(), center(_center), radius(_radius), orient(_orient), prev(NULL), next(NULL) {}

	vector2 center;
	float radius;
	matrix orient;

	virtual bool collides(const bounding_area &area) const;
	virtual int collides(const line_segment &seg, vector2 *first_hit_out = NULL, vector2 *second_hit_out = NULL) const;
	virtual bounding_area_type get_type() const {return BAT_CIRCLE;}
	virtual vector2 get_center() const {return center;}
	virtual void move(const vector2 &delta);
	virtual void copy(const bounding_area *src);
	virtual void get_orient(matrix &orient_out) const {orient_out = orient;}

	void copy(const bcircle &source);
};

// Unified bounding area type that can be used to theoretically describe any shape.
//
struct bounding_area_unified : public bounding_area {
	bounding_area_unified() : prev(NULL), next(NULL), inner_type(BAT_CIRCLE), center(ZERO_VECTOR), size(ZERO_VECTOR), orient(IDENTITY_MATRIX) {}
	bounding_area_unified *prev, *next;

	bounding_area_type inner_type;
	vector2 center;
	vector2 size;
	matrix orient;

	virtual bool collides(const bounding_area &area) const;
	virtual int collides(const line_segment &seg, vector2 *first_hit_out = NULL, vector2 *second_hit_out = NULL) const;
	virtual bounding_area_type get_type() const {return BAT_UNIFIED;}
	virtual vector2 get_center() const {return center;}
	virtual void move(const vector2 &delta) {center += delta;}
	virtual void copy(const bounding_area *src);
	virtual void get_orient(matrix &orient_out) const {orient_out = orient;}
	void scale( const vector2 & size_delta, float min_size = 1.0f );
};

bool bbox_intersects_point(const bbox_aligned &bbox, const vector2 &pt);
bool bbox_intersects_circle(const bbox_aligned &bbox, const bcircle &circle);
bool bbox_intersects_circle(const bbox_oriented &bbox, const bcircle &circle);
bool ray_intersects_segment(const line &_ray, const line_segment &_seg, vector2 *ix_point_out = NULL);
bool segment_intersects_segment(const line_segment &_a, const line_segment &_b, vector2 *ix_point_out = NULL);
bool line_intersects_line(const line &_a, const line &_b, vector2 *ix_point_out = NULL);
bool lines_coincide(const line &_a, const line &b);
bool bbox_intersects_ray(const bbox_oriented &bbox, const line &ray);
bool bbox_overlap(const bbox_aligned &bbox1, const bbox_aligned &bbox2);
bool bbox_overlap(const bbox_oriented &bbox1, const bbox_oriented &bbox2);
bool bbox_overlap(const bbox_oriented &o_box, const bbox_aligned &a_box);
bool circle_overlap(const bcircle &circle1, const bcircle &circle2);

void bbox_convert_to_oriented(const bbox_aligned &source, bbox_oriented &dest);

#endif // __BBOX_H