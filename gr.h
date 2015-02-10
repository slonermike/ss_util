#ifndef __GR_H
#define __GR_H

#pragma once

#define COLOR_GET_RED(c)	((c & 0xFF000000) >> 24)
#define COLOR_GET_GREEN(c)	((c & 0x00FF0000) >> 16)
#define COLOR_GET_BLUE(c)	((c & 0x0000FF00) >> 8)
#define COLOR_GET_ALPHA(c)	(c & 0x000000FF)

// (1.0/255.0)
#define INVERSE_255					(0.00392156863f)
#define COLOR_GET_RED_FL(c)			(COLOR_GET_RED(c) * INVERSE_255)
#define COLOR_GET_GREEN_FL(c)		(COLOR_GET_GREEN(c) * INVERSE_255)
#define COLOR_GET_BLUE_FL(c)		(COLOR_GET_BLUE(c) * INVERSE_255)
#define COLOR_GET_ALPHA_FL(c)		(COLOR_GET_ALPHA(c) * INVERSE_255)

#define COLOR(r,g,b,a)		((r << 24) | (g << 16) | (b << 8) | a)

int color_lerp(int from_color, int to_color, float amt);

#endif // __GR_H