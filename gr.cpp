#include "gr.h"

#include "math/ss_math.h"

int color_lerp( int from_color, int to_color, float amt )
{
	int r1 = COLOR_GET_RED(from_color);
	int g1 = COLOR_GET_GREEN(from_color);
	int b1 = COLOR_GET_BLUE(from_color);
	int a1 = COLOR_GET_ALPHA(from_color);

	int r2 = COLOR_GET_RED(to_color);
	int g2 = COLOR_GET_GREEN(to_color);
	int b2 = COLOR_GET_BLUE(to_color);
	int a2 = COLOR_GET_ALPHA(to_color);

	int r = fl2i(LERP(r1, r2, amt));
	int g = fl2i(LERP(g1, g2, amt));
	int b = fl2i(LERP(b1, b2, amt));
	int a = fl2i(LERP(a1, a2, amt));

	int new_color = COLOR(r, g, b, a);
	return new_color;
}