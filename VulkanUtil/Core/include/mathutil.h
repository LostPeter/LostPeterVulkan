// author: LostPeter
// time:   2022-10-30

#ifndef _MATH_UTIL_H_
#define _MATH_UTIL_H_

#include "predefine.h"

namespace LibUtil
{
    class utilExport MathUtil
    {
    public:
		static const float Infinity;
		static const float Pi;

	public:
		static float RandF()
		{
			return (float)(rand()) / (float)RAND_MAX;
		}

		static float RandF(float a, float b)
		{
			return a + RandF() * (b - a);
		}

		static int Rand(int a, int b)
		{
			return a + rand() % ((b - a) + 1);
		}

		template<typename T>
		static T Min(const T& a, const T& b)
		{
			return a < b ? a : b;
		}

		template<typename T>
		static T Max(const T& a, const T& b)
		{
			return a > b ? a : b;
		}

		template<typename T>
		static T Lerp(const T& a, const T& b, float t)
		{
			return a + (b - a) * t;
		}

		template<typename T>
		static T Clamp(const T& x, const T& low, const T& high)
		{
			return x < low ? low : (x > high ? high : x);
		}

		static float AngleFromXY(float x, float y);

		static glm::vec3 SphericalToCartesian(float radius, float theta, float phi)
		{
			return glm::vec3(radius * sinf(phi) * cosf(theta),
				             radius * cosf(phi),
				             radius * sinf(phi) * sinf(theta));
		}

		static glm::mat4 Identity4x4()
		{
			static glm::mat4 I(1.0f);

			return I;
		}
    };

}; //LibUtil

#endif