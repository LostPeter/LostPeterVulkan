#include "../include/preinclude.h"
#include "../include/mathutil.h"

namespace LibUtil
{
    const float MathUtil::Infinity = FLT_MAX;
	const float MathUtil::Pi = 3.1415926535f;

	float MathUtil::AngleFromXY(float x, float y)
	{
		float theta = 0.0f;

		// Quadrant I or IV
		if (x >= 0.0f)
		{
			// If x = 0, then atanf(y/x) = +pi/2 if y > 0
			//                atanf(y/x) = -pi/2 if y < 0
			theta = atanf(y / x); // in [-pi/2, +pi/2]

			if (theta < 0.0f)
				theta += 2.0f * Pi; // in [0, 2*pi).
		}

		// Quadrant II or III
		else
			theta = atanf(y / x) + Pi; // in [0, 2*pi).

		return theta;
	}

}; //LibUtil