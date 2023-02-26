/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2022-10-30
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_MATH_H_
#define _VULKAN_MATH_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport VulkanMath
    {
    public:
		static const float ms_fPI_Half;
        static const float ms_fPI;
        static const float ms_fPI_Two;
        static const float ms_fDeg2Rad;			
		static const float ms_fRad2Deg;			
		static const float ms_fLog2;	

        static const float ms_fPosInfinity;		
        static const float ms_fNegInfinity;

        static const glm::vec2 ms_v2Zero;
        static const glm::vec2 ms_v2One;
        static const glm::vec2 ms_v2UnitX;
        static const glm::vec2 ms_v2UnitY;
        static const glm::vec2 ms_v2UnitNegX;
        static const glm::vec2 ms_v2UnitNegY;

        static const glm::vec3 ms_v3Zero;
        static const glm::vec3 ms_v3One;
        static const glm::vec3 ms_v3Centi;
        static const glm::vec3 ms_v3UnitX;
        static const glm::vec3 ms_v3UnitY;
        static const glm::vec3 ms_v3UnitZ;
        static const glm::vec3 ms_v3UnitNegX;
        static const glm::vec3 ms_v3UnitNegY;
        static const glm::vec3 ms_v3UnitNegZ;

        static const glm::vec4 ms_v4Zero;
        static const glm::vec4 ms_v4One;
        static const glm::vec4 ms_v4UnitX;
        static const glm::vec4 ms_v4UnitY;
        static const glm::vec4 ms_v4UnitZ;
        static const glm::vec4 ms_v4UnitW;
        static const glm::vec4 ms_v4UnitNegX;
        static const glm::vec4 ms_v4UnitNegY;
        static const glm::vec4 ms_v4UnitNegZ;
        static const glm::vec4 ms_v4UnitNegW;

        static const glm::quat ms_qUnit;
        
        static const glm::mat3 ms_mat3Zero;
        static const glm::mat3 ms_mat3Unit;

        static const glm::mat4 ms_mat4Zero;
        static const glm::mat4 ms_mat4Unit;

        static const glm::vec4 ms_clBlack;
        static const glm::vec4 ms_clWhite;
        static const glm::vec4 ms_clRed;
        static const glm::vec4 ms_clGreen;
        static const glm::vec4 ms_clBlue;
        static const glm::vec4 ms_clGray;
        static const glm::vec4 ms_clDarkRed;
        static const glm::vec4 ms_clDarkGreen;
        static const glm::vec4 ms_clDarkBlue;
        static const glm::vec4 ms_clDarkGray;
        static const glm::vec4 ms_clYellow;
        static const glm::vec4 ms_clCyan;
        static const glm::vec4 ms_clMagenta;
        static const glm::vec4 ms_clTransparent;

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

        static float RandomMinMax(float fMin, float fMax)								
		{
			if (fMin == fMax) 
				return(fMin);
			float fRandom = (float)rand() / (float)RAND_MAX;
			return((fRandom * (float)fabs(fMax - fMin)) + fMin);
		}

        static glm::vec4 RandomColor(bool isAlpha)	
        {
            float r = VulkanMath::RandomMinMax(0, 1);
			float g = VulkanMath::RandomMinMax(0, 1);
			float b = VulkanMath::RandomMinMax(0, 1);
			float a = 1.0f;
            if (isAlpha)
                a = VulkanMath::RandomMinMax(0, 1);

			return glm::vec4(r, g, b, a);
        }

        static glm::vec4 RandomColor(glm::vec4 clMin, glm::vec4 clMax)	
		{
			float r = VulkanMath::RandomMinMax(clMin.r, clMax.r);
			float g = VulkanMath::RandomMinMax(clMin.g, clMax.g);
			float b = VulkanMath::RandomMinMax(clMin.b, clMax.b);
			float a = VulkanMath::RandomMinMax(clMin.a, clMax.a);

			return glm::vec4(r, g, b, a);
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

    ////Abs
        static int32 Abs(int32 iValue)											
        {
            return (iValue >= 0 ? iValue : -iValue); 
        }

        static float Abs(float fValue)	
        {
            return float(fabs(fValue)); 
        }

    ////Sqrt
        static float Sqrt(float fValue)
        {
            return float(sqrt(fValue));
        }

		static float Sqrt_Invert(float fValue)
        {
            return float(1.0f / sqrt(fValue));
        }
    
    ////Square
        static float Square(float fValue)
		{ 
			return fValue * fValue; 
		}

    ////Pow
        static float Pow(float fBase,float fExponent)
		{ 
			return float(pow(fBase, fExponent)); 
		}

		static uint32 Power2PlusOne(uint32 number);
		static bool IsPower2(size_t nValue, size_t& nNearestPow);

    ////Log
		static float Exp(float fValue)
		{ 
			return float(exp(fValue));
		}

		static float Log(float fValue)
		{ 
			return float(log(fValue)); 
		}

		static float Log2(float fValue)
		{ 
			return float(log(fValue) / ms_fLog2); 
		}

		static float LogN(float base,float fValue)
		{
			return float(log(fValue) / log(base)); 
		}
		
	////Ceil/Floor
		static int32	CeilI(float fValue)										
		{
			return int32(ceil(fValue));
		}

		static float CeilR(float fValue)
		{ 
			return float(ceil(fValue));
		}

		static int32	FloorI(float fValue)								
		{
			return int32(floor(fValue)); 
		}

		static float FloorR(float fValue)								
		{
			return float(floor(fValue)); 
		}
		
	////Sign
		static int32 Sign(int32 iValue)											
		{
			return (iValue > 0 ? +1 : (iValue < 0 ? -1 : 0));
		}
		
		static float Sign(float fValue)
		{
			return (fValue > 0.0f ? +1.0f : (fValue < 0.0f ? -1.0f : 0.0f));
		}

	////Saturate
        static float Saturate(float t) 
        { 
            return (t < 0) ? 0 : ((t > 1) ? 1 : t); 
        }
        static double Saturate(double t) 
        { 
            return (t < 0) ? 0 : ((t > 1) ? 1 : t); 
        }

	////Value
		static float Min(const glm::vec2& v) { return glm::min(v.x, v.y); }
        static float Min(const glm::vec3& v) { return glm::min(v.x, v.y, v.z); }
        static float Min(const glm::vec4& v) { return glm::min(v.x, v.y, v.z, v.w); }
        static glm::vec2 Min(const glm::vec2& v1, const glm::vec2& v2) 
        { 
            return glm::vec2(glm::min(v1.x, v2.x),
                             glm::min(v1.y, v2.y));
        } 
        static void Min(glm::vec2& vSrc, const glm::vec2& vIn) 
        { 
            vSrc.x = glm::min(vSrc.x, vIn.x);
            vSrc.y = glm::min(vSrc.y, vIn.y);
        } 
        static glm::vec3 Min(const glm::vec3& v1, const glm::vec3& v2) 
        { 
            return glm::vec3(glm::min(v1.x, v2.x),
                             glm::min(v1.y, v2.y),
                             glm::min(v1.z, v2.z));
        } 
        static void Min(glm::vec3& vSrc, const glm::vec3& vIn) 
        { 
            vSrc.x = glm::min(vSrc.x, vIn.x);
            vSrc.y = glm::min(vSrc.y, vIn.y);
            vSrc.z = glm::min(vSrc.z, vIn.z);
        }
        static glm::vec4 Min(const glm::vec4& v1, const glm::vec4& v2) 
        { 
            return glm::vec4(glm::min(v1.x, v2.x),
                             glm::min(v1.y, v2.y),
                             glm::min(v1.z, v2.z),
                             glm::min(v1.w, v2.w));
        } 
        static void Min(glm::vec4& vSrc, const glm::vec4& vIn) 
        { 
            vSrc.x = glm::min(vSrc.x, vIn.x);
            vSrc.y = glm::min(vSrc.y, vIn.y);
            vSrc.z = glm::min(vSrc.z, vIn.z);
            vSrc.w = glm::min(vSrc.w, vIn.w);
        } 

        static float Max(const glm::vec2& v) { return glm::max(v.x, v.y); }
        static float Max(const glm::vec3& v) { return glm::max(v.x, v.y, v.z); }
        static float Max(const glm::vec4& v) { return glm::max(v.x, v.y, v.z, v.w); }
        static glm::vec2 Max(const glm::vec2& v1, const glm::vec2& v2) 
        { 
            return glm::vec2(glm::max(v1.x, v2.x),
                             glm::max(v1.y, v2.y));
        } 
        static void Max(glm::vec2& vSrc, const glm::vec2& vIn) 
        { 
            vSrc.x = glm::max(vSrc.x, vIn.x);
            vSrc.y = glm::max(vSrc.y, vIn.y);
        } 
        static glm::vec3 Max(const glm::vec3& v1, const glm::vec3& v2) 
        { 
            return glm::vec3(glm::max(v1.x, v2.x),
                             glm::max(v1.y, v2.y),
                             glm::max(v1.z, v2.z));
        } 
        static void Max(glm::vec3& vSrc, const glm::vec3& vIn) 
        { 
            vSrc.x = glm::max(vSrc.x, vIn.x);
            vSrc.y = glm::max(vSrc.y, vIn.y);
            vSrc.z = glm::max(vSrc.z, vIn.z);
        } 
        static glm::vec4 Max(const glm::vec4& v1, const glm::vec4& v2) 
        { 
            return glm::vec4(glm::max(v1.x, v2.x),
                             glm::max(v1.y, v2.y),
                             glm::max(v1.z, v2.z),
                             glm::max(v1.w, v2.w));
        } 
        static void Max(glm::vec4& vSrc, const glm::vec4& vIn) 
        { 
            vSrc.x = glm::max(vSrc.x, vIn.x);
            vSrc.y = glm::max(vSrc.y, vIn.y);
            vSrc.z = glm::max(vSrc.z, vIn.z);
            vSrc.w = glm::max(vSrc.w, vIn.w);
        } 

        static bool IsGreat(const glm::vec3& v1, const glm::vec3& v2)
        {
            if (v1.x > v2.x && v1.y > v2.y && v1.z > v2.z)
                return true;
            return false;
        }    
		static bool IsEqual(float a, float b, float tolerance = std::numeric_limits<float>::epsilon())
        {
            return (fabs(b - a) <= tolerance) ? true : false; 
        }
        static bool IsEqual(const glm::vec3& v1, const glm::vec3& v2)
        {
            if(v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
                return true;
            return false;
        }
        static bool IsEqual(const glm::vec3& v1, const glm::vec3& v2, float tolerance)
        {
            if (VulkanMath::IsEqual(v1.x, v2.x, tolerance) &&
				VulkanMath::IsEqual(v1.y, v2.y, tolerance) &&
				VulkanMath::IsEqual(v1.z, v2.z, tolerance))
                return true;
            return false;
        }
        static bool IsLess(const glm::vec3& v1, const glm::vec3& v2)
        {
            if(v1.x < v2.x && v1.y < v2.y && v1.z < v2.z)
                return true;
            return false;
        }
        static bool IsZeroLength(const glm::vec3& v)
        {
            float fLen = (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
			return (fLen < (1e-06 * 1e-06));
        }

	public:
		static float Dot(const glm::vec3& v1, const glm::vec3& v2)
        {
            return glm::dot(v1, v2);
        }
        static float DotAbs(const glm::vec3& v1, const glm::vec3& v2)
        {
            return abs(glm::dot(v1, v2));
        }
        
        static float Dot(const glm::vec4& v1, const glm::vec4& v2)
        {
            return glm::dot(v1, v2);
        }
        static float DotAbs(const glm::vec4& v1, const glm::vec4& v2)
        {
            return abs(glm::dot(v1, v2));
        }

        static float Dot(const glm::quat& q1, const glm::quat& q2)
        {
            return glm::dot(q1, q2);
        }

        static glm::vec3 Cross(const glm::vec3& v1, const glm::vec3& v2)
        {
            return glm::cross(v1, v2);
        }

        static glm::vec3 Normalize(const glm::vec3& v)
        {
            return glm::normalize(v);
        }

        static glm::quat Normalize(const glm::quat& q)
        {
            float len = glm::length(q);
            return q / len;
        }

        static float Length(const glm::vec3& v)
        {
            return glm::length(v);
        }
        static float Length2(const glm::vec3& v)
        {
            return glm::length2(v);
        }

        static float Distance2(const glm::vec3& v1, const glm::vec3& v2)
        {
            return Length2(v1 - v2);
        }

        static void	MakeFloor(glm::vec3& src, const glm::vec3& cmp)
        {
            if (cmp.x < src.x) src.x = cmp.x;
            if (cmp.y < src.y) src.y = cmp.y;
            if (cmp.z < src.z) src.z = cmp.z;
        }
        static void	MakeCeil(glm::vec3& src, const glm::vec3& cmp)
        {
            if (cmp.x > src.x) src.x = cmp.x;
            if (cmp.y > src.y) src.y = cmp.y;
            if (cmp.z > src.z) src.z = cmp.z;
        }

        static glm::vec3 Reflect(const glm::vec3& vI, const glm::vec3& vN)
        {
            return glm::reflect(vI, vN);
        }

        static glm::vec3 Refract(const glm::vec3& vI, const glm::vec3& vN, float eta)
        {
            return glm::refract(vI, vN, eta);
        }

        static glm::vec3 Add(const glm::vec3& v1, const glm::vec3& v2)
        {
            return glm::vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
        }
        static glm::vec3 Sub(const glm::vec3& v1, const glm::vec3& v2)
        {
            return glm::vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
        }
        static glm::vec3 Half(const glm::vec3& v1)
        {
            return glm::vec3(v1.x / 2.0f, v1.y / 2.0f, v1.z / 2.0f);
        }
        static glm::vec3 HalfAdd(const glm::vec3& v1, const glm::vec3& v2)
        {
            return glm::vec3((v1.x + v2.x) / 2.0f, (v1.y + v2.y) / 2.0f, (v1.z + v2.z) / 2.0f);
        }
        static glm::vec3 HalfSub(const glm::vec3& v1, const glm::vec3& v2)
        {
            return glm::vec3((v1.x - v2.x) / 2.0f, (v1.y - v2.y) / 2.0f, (v1.z - v2.z) / 2.0f);
        }

	public:
		static glm::mat4 Translate(float x, float y, float z);
        static glm::mat4 Translate(const glm::vec3& vTranslate);
        static glm::mat4 Translate(const glm::mat4& mat4, float x, float y, float z);
        static glm::mat4 Translate(const glm::mat4& mat4, const glm::vec3& vTranslate);

        static glm::mat4 RotateX(float angleX);
        static glm::mat4 RotateY(float angleY);
        static glm::mat4 RotateZ(float angleZ);
        static glm::mat4 Rotate(float angleX, float angleY, float angleZ);
        static glm::mat4 Rotate(const glm::vec3& vAngle);

        static glm::mat4 Rotate(const glm::quat& qRot);

        static glm::mat4 Scale(float scaleX, float scaleY, float scaleZ);
        static glm::mat4 Scale(const glm::vec3& vScale);
        static void Scale(glm::mat3& mat3, const glm::vec3& vScale);

        static glm::mat4 FromTRS(const glm::vec3& vTranslate, const glm::vec3& vAngle, const glm::vec3& vScale);
        static glm::mat4 FromTQS(const glm::vec3& vTranslate, const glm::quat& qRot, const glm::vec3& vScale);

        static void FromMatrix3(glm::mat3& mat3, glm::vec3& xAxis, glm::vec3& yAxis, glm::vec3& zAxis);
        static glm::mat3 ToMatrix3(const glm::vec3& xAxis, const glm::vec3& yAxis, const glm::vec3& zAxis);
        static glm::mat3 ToMatrix3(const glm::quat& qRot);
        static glm::mat3 ToMatrix3(const glm::mat4& mat4);
        
        static void FromMatrix4(glm::mat4& mat4, glm::vec3& xAxis, glm::vec3& yAxis, glm::vec3& zAxis);
        static glm::mat4 ToMatrix4(const glm::vec3& xAxis, const glm::vec3& yAxis, const glm::vec3& zAxis);
        static glm::mat4 ToMatrix4(const glm::quat& qRot);
        static glm::mat4 ToMatrix4(const glm::vec3& vPos, const glm::vec3& vScale, const glm::quat& qRot);
        static glm::mat4 ToMatrix4(const glm::mat3& mat3);

        static glm::vec3 ToEulerAngles(const glm::quat& qRot);
        static glm::vec3 ToEulerAngles(const glm::vec3& vDir);
        static glm::vec3 ToDirection(const glm::vec3& vEulerAngles);
        
        static glm::quat ToQuaternion(const glm::mat3& mat3);
        static glm::quat ToQuaternion(const glm::mat4& mat4);
        static glm::quat ToQuaternion(const glm::vec3& vEulerAngles);
        static glm::quat ToQuaternion(const glm::vec3& xAxis, const glm::vec3& yAxis, const glm::vec3& zAxis);
        static glm::quat ToQuaternionFromAngleAxis(float fAngle, const glm::vec3& vAxis);
        static glm::quat ToQuaternionFromAngleAxis(const glm::quat& qSrc, float fAngle, const glm::vec3& vAxis);
        static glm::quat ToQuaternionFromRadianAxis(float fRadian, const glm::vec3& vAxis);
        static glm::quat ToQuaternionFromRadianAxis(const glm::quat& qSrc, float fRadian, const glm::vec3& vAxis);
        static glm::quat ToQuaternionFromSrc2Dst(const glm::vec3& vSrc, const glm::vec3& vDst, const glm::vec3& vFallbackAxis = VulkanMath::ms_v3Zero);

        static void ToAxes(const glm::quat& qRot, glm::vec3& xAxis, glm::vec3& yAxis, glm::vec3& zAxis);
        static void ToAngleAxis(const glm::quat& qRot, float& fAngle, glm::vec3& vAxis);

        static glm::vec3 Transform(const glm::mat4& mat4, const glm::vec3& vPos);
        static glm::vec3 Transform(const glm::quat& qRot, const glm::vec3& vPos);

        static bool IsAffine(const glm::mat4& mat4);
        static glm::vec3 TransformAffine(const glm::mat4& mat4, const glm::vec3& v);
		static glm::vec3 TransformAffine(const glm::mat4& mat4, const glm::vec4& v);	
        static glm::mat4 AffineInverse(const glm::mat4& mat4); 

        static glm::quat InverseQuaternion(const glm::quat& qRot);
        static glm::mat3 InverseMatrix3(const glm::mat3& mat3);
        static glm::mat4 InverseMatrix4(const glm::mat4& mat4);

        static glm::mat3 TransposeMatrix3(const glm::mat3& mat3);
        static glm::mat4 TransposeMatrix4(const glm::mat4& mat4);
    };

}; //LostPeter

#endif