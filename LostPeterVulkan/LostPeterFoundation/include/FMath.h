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
		static float Min(const FVector2& v) { return glm::min(v.x, v.y); }
        static float Min(const FVector3& v) { return glm::min(v.x, v.y, v.z); }
        static float Min(const FVector4& v) { return glm::min(v.x, v.y, v.z, v.w); }
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

        static float Max(const FVector2& v) { return glm::max(v.x, v.y); }
        static float Max(const FVector3& v) { return glm::max(v.x, v.y, v.z); }
        static float Max(const FVector4& v) { return glm::max(v.x, v.y, v.z, v.w); }
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

        static bool IsGreat(const FVector3& v1, const FVector3& v2)
        {
            if (v1.x > v2.x && v1.y > v2.y && v1.z > v2.z)
                return true;
            return false;
        }    
		static bool IsEqual(float a, float b, float tolerance = std::numeric_limits<float>::epsilon())
        {
            return (fabs(b - a) <= tolerance) ? true : false; 
        }
        static bool IsEqual(const FVector3& v1, const FVector3& v2)
        {
            if(v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
                return true;
            return false;
        }
        static bool IsEqual(const FVector3& v1, const FVector3& v2, float tolerance)
        {
            if (FMath::IsEqual(v1.x, v2.x, tolerance) &&
				FMath::IsEqual(v1.y, v2.y, tolerance) &&
				FMath::IsEqual(v1.z, v2.z, tolerance))
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
			return (fLen < (1e-06 * 1e-06));
        }

	public:
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

        static FVector3 Cross(const FVector3& v1, const FVector3& v2)
        {
            return glm::cross(v1, v2);
        }

        static FVector3 Normalize(const FVector3& v)
        {
            return glm::normalize(v);
        }

        static FQuaternion Normalize(const FQuaternion& q)
        {
            float len = glm::length(q);
            return q / len;
        }

        static float Length(const FVector3& v)
        {
            return glm::length(v);
        }
        static float Length2(const FVector3& v)
        {
            return glm::length2(v);
        }

        static float Distance2(const FVector3& v1, const FVector3& v2)
        {
            return Length2(v1 - v2);
        }

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

        static FVector3 Reflect(const FVector3& vI, const FVector3& vN)
        {
            return glm::reflect(vI, vN);
        }

        static FVector3 Refract(const FVector3& vI, const FVector3& vN, float eta)
        {
            return glm::refract(vI, vN, eta);
        }

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
        static float GetRadiusFromAABB(const FAABB& aabb);

    public:
        //static bool Intersects_PointLine();
        

    public:
        static std::pair<bool, float> Intersects_RayTriangle(const FRay& ray, const FVector3& a, const FVector3& b, const FVector3& c,
                                                             bool positiveSide = true, bool negativeSide = true);
        static std::pair<bool, float> Intersects_RayTriangle(const FRay& ray, const FVector3& a, const FVector3& b, const FVector3& c, const FVector3& normal,
                                                             bool positiveSide = true, bool negativeSide = true);
        static std::pair<bool, float> Intersects_RayPlane(const FRay& ray, const FPlane& plane);
        static std::pair<bool, float> Intersects_RayPlaneVector(const FRay& ray, const FPlaneVector& aPlanes, bool normalIsOutside);
        static std::pair<bool, float> Intersects_RayPlaneList(const FRay& ray, FPlaneList& listPlanes, bool normalIsOutside);
        static std::pair<bool, float> Intersects_RaySphere(const FRay& ray, const FSphere& sphere, bool discardInside = true);			
        static std::pair<bool, float> Intersects_RayAABB(const FRay& ray, const FAABB& aabb);
        static bool Intersects_RayAABB_Test(const FRay& ray, const FAABB& aabb);
        static bool	Intersects_RayAABB(const FRay& ray, const FAABB& aabb, float* d1, float* d2);
            
        static bool	Intersects_SpherePlane(const FSphere& sphere, const FPlane& plane);
        static bool Intersects_SphereAABB(const FSphere& sphere, const FAABB& aabb);
        static bool	Intersects_SphereFrustum(const FSphere& sphere, const FFrustum& frustum);
        
        //static bool Intersects_SegmentAABB(const FSegment& s, const FAABB& aabb);

        static bool	Intersects_PlaneAABB(const FPlane& plane, const FAABB& aabb);

    public:
		static FMatrix4 BuildReflectionMatrix(const FPlane& p);	

        static FVector3 CalculateTangentSpaceVector(const FVector3& position1, const FVector3& position2, const FVector3& position3,
												    float u1, float v1, float u2, float v2, float u3, float v3);

        static FVector4 CalculateFaceNormal(const FVector3& v1, const FVector3& v2, const FVector3& v3);
		static FVector3 CalculateBasicFaceNormal(const FVector3& v1, const FVector3& v2, const FVector3& v3);
		static FVector4 CalculateFaceNormalWithoutNormalize(const FVector3& v1, const FVector3& v2, const FVector3& v3);
		static FVector3 CalculateBasicFaceNormalWithoutNormalize(const FVector3& v1, const FVector3& v2, const FVector3& v3);

		static float GaussianDistribution(float x, float offset = 0.0f, float scale = 1.0f);

		static FMatrix4 MakeMatrix4ViewLH(const FVector3& vPos, const FQuaternion& qRot, const FMatrix4* pReflectMatrix = nullptr);
        static FMatrix4 MakeMatrix4ProjectionPerspectiveLH(float rFovY, float fAspect, float fNear, float fFar);

        static FVector3 TransformFromScreenToWorld(const FVector3& vScreenCoord, const FMatrix4& mat4ModelView, const FMatrix4& mat4Proj, const FVector4& vViewport);
        static void TransformFromScreenToWorld(const FVector3& vScreenCoord, const FMatrix4& mat4ModelView, const FMatrix4& mat4Proj, const FVector4& vViewport, FVector3& vWorldCoord);
    };

}; //LostPeterFoundation

#endif