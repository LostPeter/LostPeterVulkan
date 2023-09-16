/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-01
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_MATH_H_
#define _F_MATH_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class LPF_Export FMath
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
        static const float ms_fEpsilon;
        static const float ms_fRayFar;

        static const FVector2 ms_v2Zero;
        static const FVector2 ms_v2One;
        static const FVector2 ms_v2UnitX;
        static const FVector2 ms_v2UnitY;
        static const FVector2 ms_v2UnitNegX;
        static const FVector2 ms_v2UnitNegY;

        static const FVector3 ms_v3Zero;
        static const FVector3 ms_v3One;
        static const FVector3 ms_v3Centi;
        static const FVector3 ms_v3UnitX;
        static const FVector3 ms_v3UnitY;
        static const FVector3 ms_v3UnitZ;
        static const FVector3 ms_v3UnitNegX;
        static const FVector3 ms_v3UnitNegY;
        static const FVector3 ms_v3UnitNegZ;

        static const FVector4 ms_v4Zero;
        static const FVector4 ms_v4One;
        static const FVector4 ms_v4UnitX;
        static const FVector4 ms_v4UnitY;
        static const FVector4 ms_v4UnitZ;
        static const FVector4 ms_v4UnitW;
        static const FVector4 ms_v4UnitNegX;
        static const FVector4 ms_v4UnitNegY;
        static const FVector4 ms_v4UnitNegZ;
        static const FVector4 ms_v4UnitNegW;

        static const FQuaternion ms_qUnit;
        
        static const FMatrix3 ms_mat3Zero;
        static const FMatrix3 ms_mat3Unit;

        static const FMatrix4 ms_mat4Zero;
        static const FMatrix4 ms_mat4Unit;

        static const FVector4 ms_clBlack;
        static const FVector4 ms_clWhite;
        static const FVector4 ms_clRed;
        static const FVector4 ms_clGreen;
        static const FVector4 ms_clBlue;
        static const FVector4 ms_clGray;
        static const FVector4 ms_clDarkRed;
        static const FVector4 ms_clDarkGreen;
        static const FVector4 ms_clDarkBlue;
        static const FVector4 ms_clDarkGray;
        static const FVector4 ms_clYellow;
        static const FVector4 ms_clCyan;
        static const FVector4 ms_clMagenta;
        static const FVector4 ms_clTransparent;

	public:
        //Rand
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

        static FVector4 RandomColor(bool isAlpha)	
        {
            float r = FMath::RandomMinMax(0, 1);
			float g = FMath::RandomMinMax(0, 1);
			float b = FMath::RandomMinMax(0, 1);
			float a = 1.0f;
            if (isAlpha)
                a = FMath::RandomMinMax(0, 1);

			return FVector4(r, g, b, a);
        }

        static FVector4 RandomColor(FVector4 clMin, FVector4 clMax)	
		{
			float r = FMath::RandomMinMax(clMin.r, clMax.r);
			float g = FMath::RandomMinMax(clMin.g, clMax.g);
			float b = FMath::RandomMinMax(clMin.b, clMax.b);
			float a = FMath::RandomMinMax(clMin.a, clMax.a);

			return FVector4(r, g, b, a);
		}

        //Min
		template<typename T>
		static T Min(const T& a, const T& b)
		{
			return a < b ? a : b;
		}

        //Max
		template<typename T>
		static T Max(const T& a, const T& b)
		{
			return a > b ? a : b;
		}

        //Lerp
		template<typename T>
		static T Lerp(const T& a, const T& b, float t)
		{
			return a + (b - a) * t;
		}

        //Clamp
		template<typename T>
		static T Clamp(const T& x, const T& low, const T& high)
		{
			return x < low ? low : (x > high ? high : x);
		}

		static float AngleFromXY(float x, float y);

		static FVector3 SphericalToCartesian(float radius, float theta, float phi)
		{
			return FVector3(radius * sinf(phi) * cosf(theta),
				            radius * cosf(phi),
				            radius * sinf(phi) * sinf(theta));
		}

		static FMatrix4 Identity4x4()
		{
			static FMatrix4 I(1.0f);
			return I;
		}

        //Abs
        static int32 Abs(int32 iValue)											
        {
            return (iValue >= 0 ? iValue : -iValue); 
        }
        static float Abs(float fValue)	
        {
            return float(fabs(fValue)); 
        }
        static FVector2 Abs(const FVector2& v2)
        {
            return FVector2(fabs(v2.x), fabs(v2.y));
        }
        static FVector3 Abs(const FVector3& v3)
        {
            return FVector3(fabs(v3.x), fabs(v3.y), fabs(v3.z));
        }
        static FVector4 Abs(const FVector4& v4)
        {
            return FVector4(fabs(v4.x), fabs(v4.y), fabs(v4.z), fabs(v4.w));
        }


        //Sqrt
        static float Sqrt(float fValue)
        {
            return float(sqrt(fValue));
        }

		static float Sqrt_Invert(float fValue)
        {
            return float(1.0f / sqrt(fValue));
        }
    
        //Square
        static float Square(float fValue)
		{ 
			return fValue * fValue; 
		}

        //Pow
        static float Pow(float fBase,float fExponent)
		{ 
			return float(pow(fBase, fExponent)); 
		}

		static uint32 Power2PlusOne(uint32 number);
		static bool IsPower2(size_t nValue, size_t& nNearestPow);

        //Exp
		static float Exp(float fValue)
		{ 
			return float(exp(fValue));
		}

        //Log
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
		
	    //Ceil/Floor
		static int32 CeilI(float fValue)										
		{
			return int32(ceil(fValue));
		}
		static float CeilR(float fValue)
		{ 
			return float(ceil(fValue));
		}
		static int32 FloorI(float fValue)								
		{
			return int32(floor(fValue)); 
		}
		static float FloorR(float fValue)								
		{
			return float(floor(fValue)); 
		}
		
	    //Sign
		static int32 Sign(int32 iValue)											
		{
			return (iValue > 0 ? +1 : (iValue < 0 ? -1 : 0));
		}
		static float Sign(float fValue)
		{
			return (fValue > 0.0f ? +1.0f : (fValue < 0.0f ? -1.0f : 0.0f));
		}

	    //Saturate
        static float Saturate(float t) 
        { 
            return (t < 0) ? 0 : ((t > 1) ? 1 : t); 
        }
        static double Saturate(double t) 
        { 
            return (t < 0) ? 0 : ((t > 1) ? 1 : t); 
        }

	    //Min
		static float Min(const FVector2& v) 
        { 
            return glm::min(v.x, v.y); 
        }
        static float Min(const FVector3& v) 
        { 
            return glm::min(v.x, v.y, v.z); 
        }
        static float Min(const FVector4& v) 
        { 
            return glm::min(v.x, v.y, v.z, v.w); 
        }
        static FVector2 Min(const FVector2& v1, const FVector2& v2) 
        { 
            return FVector2(glm::min(v1.x, v2.x),
                            glm::min(v1.y, v2.y));
        } 
        static void Min(FVector2& vSrc, const FVector2& vIn) 
        { 
            vSrc.x = glm::min(vSrc.x, vIn.x);
            vSrc.y = glm::min(vSrc.y, vIn.y);
        } 
        static FVector3 Min(const FVector3& v1, const FVector3& v2) 
        { 
            return FVector3(glm::min(v1.x, v2.x),
                            glm::min(v1.y, v2.y),
                            glm::min(v1.z, v2.z));
        } 
        static void Min(FVector3& vSrc, const FVector3& vIn) 
        { 
            vSrc.x = glm::min(vSrc.x, vIn.x);
            vSrc.y = glm::min(vSrc.y, vIn.y);
            vSrc.z = glm::min(vSrc.z, vIn.z);
        }
        static FVector4 Min(const FVector4& v1, const FVector4& v2) 
        { 
            return FVector4(glm::min(v1.x, v2.x),
                            glm::min(v1.y, v2.y),
                            glm::min(v1.z, v2.z),
                            glm::min(v1.w, v2.w));
        } 
        static void Min(FVector4& vSrc, const FVector4& vIn) 
        { 
            vSrc.x = glm::min(vSrc.x, vIn.x);
            vSrc.y = glm::min(vSrc.y, vIn.y);
            vSrc.z = glm::min(vSrc.z, vIn.z);
            vSrc.w = glm::min(vSrc.w, vIn.w);
        } 

        //Max
        static float Max(const FVector2& v) 
        { 
            return glm::max(v.x, v.y); 
        }
        static float Max(const FVector3& v) 
        { 
            return glm::max(v.x, v.y, v.z); 
        }
        static float Max(const FVector4& v) 
        { 
            return glm::max(v.x, v.y, v.z, v.w); 
        }
        static FVector2 Max(const FVector2& v1, const FVector2& v2) 
        { 
            return FVector2(glm::max(v1.x, v2.x),
                            glm::max(v1.y, v2.y));
        } 
        static void Max(FVector2& vSrc, const FVector2& vIn) 
        { 
            vSrc.x = glm::max(vSrc.x, vIn.x);
            vSrc.y = glm::max(vSrc.y, vIn.y);
        } 
        static FVector3 Max(const FVector3& v1, const FVector3& v2) 
        { 
            return FVector3(glm::max(v1.x, v2.x),
                            glm::max(v1.y, v2.y),
                            glm::max(v1.z, v2.z));
        } 
        static void Max(FVector3& vSrc, const FVector3& vIn) 
        { 
            vSrc.x = glm::max(vSrc.x, vIn.x);
            vSrc.y = glm::max(vSrc.y, vIn.y);
            vSrc.z = glm::max(vSrc.z, vIn.z);
        } 
        static FVector4 Max(const FVector4& v1, const FVector4& v2) 
        { 
            return FVector4(glm::max(v1.x, v2.x),
                            glm::max(v1.y, v2.y),
                            glm::max(v1.z, v2.z),
                            glm::max(v1.w, v2.w));
        } 
        static void Max(FVector4& vSrc, const FVector4& vIn) 
        { 
            vSrc.x = glm::max(vSrc.x, vIn.x);
            vSrc.y = glm::max(vSrc.y, vIn.y);
            vSrc.z = glm::max(vSrc.z, vIn.z);
            vSrc.w = glm::max(vSrc.w, vIn.w);
        } 

        //Compare
        static bool IsGreat(const FVector3& v1, const FVector3& v2)
        {
            if (v1.x > v2.x && v1.y > v2.y && v1.z > v2.z)
                return true;
            return false;
        }    
		static bool IsEqual(float a, float b, float fEpsilon = FMath::ms_fEpsilon)
        {
            return (fabs(b - a) <= fEpsilon) ? true : false; 
        }
        static bool IsEqual(const FVector3& v1, const FVector3& v2)
        {
            if(v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
                return true;
            return false;
        }
        static bool IsEqual(const FVector3& v1, const FVector3& v2, float fEpsilon)
        {
            if (FMath::IsEqual(v1.x, v2.x, fEpsilon) &&
				FMath::IsEqual(v1.y, v2.y, fEpsilon) &&
				FMath::IsEqual(v1.z, v2.z, fEpsilon))
                return true;
            return false;
        }
        static bool IsLess(const FVector3& v1, const FVector3& v2)
        {
            if(v1.x < v2.x && v1.y < v2.y && v1.z < v2.z)
                return true;
            return false;
        }
        static bool IsZeroLength(const FVector3& v)
        {
            float fLen = (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
			return (fLen < (ms_fEpsilon * ms_fEpsilon));
        }

	public:
        //Zero
        static bool Zero(float f, float fEpsilon /*= FMath::ms_fEpsilon*/)
        {
            if (Abs(f) < fEpsilon)
                return true;
            return false;
        }
        static bool Zero(const FVector2& v2)
        {
            if (Abs(v2.x) < ms_fEpsilon &&
                Abs(v2.y) < ms_fEpsilon)
                return true;
            return false;
        }
        static bool Zero(const FVector3& v3)
        {
            if (Abs(v3.x) < ms_fEpsilon &&
                Abs(v3.y) < ms_fEpsilon &&
                Abs(v3.z) < ms_fEpsilon)
                return true;
            return false;
        }
        static bool Zero(const FVector4& v4)
        {
            if (Abs(v4.x) < ms_fEpsilon &&
                Abs(v4.y) < ms_fEpsilon &&
                Abs(v4.z) < ms_fEpsilon &&
                Abs(v4.w) < ms_fEpsilon)
                return true;
            return false;
        }

        //Dot
        static float Dot(const FVector2& v1, const FVector2& v2)
        {
            return glm::dot(v1, v2);
        }
        static float DotAbs(const FVector2& v1, const FVector2& v2)
        {
            return abs(glm::dot(v1, v2));
        }

		static float Dot(const FVector3& v1, const FVector3& v2)
        {
            return glm::dot(v1, v2);
        }
        static float DotAbs(const FVector3& v1, const FVector3& v2)
        {
            return abs(glm::dot(v1, v2));
        }
        
        static float Dot(const FVector4& v1, const FVector4& v2)
        {
            return glm::dot(v1, v2);
        }
        static float DotAbs(const FVector4& v1, const FVector4& v2)
        {
            return abs(glm::dot(v1, v2));
        }

        static float Dot(const FQuaternion& q1, const FQuaternion& q2)
        {
            return glm::dot(q1, q2);
        }

        //Cross
        static FVector3 Cross(const FVector3& v1, const FVector3& v2)
        {
            return glm::cross(v1, v2);
        }

        //Normalize
        static FVector3 Normalize(const FVector3& v)
        {
            return glm::normalize(v);
        }

        static FQuaternion Normalize(const FQuaternion& q)
        {
            float len = glm::length(q);
            return q / len;
        }

        //Length
        static float Length(const FVector2& v)
        {
            return glm::length(v);
        }
        static float Length2(const FVector2& v)
        {
            return glm::length2(v);
        }

        static float Length(const FVector3& v)
        {
            return glm::length(v);
        }
        static float Length2(const FVector3& v)
        {
            return glm::length2(v);
        }

        //Distance
        static float Distance(const FVector2& v1, const FVector2& v2)
        {
            return Length(v1 - v2);
        }
        static float Distance2(const FVector2& v1, const FVector2& v2)
        {
            return Length2(v1 - v2);
        }

        static float Distance(const FVector3& v1, const FVector3& v2)
        {
            return Length(v1 - v2);
        }
        static float Distance2(const FVector3& v1, const FVector3& v2)
        {
            return Length2(v1 - v2);
        }

        //Floor - Ceil
        static void	MakeFloor(FVector3& src, const FVector3& cmp)
        {
            if (cmp.x < src.x) src.x = cmp.x;
            if (cmp.y < src.y) src.y = cmp.y;
            if (cmp.z < src.z) src.z = cmp.z;
        }
        static void	MakeCeil(FVector3& src, const FVector3& cmp)
        {
            if (cmp.x > src.x) src.x = cmp.x;
            if (cmp.y > src.y) src.y = cmp.y;
            if (cmp.z > src.z) src.z = cmp.z;
        }

        //Reflect
        static FVector3 Reflect(const FVector3& vI, const FVector3& vN)
        {
            return glm::reflect(vI, vN);
        }

        //Refract
        static FVector3 Refract(const FVector3& vI, const FVector3& vN, float eta)
        {
            return glm::refract(vI, vN, eta);
        }

        //Add - Sub - Half
        static FVector3 Add(const FVector3& v1, const FVector3& v2)
        {
            return FVector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
        }
        static FVector3 Sub(const FVector3& v1, const FVector3& v2)
        {
            return FVector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
        }
        static FVector3 Half(const FVector3& v1)
        {
            return FVector3(v1.x / 2.0f, v1.y / 2.0f, v1.z / 2.0f);
        }
        static FVector3 HalfAdd(const FVector3& v1, const FVector3& v2)
        {
            return FVector3((v1.x + v2.x) / 2.0f, (v1.y + v2.y) / 2.0f, (v1.z + v2.z) / 2.0f);
        }
        static FVector3 HalfSub(const FVector3& v1, const FVector3& v2)
        {
            return FVector3((v1.x - v2.x) / 2.0f, (v1.y - v2.y) / 2.0f, (v1.z - v2.z) / 2.0f);
        }

	public:
		static FMatrix4 Translate(float x, float y, float z);
        static FMatrix4 Translate(const FVector3& vTranslate);
        static FMatrix4 Translate(const FMatrix4& mat4, float x, float y, float z);
        static FMatrix4 Translate(const FMatrix4& mat4, const FVector3& vTranslate);

        static FMatrix4 RotateX(float angleX);
        static FMatrix4 RotateY(float angleY);
        static FMatrix4 RotateZ(float angleZ);
        static FMatrix4 Rotate(float angleX, float angleY, float angleZ);
        static FMatrix4 Rotate(const FVector3& vAngle);

        static FMatrix4 Rotate(const FQuaternion& qRot);

        static FMatrix4 Scale(float scaleX, float scaleY, float scaleZ);
        static FMatrix4 Scale(const FVector3& vScale);
        static void Scale(FMatrix3& mat3, const FVector3& vScale);

        static FMatrix4 FromTRS(const FVector3& vTranslate, const FVector3& vAngle, const FVector3& vScale);
        static FMatrix4 FromTQS(const FVector3& vTranslate, const FQuaternion& qRot, const FVector3& vScale);

        static void FromMatrix3(FMatrix3& mat3, FVector3& xAxis, FVector3& yAxis, FVector3& zAxis);
        static FMatrix3 ToMatrix3(const FVector3& xAxis, const FVector3& yAxis, const FVector3& zAxis);
        static FMatrix3 ToMatrix3(const FQuaternion& qRot);
        static FMatrix3 ToMatrix3(const FMatrix4& mat4);
        
        static void FromMatrix4(FMatrix4& mat4, FVector3& xAxis, FVector3& yAxis, FVector3& zAxis);
        static FMatrix4 ToMatrix4(const FVector3& xAxis, const FVector3& yAxis, const FVector3& zAxis);
        static FMatrix4 ToMatrix4(const FQuaternion& qRot);
        static FMatrix4 ToMatrix4(const FVector3& vPos, const FVector3& vScale, const FQuaternion& qRot);
        static FMatrix4 ToMatrix4(const FMatrix3& mat3);

        static FVector3 ToEulerAngles(const FQuaternion& qRot);
        static FVector3 ToEulerAngles(const FVector3& vDir);
        static FVector3 ToDirection(const FVector3& vEulerAngles);
        
        static FQuaternion ToQuaternion(const FMatrix3& mat3);
        static FQuaternion ToQuaternion(const FMatrix4& mat4);
        static FQuaternion ToQuaternion(const FVector3& vEulerAngles);
        static FQuaternion ToQuaternion(const FVector3& xAxis, const FVector3& yAxis, const FVector3& zAxis);
        static FQuaternion ToQuaternionFromAngleAxis(float fAngle, const FVector3& vAxis);
        static FQuaternion ToQuaternionFromAngleAxis(const FQuaternion& qSrc, float fAngle, const FVector3& vAxis);
        static FQuaternion ToQuaternionFromRadianAxis(float fRadian, const FVector3& vAxis);
        static FQuaternion ToQuaternionFromRadianAxis(const FQuaternion& qSrc, float fRadian, const FVector3& vAxis);
        static FQuaternion ToQuaternionFromSrc2Dst(const FVector3& vSrc, const FVector3& vDst, const FVector3& vFallbackAxis = FMath::ms_v3Zero);

        static void ToAxes(const FQuaternion& qRot, FVector3& xAxis, FVector3& yAxis, FVector3& zAxis);
        static void ToAngleAxis(const FQuaternion& qRot, float& fAngle, FVector3& vAxis);

        static FVector3 Transform(const FMatrix4& mat4, const FVector3& vPos);
        static FVector3 Transform(const FQuaternion& qRot, const FVector3& vPos);

        static bool TransformPerspective(const FMatrix4& mat4, const FVector3& vPosIn, FVector3& vPosOut);

        static bool IsAffine(const FMatrix4& mat4);
        static FVector3 TransformAffine(const FMatrix4& mat4, const FVector3& v);
		static FVector3 TransformAffine(const FMatrix4& mat4, const FVector4& v);	
        static FMatrix4 AffineInverse(const FMatrix4& mat4); 

        static FQuaternion InverseQuaternion(const FQuaternion& qRot);
        static FMatrix3 InverseMatrix3(const FMatrix3& mat3);
        static FMatrix4 InverseMatrix4(const FMatrix4& mat4);

        static FMatrix3 TransposeMatrix3(const FMatrix3& mat3);
        static FMatrix4 TransposeMatrix4(const FMatrix4& mat4);

    public:
        //Direction From Point2
        static FVector3 GetDirectionWithoutNormalizeFromPoint2(const FVector3& v1, const FVector3& v2);
        static FVector3 GetDirectionFromPoint2(const FVector3& v1, const FVector3& v2);

        //Normal From Point3
        static FVector3 GetNormal3WithoutNormalizeFromPoints3(const FVector3& v1, const FVector3& v2, const FVector3& v3);
        static FVector4 GetNormal4WithoutNormalizeFromPoints3(const FVector3& v1, const FVector3& v2, const FVector3& v3);
        static FVector3 GetNormal3FromPoints3(const FVector3& v1, const FVector3& v2, const FVector3& v3);
        static FVector4 GetNormal4FromPoints3(const FVector3& v1, const FVector3& v2, const FVector3& v3);

        //Angle Cos From Direction2/Segment2/Plane2
        static float GetAngleCosFromDirection2(const FVector3& v1, const FVector3& v2);
        static float GetAngleCosFromSegment2(const FSegment& s1, const FSegment& s2);
        static float GetAngleCosFromPlane2(const FVector3& p11, const FVector3& p12, const FVector3& p13, const FVector3& p21, const FVector3& p22, const FVector3& p23);

        //Angle Sin From Line-Plane
        static float GetAngleSinFromLinePlane(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3);
        static float GetAngleSinFromLinePlane(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3);

        //Distance From Point-Line
        static float GetDistanceFromPointLine(const FVector3& pt, const FVector3& ptLine11, const FVector3& ptLine12);
        static float GetDistanceFromPointLine(const FVector3& pt, const FSegment& segment);

        //Distance From Point-Plane
        static float GetDistanceFromPointPlane(const FVector3& pt, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3);

        //Distance From Line2
        static float GetDistanceFromLine2(const FVector3& ptLine11, const FVector3& ptLine12, const FVector3& ptLine21, const FVector3& ptLine22);
        static float GetDistanceFromLine2(const FSegment& segment1, const FSegment& segment2);

        //Distance From Ray-AABB-Axis
        static bool GetDistanceFromRayAABBAxis(const FVector3& rayOrig, const FVector3& rayDir, const FVector3& min, const FVector3& max, int32 nAxis, float& fStart, float& fEnd);
        static bool GetDistanceFromRayAABBAxis(const FRay& ray, const FAABB& aabb, int32 nAxis, float& fStart, float& fEnd);

        //Intersection Point Perpendicular To Line/Plane
        static void GetIntersectionPointPerpendicularToLine(const FVector3& pt, const FVector3& ptLine1, const FVector3& ptLine2, FVector3& vIntersection);
        static void GetIntersectionPointPerpendicularToPlane(const FVector3& pt, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, FVector3& vIntersection);

        //Intersection Point From Line2
        static bool GetIntersectionPointFromLine2(const FVector3& ptLine11, const FVector3& ptLine12, const FVector3& ptLine21, const FVector3& ptLine22, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);
        static bool GetIntersectionPointFromLine2(const FSegment& segment1, const FSegment& segment2, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);

        //Intersection Point From Line-Triangle/Quad/Plane
        static bool GetIntersectionPointFromLineTriangle(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);
        static bool GetIntersectionPointFromLineTriangle(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);
        static bool GetIntersectionPointFromLineQuad(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, const FVector3& pt4, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);
        static bool GetIntersectionPointFromLineQuad(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, const FVector3& pt4, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);
        static bool GetIntersectionPointFromLinePlane(const FVector3& ptLine1, const FVector3& ptLine2, const FPlane& plane, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);
        static bool GetIntersectionPointFromLinePlane(const FSegment& segment, const FPlane& plane, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);

        //Intersection Point From Ray-Line/Triangle/Quad/Plane
        static bool GetIntersectionPointFromRayLine(const FRay& ray, const FVector3& pt1, const FVector3& pt2, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);
        static bool GetIntersectionPointFromRayLine(const FRay& ray, const FSegment& segment, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);
        static bool GetIntersectionPointFromRayTriangle(const FRay& ray, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);
        static bool GetIntersectionPointFromRayTriangle(const FRay& ray, const FTriangle& triangle, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);
        static bool GetIntersectionPointFromRayQuad(const FRay& ray, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, const FVector3& pt4, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);
        static bool GetIntersectionPointFromRayQuad(const FRay& ray, const FQuad& quad, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);
        static bool GetIntersectionPointFromRayPlane(const FRay& ray, const FPlane& plane, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);
        static bool GetIntersectionPointFromRayCircle(const FRay& ray, const FCircle& circle, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);
        static bool GetIntersectionPointFromRayAABB(const FRay& ray, const FAABB& aabb, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);
        static bool GetIntersectionPointFromRaySphere(const FRay& ray, const FSphere& sphere, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);

        //Intersection Line From Plane2
        static bool GetIntersectionLineFromPlane2(const FVector3& pP11, const FVector3& pP12, const FVector3& pP13, const FVector3& pP21, const FVector3& pP22, const FVector3& pP23, FVector3& pL1, FVector3& pL2, float fEpsilon = FMath::ms_fEpsilon);
        static bool GetIntersectionLineFromPlane2(const FVector3& pP11, const FVector3& pP12, const FVector3& pP13, const FVector3& pP21, const FVector3& pP22, const FVector3& pP23, FSegment& segment, float fEpsilon = FMath::ms_fEpsilon);

        //Area From Triangle
        template<typename T>
        static T GetAreaFromTriangle(const FTPoint<T>& a, const FTPoint<T>& b, const FTPoint<T>& c)
        {
            T x1 = b.x - a.x;  
            T y1 = b.y - a.y;  
            T x2 = c.x - a.x;  
            T y2 = c.y - a.y;  
            return Abs(x1 * y2 - x2 * y1) / 2;
        }

        //Radius From AABB
        static float GetRadiusFromAABB(const FAABB& aabb);


    public:
        //Direction - IsParallel/IsPerpendicular
        static bool Direction_IsParallel(const FVector3& vDir1, const FVector3& vDir2, float fEpsilon = FMath::ms_fEpsilon);
        static bool Direction_IsPerpendicular(const FVector3& vDir1, const FVector3& vDir2, float fEpsilon = FMath::ms_fEpsilon);
        static bool Direction_IsSameDirection(const FVector3& vDir1, const FVector3& vDir2);

        //Point - InPlaneNormalSide
        static bool Point_InPlaneNormalSide(const FVector3& pt, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3);
        static bool Point_InPlaneNormalSide(const FVector3& pt, const FPlane& plane);

        //Point - InLine
        static bool Points3_InLine(const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, float fEpsilon = FMath::ms_fEpsilon);

        //Point - InLineSameSide/NotInLineSameSide
        static bool Points2_InLineSameSide(const FVector3& pt1, const FVector3& pt2, const FVector3& ptLine1, const FVector3& ptLine2, float fEpsilon = FMath::ms_fEpsilon);
        static bool Points2_InLineSameSide(const FVector3& pt1, const FVector3& pt2, const FSegment& segment, float fEpsilon = FMath::ms_fEpsilon);
        static bool Points2_NotInLineSameSide(const FVector3& pt1, const FVector3& pt2, const FVector3& ptLine1, const FVector3& ptLine2, float fEpsilon = FMath::ms_fEpsilon);
        static bool Points2_NotInLineSameSide(const FVector3& pt1, const FVector3& pt2, const FSegment& segment, float fEpsilon = FMath::ms_fEpsilon);

        //Point - OnPlane
        static bool Points4_OnPlane(const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, const FVector3& pt4, float fEpsilon = FMath::ms_fEpsilon);

        //Point - InPlaneSameSide/NotInPlaneSameSide
        static bool Points2_InPlaneSameSide(const FVector3& pt1, const FVector3& pt2, const FVector3& ptPlane, const FVector3& vPlaneNormal, float fEpsilon = FMath::ms_fEpsilon);
        static bool Points2_InPlaneSameSide(const FVector3& pt1, const FVector3& pt2, const FVector3& ptPlane1, const FVector3& ptPlane2, const FVector3& ptPlane3, float fEpsilon = FMath::ms_fEpsilon);
        static bool Points2_InPlaneSameSide(const FVector3& pt1, const FVector3& pt2, const FPlane& plane, float fEpsilon = FMath::ms_fEpsilon);
        static bool Points2_NotInPlaneSameSide(const FVector3& pt1, const FVector3& pt2, const FVector3& ptPlane, const FVector3& vPlaneNormal, float fEpsilon = FMath::ms_fEpsilon);
        static bool Points2_NotInPlaneSameSide(const FVector3& pt1, const FVector3& pt2, const FVector3& ptPlane1, const FVector3& ptPlane2, const FVector3& ptPlane3, float fEpsilon = FMath::ms_fEpsilon);
        static bool Points2_NotInPlaneSameSide(const FVector3& pt1, const FVector3& pt2, const FPlane& plane, float fEpsilon = FMath::ms_fEpsilon);

        //Line - Line Parallel/NotParallel
        static bool LineLine_Parallel(const FVector3& ptL_11, const FVector3& ptL_12, const FVector3& ptL_21, const FVector3& ptL_22, float fEpsilon = FMath::ms_fEpsilon);
        static bool LineLine_Parallel(const FVector3& ptL_11, const FVector3& ptL_12, const FSegment& segment, float fEpsilon = FMath::ms_fEpsilon);
        static bool LineLine_Parallel(const FSegment& segment1, const FSegment& segment2, float fEpsilon = FMath::ms_fEpsilon);
        static bool LineLine_NotParallel(const FVector3& ptL_11, const FVector3& ptL_12, const FVector3& ptL_21, const FVector3& ptL_22, float fEpsilon = FMath::ms_fEpsilon);
        static bool LineLine_NotParallel(const FVector3& ptL_11, const FVector3& ptL_12, const FSegment& segment, float fEpsilon = FMath::ms_fEpsilon);
        static bool LineLine_NotParallel(const FSegment& segment1, const FSegment& segment2, float fEpsilon = FMath::ms_fEpsilon);

        //Line - Line Perpendicular/NotPerpendicular
        static bool LineLine_Perpendicular(const FVector3& ptL_11, const FVector3& ptL_12, const FVector3& ptL_21, const FVector3& ptL_22, float fEpsilon = FMath::ms_fEpsilon);
        static bool LineLine_Perpendicular(const FVector3& ptL_11, const FVector3& ptL_12, const FSegment& segment, float fEpsilon = FMath::ms_fEpsilon);
        static bool LineLine_Perpendicular(const FSegment& segment1, const FSegment& segment2, float fEpsilon = FMath::ms_fEpsilon);
        static bool LineLine_NotPerpendicular(const FVector3& ptL_11, const FVector3& ptL_12, const FVector3& ptL_21, const FVector3& ptL_22, float fEpsilon = FMath::ms_fEpsilon);
        static bool LineLine_NotPerpendicular(const FVector3& ptL_11, const FVector3& ptL_12, const FSegment& segment, float fEpsilon = FMath::ms_fEpsilon);
        static bool LineLine_NotPerpendicular(const FSegment& segment1, const FSegment& segment2, float fEpsilon = FMath::ms_fEpsilon);

        //Line - Line Intersect/NotIntersect
        static bool LineLine_Intersect(const FVector3& ptL_11, const FVector3& ptL_12, const FVector3& ptL_21, const FVector3& ptL_22, bool includeBorder = true, float fEpsilon = FMath::ms_fEpsilon);
        static bool LineLine_Intersect(const FVector3& ptL_11, const FVector3& ptL_12, const FSegment& segment, bool includeBorder = true, float fEpsilon = FMath::ms_fEpsilon);
        static bool LineLine_Intersect(const FSegment& segment1, const FSegment& segment2, bool includeBorder = true, float fEpsilon = FMath::ms_fEpsilon);
        static bool LineLine_NotIntersect(const FVector3& ptL_11, const FVector3& ptL_12, const FVector3& ptL_21, const FVector3& ptL_22, bool includeBorder = true, float fEpsilon = FMath::ms_fEpsilon);
        static bool LineLine_NotIntersect(const FVector3& ptL_11, const FVector3& ptL_12, const FSegment& segment, bool includeBorder = true, float fEpsilon = FMath::ms_fEpsilon);
        static bool LineLine_NotIntersect(const FSegment& segment1, const FSegment& segment2, bool includeBorder = true, float fEpsilon = FMath::ms_fEpsilon);

        //Line - Plane Parallel/NotParallel
        static bool LinePlane_Parallel(const FVector3& vDir, const FVector3& vNormal, float fEpsilon = FMath::ms_fEpsilon);
        static bool LinePlane_Parallel(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, float fEpsilon = FMath::ms_fEpsilon);
        static bool LinePlane_Parallel(const FVector3& ptLine1, const FVector3& ptLine2, const FPlane& plane, float fEpsilon = FMath::ms_fEpsilon);
        static bool LinePlane_Parallel(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, float fEpsilon = FMath::ms_fEpsilon);
        static bool LinePlane_Parallel(const FSegment& segment, const FPlane& plane, float fEpsilon = FMath::ms_fEpsilon);
        static bool LinePlane_NotParallel(const FVector3& vDir, const FVector3& vNormal, float fEpsilon = FMath::ms_fEpsilon);
        static bool LinePlane_NotParallel(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, float fEpsilon = FMath::ms_fEpsilon);
        static bool LinePlane_NotParallel(const FVector3& ptLine1, const FVector3& ptLine2, const FPlane& plane, float fEpsilon = FMath::ms_fEpsilon);
        static bool LinePlane_NotParallel(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, float fEpsilon = FMath::ms_fEpsilon);
        static bool LinePlane_NotParallel(const FSegment& segment, const FPlane& plane, float fEpsilon = FMath::ms_fEpsilon);

        //Line - Plane Perpendicular/NotPerpendicular
        static bool LinePlane_Perpendicular(const FVector3& vDir, const FVector3& vNormal, float fEpsilon = FMath::ms_fEpsilon);
        static bool LinePlane_Perpendicular(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, float fEpsilon = FMath::ms_fEpsilon);
        static bool LinePlane_Perpendicular(const FVector3& ptLine1, const FVector3& ptLine2, const FPlane& plane, float fEpsilon = FMath::ms_fEpsilon);
        static bool LinePlane_Perpendicular(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, float fEpsilon = FMath::ms_fEpsilon);
        static bool LinePlane_Perpendicular(const FSegment& segment, const FPlane& plane, float fEpsilon = FMath::ms_fEpsilon);
        static bool LinePlane_NotPerpendicular(const FVector3& vDir, const FVector3& vNormal, float fEpsilon = FMath::ms_fEpsilon);
        static bool LinePlane_NotPerpendicular(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, float fEpsilon = FMath::ms_fEpsilon);
        static bool LinePlane_NotPerpendicular(const FVector3& ptLine1, const FVector3& ptLine2, const FPlane& plane, float fEpsilon = FMath::ms_fEpsilon);
        static bool LinePlane_NotPerpendicular(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, float fEpsilon = FMath::ms_fEpsilon);
        static bool LinePlane_NotPerpendicular(const FSegment& segment, const FPlane& plane, float fEpsilon = FMath::ms_fEpsilon);

        //Line - Triangle Intersect/NotIntersect
        static bool LineTriangle_Intersect(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, bool includeBorder = true, float fEpsilon = FMath::ms_fEpsilon);
        static bool LineTriangle_Intersect(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, bool includeBorder = true, float fEpsilon = FMath::ms_fEpsilon);
        static bool LineTriangle_NotIntersect(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, bool includeBorder = true, float fEpsilon = FMath::ms_fEpsilon);
        static bool LineTriangle_NotIntersect(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, bool includeBorder = true, float fEpsilon = FMath::ms_fEpsilon);

        //Plane - Plane Parallel/NotParallel
        static bool PlanePlane_Parallel(const FVector3& pP11, const FVector3& pP12, const FVector3& pP13, const FVector3& pP21, const FVector3& pP22, const FVector3& pP23, float fEpsilon = FMath::ms_fEpsilon);
        static bool PlanePlane_NotParallel(const FVector3& pP11, const FVector3& pP12, const FVector3& pP13, const FVector3& pP21, const FVector3& pP22, const FVector3& pP23, float fEpsilon = FMath::ms_fEpsilon);
        static bool PlanePlane_Parallel(const FPlane& plane1, const FPlane& plane2, float fEpsilon = FMath::ms_fEpsilon); 
        static bool PlanePlane_NotParallel(const FPlane& plane1, const FPlane& plane2, float fEpsilon = FMath::ms_fEpsilon); 

        //Plane - Plane Perpendicular/NotPerpendicular
        static bool PlanePlane_Perpendicular(const FVector3& pP11, const FVector3& pP12, const FVector3& pP13, const FVector3& pP21, const FVector3& pP22, const FVector3& pP23, float fEpsilon = FMath::ms_fEpsilon);
        static bool PlanePlane_NotPerpendicular(const FVector3& pP11, const FVector3& pP12, const FVector3& pP13, const FVector3& pP21, const FVector3& pP22, const FVector3& pP23, float fEpsilon = FMath::ms_fEpsilon);
        static bool PlanePlane_Perpendicular(const FPlane& plane1, const FPlane& plane2, float fEpsilon = FMath::ms_fEpsilon);
        static bool PlanePlane_NotPerpendicular(const FPlane& plane1, const FPlane& plane2, float fEpsilon = FMath::ms_fEpsilon);

        //Quad - Convex
        static bool Quad_IsConvex(const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, const FVector3& pt4, float fEpsilon = FMath::ms_fEpsilon);
        static bool Quad_IsConvex(const FQuad& quad, float fEpsilon = FMath::ms_fEpsilon);
        static bool Quad_IsConcave(const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, const FVector3& pt4, FVector3& vConcave, int& nIndex, float fEpsilon = FMath::ms_fEpsilon);
        static bool Quad_IsConcave(const FVector3* pPt, FVector3& vConcave, int& nIndex, float fEpsilon = FMath::ms_fEpsilon);
        static bool Quad_IsConcave(const FQuad& quad, FVector3& vConcave, int& nIndex, float fEpsilon = FMath::ms_fEpsilon);


    public:
        //Point - Line
        template<typename T>
		static bool Intersects_PointInLine2D(const FTPoint<T>& pt, const FTLine<T>& line)
		{
            return line.PtInLine(pt);
		}
        static bool Intersects_PointInLine2DI(const FPointI& pt, const FLineI& line)
        {
            return Intersects_PointInLine2D<int32>(pt, line);
        }
        static bool Intersects_PointInLine2DF(const FPointF& pt, const FLineF& line)
        {
            return Intersects_PointInLine2D<float>(pt, line);
        }
        static bool Intersects_PointInLine(const FVector3& pt, const FVector3& ptLineStart, const FVector3& ptLineEnd, bool includeSE = true, float fEpsilon = FMath::ms_fEpsilon);
        static bool Intersects_PointInLine(const FVector3& pt, const FSegment& segment, bool includeSE = true, float fEpsilon = FMath::ms_fEpsilon);

        //Point - Triangle
        template<typename T>
        static bool Intersects_PointInTriangle2D(const FTPoint<T>& pt, const FTPoint<T>& a, const FTPoint<T>& b, const FTPoint<T>& c)
        {
            T area_ABC = GetAreaFromTriangle(a, b, c);
            T area_PAB = GetAreaFromTriangle(pt, a, b);
            T area_PAC = GetAreaFromTriangle(pt, a, c);
            T area_PBC = GetAreaFromTriangle(pt, b, c);

            if (Abs(area_PAB + area_PBC + area_PAC - area_ABC) < ms_fEpsilon)
                return true;
            return false;
        }
        static bool Intersects_PointInTriangle2DI(const FPointI& pt, const FPointI& a, const FPointI& b, const FPointI& c)
        {
            return Intersects_PointInTriangle2D<int32>(pt, a, b, c);
        }
        static bool Intersects_PointInTriangle2DF(const FPointF& pt, const FPointF& a, const FPointF& b, const FPointF& c)
        {
            return Intersects_PointInTriangle2D<float>(pt, a, b, c);
        }
        static bool Intersects_PointInTriangle(const FVector3& pt, const FVector3& a, const FVector3& b, const FVector3& c, bool includeBorder = true, float fEpsilon = FMath::ms_fEpsilon);


        //Point - Rect
        template<typename T>
		static bool Intersects_PointInRect2D(const FTPoint<T>& pt, const FTRect<T>& rect)
		{
            return rect.PtInRect(pt);
		}
        static bool Intersects_PointInRect2DI(const FPointI& pt, const FRectI& rect)
        {
            return Intersects_PointInRect2D<int32>(pt, rect);
        }
        static bool Intersects_PointInRect2DF(const FPointF& pt, const FRectF& rect)
        {
            return Intersects_PointInRect2D<float>(pt, rect);
        }

        static bool Intersects_PointInRect2DI(const FPointI& pt, const FPointI& a, const FPointI& b, const FPointI& c, const FPointI& d);
        static bool Intersects_PointInRect2DF(const FPointF& pt, const FPointF& a, const FPointF& b, const FPointF& c, const FPointF& d);
        static bool Intersects_PointInRect(const FVector3& pt, const FVector3& a, const FVector3& b, const FVector3& c, const FVector3& d, float fEpsilon = FMath::ms_fEpsilon);


        //Point - Circle
        template<typename T>
		static bool Intersects_PointInCircle2D(const FTPoint<T>& pt, const FTPoint<T>& center, T radius)
        {
            FVector2 v((float)(pt.x - center.x), (float)(pt.y - center.y));
            if (Length(v) <= radius)
                return true;
            return false;
        }
        static bool Intersects_PointInCircle2DI(const FPointI& pt, const FPointI& center, int32 radius)
        {
            return Intersects_PointInCircle2D<int32>(pt, center, radius);
        }
        static bool Intersects_PointInCircle2DF(const FPointF& pt, const FPointF& center, float radius)
        {
            return Intersects_PointInCircle2D<float>(pt, center, radius);
        }
        static bool Intersects_PointInCircle(const FVector3& pt, const FVector3& center, float radius);

    public:
        //Ray - Shape
        static std::pair<bool, float> Intersects_RaySegment(const FRay& ray, const FVector3& s, const FVector3& e, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);
        static std::pair<bool, float> Intersects_RaySegment(const FRay& ray, const FSegment& segment, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);
        static std::pair<bool, float> Intersects_RayTriangle(const FRay& ray, const FVector3& a, const FVector3& b, const FVector3& c, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);
        static std::pair<bool, float> Intersects_RayTriangle(const FRay& ray, const FTriangle& triangle, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);
        static std::pair<bool, float> Intersects_RayQuad(const FRay& ray, const FVector3& a, const FVector3& b, const FVector3& c, const FVector3& d, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);
        static std::pair<bool, float> Intersects_RayQuad(const FRay& ray, const FQuad& quad, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);
        static std::pair<bool, float> Intersects_RayPlane(const FRay& ray, const FPlane& plane, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);
        static std::pair<bool, float> Intersects_RayPlaneVector(const FRay& ray, const FPlaneVector& aPlanes, bool normalIsOutside);
        static std::pair<bool, float> Intersects_RayPlaneList(const FRay& ray, FPlaneList& listPlanes, bool normalIsOutside);
        static std::pair<bool, float> Intersects_RayCircle(const FRay& ray, const FPlane& plane, const FVector3& center, float radius, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);
        static std::pair<bool, float> Intersects_RayCircle(const FRay& ray, const FCircle& circle, FVector3& vIntersection, float fEpsilon = FMath::ms_fEpsilon);
        static std::pair<bool, float> Intersects_RayAABB(const FRay& ray, const FAABB& aabb);
        static bool Intersects_RayAABB(const FRay& ray, const FAABB& aabb, float* d1, float* d2);
        static bool Intersects_RayAABB(const FRay& ray, const FAABB& aabb, FVector3& vIntersection1, FVector3& vIntersection2);
        static std::pair<bool, float> Intersects_RaySphere(const FRay& ray, const FSphere& sphere, bool discardInside = true);
        static int Intersects_RaySphere(const FRay& ray, const FSphere& sphere, bool& isInside, float* d1, float* d2);
        static int Intersects_RaySphere(const FRay& ray, const FSphere& sphere, bool& isInside, FVector3& vIntersection1, FVector3& vIntersection2);
        static std::pair<bool, float> Intersects_RayCylinder(const FRay& ray, const FCylinder& cylinder, bool discardInside = true);
        static int Intersects_RayCylinder(const FRay& ray, const FCylinder& cylinder, bool& isInside, float* d1, float* d2);
        static int Intersects_RayCylinder(const FRay& ray, const FCylinder& cylinder, bool& isInside, FVector3& vIntersection1, FVector3& vIntersection2);	
        static std::pair<bool, float> Intersects_RayCapsule(const FRay& ray, const FCapsule& capsule, bool discardInside = true);
        static int Intersects_RayCapsule(const FRay& ray, const FCapsule& capsule, bool& isInside, float* d1, float* d2);
        static int Intersects_RayCapsule(const FRay& ray, const FCapsule& capsule, bool& isInside, FVector3& vIntersection1, FVector3& vIntersection2);
        static std::pair<bool, float> Intersects_RayCone(const FRay& ray, const FCone& cone, bool discardInside = true);
        static int Intersects_RayCone(const FRay& ray, const FCone& cone, bool& isInside, float* d1, float* d2);
        static int Intersects_RayCone(const FRay& ray, const FCone& cone, bool& isInside, FVector3& vIntersection1, FVector3& vIntersection2);
        static std::pair<bool, float> Intersects_RayTorus(const FRay& ray, const FTorus& torus, bool discardInside = true);
        static int Intersects_RayTorus(const FRay& ray, const FTorus& torus, bool& isInside, float* d1, float* d2);
        static int Intersects_RayTorus(const FRay& ray, const FTorus& torus, bool& isInside, FVector3& vIntersection1, FVector3& vIntersection2);

        //Ray - Shape Test
        static bool Intersects_RaySegment_Test(const FRay& ray, const FVector3& s, const FVector3& e, float fEpsilon = FMath::ms_fEpsilon);
        static bool Intersects_RaySegment_Test(const FRay& ray, const FSegment& segment, float fEpsilon = FMath::ms_fEpsilon);
        static bool Intersects_RayTriangle_Test(const FRay& ray, const FVector3& a, const FVector3& b, const FVector3& c, float fEpsilon = FMath::ms_fEpsilon);
        static bool Intersects_RayTriangle_Test(const FRay& ray, const FTriangle& triangle, float fEpsilon = FMath::ms_fEpsilon);
        static bool Intersects_RayQuad_Test(const FRay& ray, const FVector3& a, const FVector3& b, const FVector3& c, const FVector3& d, float fEpsilon = FMath::ms_fEpsilon);
        static bool Intersects_RayQuad_Test(const FRay& ray, const FQuad& quad, float fEpsilon = FMath::ms_fEpsilon);
        static bool Intersects_RayPlane_Test(const FRay& ray, const FPlane& plane, float fEpsilon = FMath::ms_fEpsilon);
        static bool Intersects_RayCircle_Test(const FRay& ray, const FPlane& plane, const FVector3& center, float radius, float fEpsilon = FMath::ms_fEpsilon);
        static bool Intersects_RayCircle_Test(const FRay& ray, const FCircle& circle, float fEpsilon = FMath::ms_fEpsilon);
        static bool Intersects_RayAABB_Test(const FRay& ray, const FAABB& aabb);
        static bool	Intersects_RayAABB_Test(const FRay& ray, const FAABB& aabb, float* d1, float* d2);
        static bool	Intersects_RayAABB_Test(const FRay& ray, const FAABB& aabb, FVector3& vIntersection1, FVector3& vIntersection2);
        static bool Intersects_RaySphere_Test(const FRay& ray, const FSphere& sphere, bool discardInside = true);
        static bool Intersects_RaySphere_Test(const FRay& ray, const FSphere& sphere, bool& isInside, float* d1, float* d2);
        static bool Intersects_RaySphere_Test(const FRay& ray, const FSphere& sphere, bool& isInside, FVector3& vIntersection1, FVector3& vIntersection2);
        static bool Intersects_RayCylinder_Test(const FRay& ray, const FCylinder& cylinder, bool discardInside = true);
        static bool Intersects_RayCylinder_Test(const FRay& ray, const FCylinder& cylinder, bool& isInside, float* d1, float* d2);
        static bool Intersects_RayCylinder_Test(const FRay& ray, const FCylinder& cylinder, bool& isInside, FVector3& vIntersection1, FVector3& vIntersection2);
        static bool Intersects_RayCapsule_Test(const FRay& ray, const FCapsule& capsule, bool discardInside = true);
        static bool Intersects_RayCapsule_Test(const FRay& ray, const FCapsule& capsule, bool& isInside, float* d1, float* d2);
        static bool Intersects_RayCapsule_Test(const FRay& ray, const FCapsule& capsule, bool& isInside, FVector3& vIntersection1, FVector3& vIntersection2);
        static bool Intersects_RayCone_Test(const FRay& ray, const FCone& cone, bool discardInside = true);
        static bool Intersects_RayCone_Test(const FRay& ray, const FCone& cone, bool& isInside, float* d1, float* d2);
        static bool Intersects_RayCone_Test(const FRay& ray, const FCone& cone, bool& isInside, FVector3& vIntersection1, FVector3& vIntersection2);
        static bool Intersects_RayTorus_Test(const FRay& ray, const FTorus& torus, bool discardInside = true);
        static bool Intersects_RayTorus_Test(const FRay& ray, const FTorus& torus, bool& isInside, float* d1, float* d2);
        static bool Intersects_RayTorus_Test(const FRay& ray, const FTorus& torus, bool& isInside, FVector3& vIntersection1, FVector3& vIntersection2);
        
        //Sphere - Shape
        static bool	Intersects_SpherePlane(const FSphere& sphere, const FPlane& plane);
        static bool Intersects_SphereAABB(const FSphere& sphere, const FAABB& aabb);
        static bool	Intersects_SphereFrustum(const FSphere& sphere, const FFrustum& frustum);
        
        //static bool Intersects_SegmentAABB(const FSegment& s, const FAABB& aabb);

        //Plane - Shape
        static bool	Intersects_PlaneAABB(const FPlane& plane, const FAABB& aabb);

    public:
        static float GaussianDistribution(float x, float offset = 0.0f, float scale = 1.0f);

		static FMatrix4 BuildReflectionMatrix(const FPlane& p);	

        static FVector3 CalculateTangentSpaceVector(const FVector3& position1, const FVector3& position2, const FVector3& position3,
												    float u1, float v1, float u2, float v2, float u3, float v3);
        
		static FMatrix4 MakeMatrix4ViewLH(const FVector3& vPos, const FQuaternion& qRot, const FMatrix4* pReflectMatrix = nullptr);
        static FMatrix4 MakeMatrix4ProjectionPerspectiveLH(float rFovY, float fAspect, float fNear, float fFar);

        static FVector3 TransformFromScreenToWorld(const FVector3& vScreenCoord, const FMatrix4& mat4ModelView, const FMatrix4& mat4Proj, const FVector4& vViewport);
        static void TransformFromScreenToWorld(const FVector3& vScreenCoord, const FMatrix4& mat4ModelView, const FMatrix4& mat4Proj, const FVector4& vViewport, FVector3& vWorldCoord);
    };

}; //LostPeterFoundation

#endif