#include "ss_math.h"

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
