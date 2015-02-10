#include "vector.h"

#include "matrix.h"

// Default constructor.
//
vector2::vector2()
{
	x = 0.0f;
	y = 0.0f;
}

// Specific constructor.
//
vector2::vector2(float _x, float _y)
{
	x = _x;
	y = _y;
}

// Rotate the vector around the origin.
//
// theta: number of radians to rotate the position.
//
void vector2::rotate(const float theta)
{
	matrix rotation;
	rotation.rotate(theta);
	vector2 old_pos = *this;
	*this = rotation * old_pos;
}

// Normalize the vector, handling the zero-vector case.
//
// default_val: value to set the vector to if normalization is not possible.
//
void vector2::normalize_safe( const vector2 &default_val /*= RIGHT_VECTOR */ )
{
	const float min_val = 0.001f;
	if (fl_equals(x, 0.0f, min_val) && fl_equals(y, 0.0f, min_val)) {
		x = default_val.x;
		y = default_val.y;
	} else {
		normalize();
	}
}

