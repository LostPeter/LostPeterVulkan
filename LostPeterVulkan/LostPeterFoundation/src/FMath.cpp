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

#include "../include/FMath.h"
#include "../include/FRay.h"
#include "../include/FSegment.h"
#include "../include/FTriangle.h"
#include "../include/FQuad.h"
#include "../include/FPlane.h"
#include "../include/FPlaneBoundedVolume.h"
#include "../include/FCircle.h"
#include "../include/FSphere.h"
#include "../include/FAABB.h"
#include "../include/FFrustum.h"
#include "../include/FCylinder.h"
#include "../include/FCapsule.h"
#include "../include/FCone.h"
#include "../include/FTorus.h"
#include "../include/FEquationQuadratic.h"
#include "../include/FEquationCubic.h"
#include "../include/FEquationQuartic.h"

namespace LostPeterFoundation
{
    const float FMath::ms_fPI_Half = glm::pi<float>() / 2.0f;
    const float FMath::ms_fPI = glm::pi<float>();
    const float FMath::ms_fPI_Two = glm::pi<float>() * 2.0f;
    const float FMath::ms_fDeg2Rad = FMath::ms_fPI / float(180.0f);		
    const float FMath::ms_fRad2Deg = float(180.0f) / FMath::ms_fPI;	
    const float FMath::ms_fLog2 = log(float(2.0f));

    const double FMath::ms_dPI_Half = glm::pi<double>() / 2.0;
    const double FMath::ms_dPI = glm::pi<double>();
    const double FMath::ms_dPI_Two = glm::pi<double>() * 2.0;
    const double FMath::ms_dDeg2Rad = FMath::ms_dPI / float(180.0);		
    const double FMath::ms_dRad2Deg = float(180.0) / FMath::ms_dPI;	

    const float FMath::ms_fPosInfinity =  std::numeric_limits<float>::infinity();	
    const float FMath::ms_fNegInfinity = -std::numeric_limits<float>::infinity();
    const float FMath::ms_fEpsilon = std::numeric_limits<float>::epsilon();
    const double FMath::ms_dEpsilon = std::numeric_limits<double>::epsilon();
    const float FMath::ms_fRayFar = 10000000000.0f;

    const FVector2 FMath::ms_v2Zero = FVector2(0.0f, 0.0f);
    const FVector2 FMath::ms_v2One = FVector2(1.0f, 1.0f);
    const FVector2 FMath::ms_v2UnitX = FVector2(1.0f, 0.0f);
    const FVector2 FMath::ms_v2UnitY = FVector2(0.0f, 1.0f);
    const FVector2 FMath::ms_v2UnitNegX = FVector2(-1.0f, 0.0f);
    const FVector2 FMath::ms_v2UnitNegY = FVector2(0.0f, -1.0f);

    const FVector3 FMath::ms_v3Zero = FVector3(0.0f, 0.0f, 0.0f);
    const FVector3 FMath::ms_v3One = FVector3(1.0f, 1.0f, 1.0f);
    const FVector3 FMath::ms_v3Centi = FVector3(1.0f / 100.f, 1.0f / 100.f, 1.0f / 100.f);
    const FVector3 FMath::ms_v3UnitX = FVector3(1.0f, 0.0f, 0.0f);
    const FVector3 FMath::ms_v3UnitY = FVector3(0.0f, 1.0f, 0.0f);
    const FVector3 FMath::ms_v3UnitZ = FVector3(0.0f, 0.0f, 1.0f);
    const FVector3 FMath::ms_v3UnitNegX = FVector3(-1.0f, 0.0f, 0.0f);
    const FVector3 FMath::ms_v3UnitNegY = FVector3(0.0f, -1.0f, 0.0f);
    const FVector3 FMath::ms_v3UnitNegZ = FVector3(0.0f, 0.0f, -1.0f);

    const FVector4 FMath::ms_v4Zero = FVector4(0.0f, 0.0f, 0.0f, 0.0f);
    const FVector4 FMath::ms_v4One = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
    const FVector4 FMath::ms_v4UnitX = FVector4(1.0f, 0.0f, 0.0f, 0.0f);
    const FVector4 FMath::ms_v4UnitY = FVector4(0.0f, 1.0f, 0.0f, 0.0f);
    const FVector4 FMath::ms_v4UnitZ = FVector4(0.0f, 0.0f, 1.0f, 0.0f);
    const FVector4 FMath::ms_v4UnitW = FVector4(0.0f, 0.0f, 0.0f, 1.0f);
    const FVector4 FMath::ms_v4UnitNegX = FVector4(-1.0f, 0.0f, 0.0f, 0.0f);
    const FVector4 FMath::ms_v4UnitNegY = FVector4(0.0f, -1.0f, 0.0f, 0.0f);
    const FVector4 FMath::ms_v4UnitNegZ = FVector4(0.0f, 0.0f, -1.0f, 0.0f);
    const FVector4 FMath::ms_v4UnitNegW = FVector4(0.0f, 0.0f, 0.0f, -1.0f);

    const FQuaternion FMath::ms_qUnit = FQuaternion(glm::radians(FVector3(0.0f, 0.0f, 0.0f)));

    const FMatrix3 FMath::ms_mat3Zero = FMatrix3(0.0f, 0.0f, 0.0f,
                                                 0.0f, 0.0f, 0.0f,
                                                 0.0f, 0.0f, 0.0f);
    const FMatrix3 FMath::ms_mat3Unit = FMatrix3(1.0f, 0.0f, 0.0f,
                                                 0.0f, 1.0f, 0.0f,
                                                 0.0f, 0.0f, 1.0);

    const FMatrix4 FMath::ms_mat4Zero = FMatrix4(0.0f, 0.0f, 0.0f, 0.0f,
                                                 0.0f, 0.0f, 0.0f, 0.0f,
                                                 0.0f, 0.0f, 0.0f, 0.0f,
                                                 0.0f, 0.0f, 0.0f, 0.0f);                             
    const FMatrix4 FMath::ms_mat4Unit = FMatrix4(1.0f, 0.0f, 0.0f, 0.0f,
                                                 0.0f, 1.0f, 0.0f, 0.0f,
                                                 0.0f, 0.0f, 1.0f, 0.0f,
                                                 0.0f, 0.0f, 0.0f, 1.0f);

    const FColor FMath::ms_clBlack = FColor(0.0f, 0.0f, 0.0f, 1.0f);
    const FColor FMath::ms_clWhite = FColor(1.0f, 1.0f, 1.0f, 1.0f);
    const FColor FMath::ms_clRed = FColor(1.0f, 0.0f, 0.0f, 1.0f);
    const FColor FMath::ms_clGreen = FColor(0.0f, 1.0f, 0.0f, 1.0f);
    const FColor FMath::ms_clBlue = FColor(0.0f, 0.0f, 1.0f, 1.0f);
    const FColor FMath::ms_clGray = FColor(0.5f, 0.5f, 0.5f, 1.0f);
    const FColor FMath::ms_clDarkRed = FColor(0.5f, 0.0f, 0.0f, 1.0f);
    const FColor FMath::ms_clDarkGreen = FColor(0.0f, 0.5f, 0.0f, 1.0f);
    const FColor FMath::ms_clDarkBlue = FColor(0.0f, 0.0f, 0.5f, 1.0f);
    const FColor FMath::ms_clDarkGray = FColor(0.25f, 0.25f, 0.25f, 1.0f);
    const FColor FMath::ms_clYellow = FColor(1.0f, 1.0f, 0.0f, 1.0f);
    const FColor FMath::ms_clCyan = FColor(0.0f, 1.0f, 1.0f, 1.0f);
    const FColor FMath::ms_clMagenta = FColor(1.0f, 0.0f, 1.0f, 1.0f);
    const FColor FMath::ms_clTransparent = FColor(1.0f, 1.0f, 1.0f, 0.0f);


    //Color
#if F_ENDIAN == F_ENDIAN_BIG
	ABGR FMath::GetAsABGR(const FColor& color)
#else
	RGBA FMath::GetAsRGBA(const FColor& color)
#endif	
    {
        uint8 val8;
		uint32 val32 = 0;

		// Convert to 32bit pattern
		// (RGBA = 8888)

		// Red
		val8 = static_cast<uint8>(color.x * 255);
		val32 = val8 << 24;

		// Green
		val8 = static_cast<uint8>(color.y * 255);
		val32 += val8 << 16;

		// Blue
		val8 = static_cast<uint8>(color.z * 255);
		val32 += val8 << 8;

		// Alpha
		val8 = static_cast<uint8>(color.w * 255);
		val32 += val8;

		return val32;
    }				
#if F_ENDIAN == F_ENDIAN_BIG
	BGRA FMath::GetAsBGRA(const FColor& color)
#else
	ARGB FMath::GetAsARGB(const FColor& color)
#endif	
    {
        uint8 val8;
		uint32 val32 = 0;

		// Convert to 32bit pattern
		// (ARGB = 8888)

		// Alpha
		val8 = static_cast<uint8>(color.w * 255);
		val32 = val8 << 24;

		// Red
		val8 = static_cast<uint8>(color.x * 255);
		val32 += val8 << 16;

		// Green
		val8 = static_cast<uint8>(color.y * 255);
		val32 += val8 << 8;

		// Blue
		val8 = static_cast<uint8>(color.z * 255);
		val32 += val8;

		return val32;
    }					
#if F_ENDIAN == F_ENDIAN_BIG
	ARGB FMath::GetAsARGB(const FColor& color)
#else
	BGRA FMath::GetAsBGRA(const FColor& color)
#endif		
    {
        uint8 val8;
		uint32 val32 = 0;

		// Convert to 32bit pattern
		// (ARGB = 8888)

		// Blue
		val8 = static_cast<uint8>(color.z * 255);
		val32 = val8 << 24;

		// Green
		val8 = static_cast<uint8>(color.y * 255);
		val32 += val8 << 16;

		// Red
		val8 = static_cast<uint8>(color.x * 255);
		val32 += val8 << 8;

		// Alpha
		val8 = static_cast<uint8>(color.w * 255);
		val32 += val8;

		return val32;
    }	
#if F_ENDIAN == F_ENDIAN_BIG
	RGBA FMath::GetAsRGBA(const FColor& color)
#else
	ABGR FMath::GetAsABGR(const FColor& color)
#endif
    {
        uint8 val8;
		uint32 val32 = 0;

		// Convert to 32bit pattern
		// (ABRG = 8888)

		// Alpha
		val8 = static_cast<uint8>(color.w * 255);
		val32 = val8 << 24;

		// Blue
		val8 = static_cast<uint8>(color.z * 255);
		val32 += val8 << 16;

		// Green
		val8 = static_cast<uint8>(color.y * 255);
		val32 += val8 << 8;

		// Red
		val8 = static_cast<uint8>(color.x * 255);
		val32 += val8;

		return val32;
    }


#if F_ENDIAN == F_ENDIAN_BIG
	FColor FMath::GetAsABGR(const ABGR val)
#else
	FColor FMath::GetAsRGBA(const RGBA val)
#endif
	{
		uint32 val32 = val;

		// Convert from 32bit pattern
		// (RGBA = 8888)

		// Red
		float r = ((val32 >> 24) & 0xFF) / 255.0f;

		// Green
		float g = ((val32 >> 16) & 0xFF) / 255.0f;

		// Blue
		float b = ((val32 >> 8) & 0xFF) / 255.0f;

		// Alpha
		float a = (val32 & 0xFF) / 255.0f;

        return FColor(r, g, b, a);
	}

#if F_ENDIAN == F_ENDIAN_BIG
	FColor FMath::GetAsBGRA(const BGRA val)
#else
	FColor FMath::GetAsARGB(const ARGB val)
#endif
	{
		uint32 val32 = val;

		// Convert from 32bit pattern
		// (ARGB = 8888)

		// Alpha
		float a = ((val32 >> 24) & 0xFF) / 255.0f;

		// Red
		float r = ((val32 >> 16) & 0xFF) / 255.0f;

		// Green
		float g = ((val32 >> 8) & 0xFF) / 255.0f;

		// Blue
		float b = (val32 & 0xFF) / 255.0f;

        return FColor(r, g, b, a);
	}

#if F_ENDIAN == F_ENDIAN_BIG
	FColor FMath::GetAsARGB(const ARGB val)
#else
	FColor FMath::GetAsBGRA(const BGRA val)
#endif
	{
		uint32 val32 = val;

		// Convert from 32bit pattern
		// (ARGB = 8888)

		// Blue
		float b = ((val32 >> 24) & 0xFF) / 255.0f;

		// Green
		float g = ((val32 >> 16) & 0xFF) / 255.0f;

		// Red
		float r = ((val32 >> 8) & 0xFF) / 255.0f;

		// Alpha
		float a = (val32 & 0xFF) / 255.0f;

        return FColor(r, g, b, a);
	}

#if F_ENDIAN == F_ENDIAN_BIG
	FColor FMath::GetAsRGBA(const RGBA val)
#else
	FColor FMath::GetAsABGR(const ABGR val)
#endif
	{
		uint32 val32 = val;

		// Convert from 32bit pattern
		// (ABGR = 8888)

		// Alpha
		float a = ((val32 >> 24) & 0xFF) / 255.0f;

		// Blue
		float b = ((val32 >> 16) & 0xFF) / 255.0f;

		// Green
		float g = ((val32 >> 8) & 0xFF) / 255.0f;

		// Red
		float r = (val32 & 0xFF) / 255.0f;

        return FColor(r, g, b, a);
	}	


    static bool float_equal(float& f1, float&f2)
    {
        return f1 == f2;
    }
    static bool float_greater(float& f1, float&f2)
    {
        return f1 > f2;
    }
    static bool float_less(float& f1, float&f2)
    {
        return f1 < f2;
    }

	float FMath::AngleFromXY(float x, float y)
	{
		float theta = 0.0f;

		// Quadrant I or IV
		if (x >= 0.0f)
		{
			// If x = 0, then atanf(y/x) = +pi/2 if y > 0
			//                atanf(y/x) = -pi/2 if y < 0
			theta = atanf(y / x); // in [-pi/2, +pi/2]

			if (theta < 0.0f)
				theta += 2.0f * ms_fPI; // in [0, 2*pi).
		}

		// Quadrant II or III
		else
		{
            theta = atanf(y / x) + ms_fPI; // in [0, 2*pi).
        }

		return theta;
	}

	uint32 FMath::Power2PlusOne(uint32 number)
	{
		int32 i = 0;
		uint32 pow = 2;
		while (i < 32)
		{
			if (number <= pow + 1)
				return pow + 1;
			pow <<= 1;
			i++;
		}

		return 0;
	}

	bool FMath::IsPower2(size_t nValue, size_t& nNearestPow)
	{
		bool bRet = true;
		nNearestPow = 0;
		if(0 == nValue)
		{ 
			bRet = false;
		}
		else
		{
			while(nValue != 0x01)
			{
				if((nValue&0x01) != 0)
				{
					bRet = false;
				}
				++nNearestPow;
				nValue = (nValue>>1);
			}
		}
		return bRet;
	}


	FMatrix4 FMath::Translate(float x, float y, float z)
    {
        return Translate(FVector3(x, y, z));
    }
    FMatrix4 FMath::Translate(const FVector3& vTranslate)
    {
        return glm::translate(vTranslate);
    }

    FMatrix4 FMath::Translate(const FMatrix4& mat4, float x, float y, float z)
    {
        return Translate(mat4, FVector3(x, y, z));
    }
    FMatrix4 FMath::Translate(const FMatrix4& mat4, const FVector3& vTranslate)
    {
        return glm::translate(mat4, vTranslate);
    }


    FMatrix4 FMath::RotateX(float angleX)
    {
        float rad = glm::radians(angleX);
        return glm::eulerAngleX(rad);
    }
    FMatrix4 FMath::RotateY(float angleY)
    {
        float rad = glm::radians(angleY);
        return glm::eulerAngleY(rad);
    }
    FMatrix4 FMath::RotateZ(float angleZ)
    {
        float rad = glm::radians(angleZ);
        return glm::eulerAngleZ(rad);
    }
    FMatrix4 FMath::Rotate(float angleX, float angleY, float angleZ)
    {
        float radX = glm::radians(angleX);
        float radY = glm::radians(angleY);
        float radZ = glm::radians(angleZ);
        return glm::eulerAngleXYZ(radX, radY, radZ);
    }
    FMatrix4 FMath::Rotate(const FVector3& vAngle)
    {
        return Rotate(vAngle.x, vAngle.y, vAngle.z);
    }

    FMatrix4 FMath::Rotate(const FQuaternion& qRot)
    {
        return glm::mat4_cast(qRot);
    }

    FMatrix4 FMath::Scale(float scaleX, float scaleY, float scaleZ)
    {
        return Scale(FVector3(scaleX, scaleY, scaleZ));
    }
    FMatrix4 FMath::Scale(const FVector3& vScale)
    {
        return glm::scale(vScale);
    }
    void FMath::Scale(FMatrix3& mat3, const FVector3& vScale)
    {
        mat3[0][0] *= vScale.x;	mat3[0][1] *= vScale.x;	mat3[0][2] *= vScale.x;
        mat3[1][0] *= vScale.y;	mat3[1][1] *= vScale.y;	mat3[1][2] *= vScale.y;
        mat3[2][0] *= vScale.z;	mat3[2][1] *= vScale.z;	mat3[2][2] *= vScale.z;
    }

    FMatrix4 FMath::FromTRS(const FVector3& vTranslate, const FVector3& vAngle, const FVector3& vScale)
    {
        // Scale -> Rotate -> Translate
        FMatrix4 matScale = FMath::Scale(vScale);
        FMatrix4 matRotate = FMath::Rotate(vAngle);
        FMatrix4 matTranslate = FMath::Translate(vTranslate);
        return matTranslate * matRotate * matScale;
    }

    FMatrix4 FMath::FromTQS(const FVector3& vTranslate, const FQuaternion& qRot, const FVector3& vScale)
    {
        // Scale -> Rotate -> Translate
        FMatrix4 matScale = FMath::Scale(vScale);
        FMatrix4 matRotate = FMath::Rotate(qRot);
        FMatrix4 matTranslate = FMath::Translate(vTranslate);
        return matTranslate * matRotate * matScale;
    }

    void FMath::FromMatrix3(FMatrix3& mat3, FVector3& xAxis, FVector3& yAxis, FVector3& zAxis)
    {
        xAxis.x = mat3[0][0]; xAxis.y = mat3[0][1]; xAxis.z = mat3[0][2]; 
        yAxis.x = mat3[1][0]; yAxis.y = mat3[1][1]; yAxis.z = mat3[1][2];
        zAxis.x = mat3[2][0]; zAxis.y = mat3[2][1]; zAxis.z = mat3[2][2];
    }
    FMatrix3 FMath::ToMatrix3(const FVector3& xAxis, const FVector3& yAxis, const FVector3& zAxis)
    {
        return FMatrix3(xAxis.x, xAxis.y, xAxis.z,
                        yAxis.x, yAxis.y, yAxis.z,
                        zAxis.x, zAxis.y, zAxis.z);
    }
    FMatrix3 FMath::ToMatrix3(const FQuaternion& qRot)
    {
        return glm::toMat3(qRot);
    }
    FMatrix3 FMath::ToMatrix3(const FMatrix4& mat4)
    {
        return FMatrix3(mat4[0][0], mat4[0][1], mat4[0][2],
                        mat4[1][0], mat4[1][1], mat4[1][2],
                        mat4[2][0], mat4[2][1], mat4[2][2]);
    }

    void FMath::FromMatrix4(FMatrix4& mat4, FVector3& xAxis, FVector3& yAxis, FVector3& zAxis)
    {
        FMatrix3 mat3 = FMath::ToMatrix3(mat4);
        FMath::FromMatrix3(mat3, xAxis, yAxis, zAxis);
    }
    FMatrix4 FMath::ToMatrix4(const FVector3& xAxis, const FVector3& yAxis, const FVector3& zAxis)
    {
        return FMatrix4(xAxis.x, xAxis.y, xAxis.z, 0,
                        yAxis.x, yAxis.y, yAxis.z, 0,
                        zAxis.x, zAxis.y, zAxis.z, 0,
                               0,       0,       0, 1);
    }
    FMatrix4 FMath::ToMatrix4(const FQuaternion& qRot)
    {
        return glm::toMat4(qRot);
    }
    FMatrix4 FMath::ToMatrix4(const FVector3& vPos, const FVector3& vScale, const FQuaternion& qRot)
    {
        // Scale -> Rotate-> Translate
        FMatrix4 mat4Scale = Scale(vScale);
        FMatrix4 mat4Rot = Rotate(qRot);
        FMatrix4 mat4Trans = Translate(vPos);
        FMatrix4 mat4 = mat4Trans * mat4Rot * mat4Scale;

		// No projection term
		mat4[0][3] = 0; mat4[1][3] = 0; mat4[2][3] = 0; mat4[3][3] = 1;

        return mat4;
    }   
    FMatrix4 FMath::ToMatrix4(const FMatrix3& mat3)
    {
        return FMatrix4(mat3[0][0], mat3[0][1], mat3[0][2], 0.0f,
                        mat3[1][0], mat3[1][1], mat3[1][2], 0.0f,
                        mat3[2][0], mat3[2][1], mat3[2][2], 0.0f,
                              0.0f,       0.0f,       0.0f, 1.0f);
    }

    FVector3 FMath::ToEulerAngles(const FQuaternion& qRot)
    {
        FVector3 vEulerAngles(0, 0, 0);
        glm::extractEulerAngleYXZ(glm::toMat4(qRot), vEulerAngles.y, vEulerAngles.x, vEulerAngles.z);
        vEulerAngles.x = glm::degrees(vEulerAngles.x);
        vEulerAngles.y = glm::degrees(vEulerAngles.y);
        vEulerAngles.z = glm::degrees(vEulerAngles.z);
        return vEulerAngles;
    }
    FVector3 FMath::ToEulerAngles(const FVector3& vDir)
    {
        FVector3 vRight;
        FVector3 vUp;
        FVector3 vDirection = Normalize(vDir);
        if (vDirection == ms_v3UnitY)
        {
            vRight = ms_v3UnitX;
            vUp = ms_v3UnitNegZ;
        }
        else if (vDirection == ms_v3UnitNegY)
        {
            vRight = ms_v3UnitX;
            vUp = ms_v3UnitZ;
        }
        else
        {
            vRight = Normalize(Cross(ms_v3UnitY, vDirection));
            vUp = Normalize(Cross(vDirection, vRight));
        }
        FQuaternion qRot = ToQuaternion(vRight, vUp, vDirection);
        return ToEulerAngles(qRot);
    }
    FVector3 FMath::ToDirection(const FVector3& vEulerAngles)
    {
        FQuaternion qRot = ToQuaternion(vEulerAngles);
        FVector3 xAxis(0);
        FVector3 yAxis(0);
        FVector3 zAxis(0);
        ToAxes(qRot, xAxis, yAxis, zAxis);
        return Normalize(zAxis);
    }

    FQuaternion FMath::ToQuaternion(const FMatrix3& mat3)
    {
        return glm::toQuat(mat3);
    }
    FQuaternion FMath::ToQuaternion(const FMatrix4& mat4)
    {
        return glm::toQuat(mat4);
    }
    FQuaternion FMath::ToQuaternion(const FVector3& vEulerAngles)
    {
        FMatrix3 mat3 = glm::eulerAngleYXZ(glm::radians(vEulerAngles.y), glm::radians(vEulerAngles.x), glm::radians(vEulerAngles.z));
        return glm::toQuat(mat3);
    }
    FQuaternion FMath::ToQuaternion(const FVector3& xAxis, const FVector3& yAxis, const FVector3& zAxis)
    {
        FMatrix3 mat3 = ToMatrix3(xAxis, yAxis, zAxis);
        return ToQuaternion(mat3);
    }
    FQuaternion FMath::ToQuaternionFromAngleAxis(float fAngle, const FVector3& vAxis)
    {
        return glm::angleAxis(glm::radians(fAngle), vAxis);
    }
    FQuaternion FMath::ToQuaternionFromAngleAxis(const FQuaternion& qSrc, float fAngle, const FVector3& vAxis)
    {
        return glm::rotate(qSrc, glm::radians(fAngle), vAxis);
    }
    FQuaternion FMath::ToQuaternionFromRadianAxis(float fRadian, const FVector3& vAxis)
    {
        return glm::angleAxis(fRadian, vAxis);
    }
    FQuaternion FMath::ToQuaternionFromRadianAxis(const FQuaternion& qSrc, float fRadian, const FVector3& vAxis)
    {
        return glm::rotate(qSrc, fRadian, vAxis);
    }
    FQuaternion FMath::ToQuaternionFromSrc2Dst(const FVector3& vSrc, const FVector3& vDst, const FVector3& vFallbackAxis /*= FMath::ms_v3Zero*/)
    {
        FQuaternion q = FMath::ms_qUnit;
		FVector3 v0 = FMath::Normalize(vSrc);
		FVector3 v1 = FMath::Normalize(vDst);

		float d = FMath::Dot(v0, v1);
		if (d >= 1.0f)
		{
			return q;
		}
		if (d < (1e-6f - 1.0f))
		{
			if (vFallbackAxis != FMath::ms_v3Zero)
			{
                q = FMath::ToQuaternionFromRadianAxis(q, FMath::ms_fPI, vFallbackAxis);
			}
			else
			{
				FVector3 vAxis = FMath::Cross(FMath::ms_v3UnitX, vSrc);
				if (FMath::IsZeroLength(vAxis)) 
					vAxis = FMath::Cross(FMath::ms_v3UnitY, vSrc);
                vAxis = FMath::Normalize(vAxis);
                q = FMath::ToQuaternionFromRadianAxis(q, FMath::ms_fPI, vAxis);
			}
		}
		else
		{
			float s = FMath::Sqrt((1 + d) * 2);
			float fInv = 1.0f / s;

			FVector3 c = FMath::Cross(v0, v1);
			q.x = c.x * fInv;
			q.y = c.y * fInv;
			q.z = c.z * fInv;
			q.w = s * 0.5f;
            q = FMath::Normalize(q);
		}
		return q;
    }

    void FMath::ToAxes(const FQuaternion& qRot, FVector3& xAxis, FVector3& yAxis, FVector3& zAxis)
    {
        FMatrix3 mat3 = glm::toMat3(qRot);
        FMath::FromMatrix3(mat3, xAxis, yAxis, zAxis);
    }
    void FMath::ToAngleAxis(const FQuaternion& qRot, float& fAngle, FVector3& vAxis)
    {
        fAngle = glm::degrees(glm::angle(qRot));
        vAxis = glm::axis(qRot);
    }

    FVector3 FMath::Transform(const FMatrix4& mat4, const FVector3& v)
    {
        FVector4 vRet = mat4 * FVector4(v.x, v.y, v.z, 1.0f);
        return FVector3(vRet.x, vRet.y, vRet.z);
    }
    FVector3 FMath::Transform(const FQuaternion& qRot, const FVector3& v)
    {
        return glm::rotate(qRot, v);
    }

    bool FMath::TransformPerspective(const FMatrix4& mat4, const FVector3& vPosIn, FVector3& vPosOut)
    {
        FVector4 vRet = mat4 * FVector4(vPosIn.x, vPosIn.y, vPosIn.z, 1.0f);
        if (Abs(vRet.w) > 1.0e-7f)
        {
            float invW = 1.0f / vRet.w;
            vPosOut.x = vRet.x * invW;
            vPosOut.y = vRet.y * invW;
            vPosOut.z = vRet.z * invW;
            return true;
        }
        
        vPosOut.x = 0.0f;
        vPosOut.y = 0.0f;
        vPosOut.z = 0.0f;
        return false;
    }

    bool FMath::IsAffine(const FMatrix4& mat4)
    {
        return mat4[3][0] == 0.0f && mat4[3][1] == 0.0f && mat4[3][2] == 0.0f && mat4[3][3] == 1.0f;
    }
    FVector3 FMath::TransformAffine(const FMatrix4& mat4, const FVector3& v)
    {
        F_Assert(FMath::IsAffine(mat4) && "FMath::TransformAffine")

		return FVector3(mat4[0][0] * v.x + mat4[0][1] * v.y + mat4[0][2] * v.z + mat4[0][3], 
                        mat4[1][0] * v.x + mat4[1][1] * v.y + mat4[1][2] * v.z + mat4[1][3],
                        mat4[2][0] * v.x + mat4[2][1] * v.y + mat4[2][2] * v.z + mat4[2][3]);
    }
	FVector3 FMath::TransformAffine(const FMatrix4& mat4, const FVector4& v)
    {
        F_Assert(FMath::IsAffine(mat4) && "FMath::TransformAffine")

		return FVector4(mat4[0][0] * v.x + mat4[0][1] * v.y + mat4[0][2] * v.z + mat4[0][3] * v.w, 
                        mat4[1][0] * v.x + mat4[1][1] * v.y + mat4[1][2] * v.z + mat4[1][3] * v.w,
                        mat4[2][0] * v.x + mat4[2][1] * v.y + mat4[2][2] * v.z + mat4[2][3] * v.w,
                        v.w);
    }
    FMatrix4 FMath::AffineInverse(const FMatrix4& mat4)
    {
        return glm::affineInverse(mat4);
    }

    FQuaternion FMath::InverseQuaternion(const FQuaternion& qRot)
    {
        return glm::inverse(qRot);
    }
    FQuaternion FMath::UnitInverseQuaternion(const FQuaternion& qRot)
    {
        return FQuaternion(-qRot.x, -qRot.y, -qRot.z, qRot.w);
    }
    FMatrix3 FMath::InverseMatrix3(const FMatrix3& mat3)
    {
        return glm::inverse(mat3);
    }
    FMatrix4 FMath::InverseMatrix4(const FMatrix4& mat4)
    {
        return glm::inverse(mat4);
    }

    FMatrix3 FMath::TransposeMatrix3(const FMatrix3& mat3)
    {
        return glm::transpose(mat3);
    }
    FMatrix4 FMath::TransposeMatrix4(const FMatrix4& mat4)
    {
        return glm::transpose(mat4);
    }

    //Equation - Quadratic/Cubic/Quartic
    //ax^2 + bx + c = 0
    int FMath::Equation_Quadratic(float a, float b, float c, float& x1, float& x2)
    {
        x1 = 0.0f;
        x2 = 0.0f;
        int count = 0;

        float delta = b * b - 4 * a * c;
        if (delta < 0)
            return count;
        if (FMath::Abs(delta) < ms_fEpsilon)
        {
            x1 = x2 = -b / (2 * a);
            count = 1;
            return count;
        }

        delta = FMath::Sqrt(delta); 
        x1 = (-b - delta) / (2 * a);
        x2 = (-b + delta) / (2 * a);
        count = 2;
        return count;
    }
    int FMath::Equation_Quadratic(float a, float b, float c, FComplex& x1, FComplex& x2)
    {
        FComplex A((double)a);
        FComplex B((double)b);
        FComplex C((double)c);
        FEquationQuadratic eQuadratic(A, B, C);
        eQuadratic.Solve();  
        x1 = eQuadratic.x[0];
        x2 = eQuadratic.x[1];
        return 2;
    }

    //ax^3 + bx^2 + cx + d = 0
    int FMath::Equation_Cubic_Cardano(float a, float b, float c, float d, float& x1, float& x2, float& x3)
    {
        static const float cos120 = -0.5f;
        static const float sin120 = 0.866025404f;

        x1 = 0.0f;
        x2 = 0.0f;
        x3 = 0.0f;
        int count = 0;

        if (FMath::Abs(d) < ms_fEpsilon)
        {
            //first solution is x = 0
            x1 = 0.0f;
            ++count;
            //divide all terms by x, converting to quadratic equation
            d = c;
            c = b;
            b = a;
            a = 0.0f;
        }
        if (FMath::Abs(a) < ms_fEpsilon)
        {
            if (FMath::Abs(b) < ms_fEpsilon)
            {
                //linear equation
                if(FMath::Abs(c) > ms_fEpsilon)
                {
                    x1 = -d/c;
                    count += 1;
                }
            }
            else
            {
                //quadratic equation
                float yy = c*c - 4*b*d;
                if (yy >= 0)
                {
                    float inv2b = 1/(2*b); 
                    float y = FMath::Sqrt(yy);
                    x1 = (-c + y) * inv2b;
                    x2 = (-c - y) * inv2b;
                    count += 2;
                }
            }
        }
        else
        {
            //cubic equation
            float inva = 1/a;
            float invaa = inva*inva;
            float bb = b*b;
            float bover3a = b*(1/3.0f)*inva;
            float p = (3*a*c - bb)*(1/3.0f)*invaa;
            float halfq = (2*bb*b - 9*a*b*c + 27*a*a*d)*(0.5f/27)*invaa*inva;
            float yy = p*p*p/27 + halfq*halfq;
            if (yy > ms_fEpsilon)
            {
                // sqrt is positive: one real solution
                float y = FMath::Sqrt(yy);
                float uuu = -halfq + y;
                float vvv = -halfq - y;
                float www = FMath::Abs(uuu) > FMath::Abs(vvv) ? uuu : vvv;
                float w = (www < 0) ? -pow(FMath::Abs(www),1/3.0f) : pow(www, 1/3.0f);
                x1 = w - p/(3*w) - bover3a;
                count = 1;
            }
            else if (yy < -ms_fEpsilon)
            {
                //sqrt is negative: three real solutions
                float x = -halfq;
                float y = FMath::Sqrt(-yy);
                float theta;
                float r;
                float ux;
                float uyi;
                //convert to polar form
                if (FMath::Abs(x) > ms_fEpsilon)
                {
                    theta = (x > 0) ? atan(y/x) : (atan(y/x) + 3.14159625f);
                    r = FMath::Sqrt(x*x - yy);
                }
                else
                {
                    //vertical line
                    theta = 3.14159625f/2;
                    r = y;
                }
                //calc cube root
                theta /= 3.0f;
                r = pow(r, 1/3.0f);
                //convert to complex coordinate
                ux = cos(theta)*r;
                uyi = sin(theta)*r;
                //first solution
                x1 = ux+ux - bover3a;
                //second solution, rotate +120 degrees
                x2 = 2*(ux*cos120 - uyi*sin120) - bover3a;
                //third solution, rotate -120 degrees
                x3 = 2*(ux*cos120 + uyi*sin120) - bover3a;
                count = 3;
            }
            else
            {
                //sqrt is zero: two real solutions
                float www = -halfq;
                float w = (www < 0) ? -pow(FMath::Abs(www),1/3.0f) : pow(www,1/3.0f); 
                //first solution           
                x1 = w+w - bover3a;
                //second solution, rotate +120 degrees
                x2 = 2*w*cos120 - bover3a;
                count = 2;
            }
        }

        return count;
    }
    int FMath::Equation_Cubic_ShengJin(float a, float b, float c, float d, float& x1, float& x2, float& x3)
    {
        x1 = 0.0f;
        x2 = 0.0f;
        x3 = 0.0f;
        int count = 0;

        float A = b*b - 3*a*c;
        float B = b*c - 9*a*d;
        float C = c*c - 3*b*d;
        float f = B*B - 4*A*C;
        
        if (FMath::Abs(A) < ms_fEpsilon && FMath::Abs(B) < ms_fEpsilon)
        {
            x1 = x2 = x3 = -b / (3 * a);
            count = 3;
        }
        else if (FMath::Abs(f) < ms_fEpsilon)
        {
            float K = B/A;
            x1 = -b / a + K;
            x2 = x3 = -K / 2;
            count = 3;
        }
        else if (f > ms_fEpsilon)
        {
            float Y1 = A*b + 3*a*(-B + FMath::Sqrt(f)) / 2;
            float Y2 = A*b + 3*a*(-B - FMath::Sqrt(f)) / 2;
            float Y1_value = (Y1/FMath::Abs(Y1)) * FMath::Pow(FMath::Abs(Y1), 1.0f/3.0f);
            float Y2_value = (Y2/FMath::Abs(Y2)) * FMath::Pow(FMath::Abs(Y2), 1.0f/3.0f);
            x1 = (-b - Y1_value - Y2_value) / (3 * a);
            count = 1;
            //give up -i 
        }
        else if (f < ms_fEpsilon)
        {
            float sqrtA = FMath::Sqrt(A);
            float sqrt3 = FMath::Sqrt(3);
            
            float T = (2*A*b - 3*a*B)/(2*A*sqrtA);
            float S = acos(T);
            float sinS = sin(S/3);
            float cosS = cos(S/3);
            x1 = (-b - 2*sqrtA*cosS)/(3*a);
            x2 = (-b + sqrtA*(cosS + sqrt3*sinS))/(3*a);
            x3 = (-b + sqrtA*(cosS - sqrt3*sinS))/(3*a);
            count = 3;
        }

        return count;
    }
    int FMath::Equation_Cubic(float a, float b, float c, float d, FComplex& x1, FComplex& x2, FComplex& x3)
    {
        FComplex A((double)a);
        FComplex B((double)b);
        FComplex C((double)c);
        FComplex D((double)d);
        FEquationCubic eCubic(A, B, C, D);
        eCubic.Solve();  
        x1 = eCubic.x[0];
        x2 = eCubic.x[1];
        x3 = eCubic.x[2];
        return 3;
    }

    //ax^4 + bx^3 + cx^2 + dx + e = 0
    int FMath::Equation_Quartic(float a, float b, float c, float d, float e, float& x1, float& x2, float& x3, float& x4, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FComplex xx1,xx2,xx3,xx4;
        Equation_Quartic(a, b, c, d, e, xx1, xx2, xx3, xx4, fEpsilon);
        std::vector<float> aTs;
        if (FMath::Abs(xx1.imaginary) < ms_fEpsilon)
        {
            aTs.push_back((float)xx1.real);
        }
        if (FMath::Abs(xx2.imaginary) < ms_fEpsilon)
        {
            aTs.push_back((float)xx2.real);
        }
        if (FMath::Abs(xx3.imaginary) < ms_fEpsilon)
        {
            aTs.push_back((float)xx3.real);
        }
        if (FMath::Abs(xx4.imaginary) < ms_fEpsilon)
        {
            aTs.push_back((float)xx4.real);
        }

        std::sort(aTs.begin(), aTs.end(), float_less);
        int count = (int)aTs.size();

        x1 = 0.0f;
        x2 = 0.0f;
        x3 = 0.0f;
        x4 = 0.0f;
        
        if (count > 3)
        {
            x1 = aTs[0];
            x2 = aTs[1];
            x3 = aTs[2];
            x4 = aTs[3];
        }
        else if (count > 2)
        {
            x1 = aTs[0];
            x2 = aTs[1];
            x3 = aTs[2];
        }
        else if (count > 1)
        {
            x1 = aTs[0];
            x2 = aTs[1];
        }
        else if (count > 0)
        {
            x1 = aTs[0];
        } 

        return count;
    }
    int FMath::Equation_Quartic(float a, float b, float c, float d, float e, FComplex& x1, FComplex& x2, FComplex& x3, FComplex& x4, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FComplex A((double)a);
        FComplex B((double)b);
        FComplex C((double)c);
        FComplex D((double)d);
        FComplex E((double)e);
        FEquationQuartic eQuartic(A, B, C, D, E, fEpsilon);
        eQuartic.Solve();
        x1 = eQuartic.x[0];
        x2 = eQuartic.x[1];
        x3 = eQuartic.x[2];
        x4 = eQuartic.x[3];
        return 4;
    }

    //Direction From Point2
    FVector3 FMath::GetDirectionWithoutNormalizeFromPoint2(const FVector3& v1, const FVector3& v2)
    {
        return v2 - v1;
    }
    FVector3 FMath::GetDirectionFromPoint2(const FVector3& v1, const FVector3& v2)
    {
        return Normalize(v2 - v1);
    }

    //Normal From Point3
    FVector3 FMath::GetNormal3WithoutNormalizeFromPoints3(const FVector3& v1, const FVector3& v2, const FVector3& v3)
    {
        return FMath::Cross(v2 - v1, v3 - v1);
    }
    FVector4 FMath::GetNormal4WithoutNormalizeFromPoints3(const FVector3& v1, const FVector3& v2, const FVector3& v3)
    {
        FVector3 normal = GetNormal3WithoutNormalizeFromPoints3(v1, v2, v3);
		return FVector4(normal.x, normal.y, normal.z, -(FMath::Dot(normal, v1)));
    }

    FVector3 FMath::GetNormal3FromPoints3(const FVector3& v1, const FVector3& v2, const FVector3& v3)
    {
        return FMath::Normalize(FMath::Cross(v2 - v1, v3 - v1));
    }
    FVector4 FMath::GetNormal4FromPoints3(const FVector3& v1, const FVector3& v2, const FVector3& v3)
    {
        FVector3 normal = GetNormal3FromPoints3(v1, v2, v3);
		return FVector4(normal.x, normal.y, normal.z, -(FMath::Dot(normal, v1)));
    }

    //Angle Cos From Direction2/Plane2
    float FMath::GetAngleCosFromDirection2(const FVector3& v1, const FVector3& v2)
    {   
        return Dot(v1, v2) / Length(v1 - v2);
    }
    float FMath::GetAngleCosFromSegment2(const FSegment& s1, const FSegment& s2)
    {
        return GetAngleCosFromDirection2(s1.GetDirection(), s2.GetDirection());
    }
    float FMath::GetAngleCosFromPlane2(const FVector3& p11, const FVector3& p12, const FVector3& p13, const FVector3& p21, const FVector3& p22, const FVector3& p23)
    {
        FVector3 vNormal1 = GetNormal3WithoutNormalizeFromPoints3(p11, p12, p13);
        FVector3 vNormal2 = GetNormal3WithoutNormalizeFromPoints3(p21, p22, p23);
        return Dot(vNormal1, vNormal2) / (Length(vNormal1) * Length(vNormal2));
    }

    //Angle Sin From Line-Plane
    float FMath::GetAngleSinFromLinePlane(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3)
    {
        FVector3 v12 = ptLine2 - ptLine1;
        FVector3 vNormal = GetNormal3WithoutNormalizeFromPoints3(pt1, pt2, pt3);
        return Dot(v12, vNormal) / (Length(v12) * Length(vNormal));
    }
    float FMath::GetAngleSinFromLinePlane(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3)
    {
        return GetAngleSinFromLinePlane(segment.m_pt0, segment.m_pt1, pt1, pt2, pt3);
    }

    //Distance From Point-Line
    float FMath::GetDistanceFromPointLine(const FVector3& pt, const FVector3& ptLine11, const FVector3& ptLine12)
    {
        FVector3 v12 = ptLine12 - ptLine11;
        FVector3 vL1P =  pt - ptLine11;
        return Length(Cross(vL1P, v12)) / Distance(ptLine11, ptLine12);
    }
    float FMath::GetDistanceFromPointLine(const FVector3& pt, const FSegment& segment)
    {
        return GetDistanceFromPointLine(pt, segment.m_pt0, segment.m_pt1);
    }

    //Distance From Point-Plane
    float FMath::GetDistanceFromPointPlane(const FVector3& pt, const FVector3& planePoint, const FVector3& planeNormal)
    {
        return Abs(Dot(planeNormal, (pt - planePoint))) / Length(planeNormal);
    }
    float FMath::GetDistanceFromPointPlane(const FVector3& pt, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3)
    {
        FVector3 vNormal = GetNormal3WithoutNormalizeFromPoints3(pt1, pt2, pt3);
        return GetDistanceFromPointPlane(pt, pt1, vNormal);
    }
    float FMath::GetDistanceFromPointPlane(const FVector3& pt, const FPlane& plane)
    {
        return GetDistanceFromPointPlane(pt, plane.GetPlanePoint(), plane.GetNormal());
    }

    //Distance From Line2
    float FMath::GetDistanceFromLine2(const FVector3& ptLine11, const FVector3& ptLine12, const FVector3& ptLine21, const FVector3& ptLine22)
    {
        FVector3 v1 = ptLine12 - ptLine11;
        FVector3 v2 = ptLine22 - ptLine21;
        FVector3 vN = Cross(v1, v2);
        return Abs(Dot((ptLine11 - ptLine21), vN)) / Length(vN);
    }
    float FMath::GetDistanceFromLine2(const FSegment& segment1, const FSegment& segment2)
    {
        return GetDistanceFromLine2(segment1.m_pt0, segment1.m_pt1, segment2.m_pt0, segment2.m_pt1);
    }

    //Distance From Ray-AABB-Axis
    bool FMath::GetDistanceFromRayAABBAxis(const FVector3& rayOrig, const FVector3& rayDir, const FVector3& min, const FVector3& max, int32 nAxis, float& fStart, float& fEnd)
    {
        float denom = 1 / rayDir[nAxis];
        float fNewStart = (min[nAxis] - rayOrig[nAxis]) * denom;	
        float fNewEnd = (max[nAxis] - rayOrig[nAxis]) * denom;
        if (fNewStart > fNewEnd) 
            std::swap(fNewStart, fNewEnd);		
        if (fNewStart > fEnd || fNewEnd < fStart) 
            return false;		
        if (fNewStart > fStart) 
            fStart = fNewStart;					
        if (fNewEnd < fEnd) 
            fEnd = fNewEnd;
        return true;
    }
    bool FMath::GetDistanceFromRayAABBAxis(const FRay& ray, const FAABB& aabb, int32 nAxis, float& fStart, float& fEnd)
    {
        return GetDistanceFromRayAABBAxis(ray.GetOrigin(), ray.GetDirection(), aabb.GetMin(), aabb.GetMax(), nAxis, fStart, fEnd);
    }

    //Intersection Point Perpendicular To Line/Plane
    void FMath::GetIntersectionPointPerpendicularToLine(const FVector3& pt, const FVector3& ptLine1, const FVector3& ptLine2, FVector3& vIntersection)
    {
        FVector3 v12 = Normalize(ptLine2 - ptLine1);
        FVector3 vDir = pt - ptLine1;
        vIntersection = ptLine1 + v12 * Dot(vDir, v12);
    }
    void FMath::GetIntersectionPointPerpendicularToPlane(const FVector3& pt, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, FVector3& vIntersection)
    {
        FVector3 vNormal = GetNormal3FromPoints3(pt1, pt2, pt3);
        FVector3 vDir = pt - pt1;
        vIntersection = pt - vNormal * Dot(vNormal, vDir);
    }

    //Intersection Point From Line2
    bool FMath::GetIntersectionPointFromLine2(const FVector3& ptLine11, const FVector3& ptLine12, const FVector3& ptLine21, const FVector3& ptLine22, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        vIntersection = FMath::ms_v3Zero;
        if (LineLine_NotIntersect(ptLine11, ptLine12, ptLine21, ptLine22, true, fEpsilon))
            return false;
        
        //method 1:
        // float d1 = GetDistanceFromPointLine(ptLine11, ptLine21, ptLine22);
        // float d2 = GetDistanceFromPointLine(ptLine12, ptLine21, ptLine22);
        // vIntersection = ptLine11;
        // float t = d1 / (d1 + d2);

        //method 2: Length(Cross(ptLine21 - ptLine11, ptLine22 - ptLine21)) / Length(Cross(ptLine12 - ptLine11, ptLine22 - ptLine21))
        float t = ((ptLine21.x - ptLine11.x)*(ptLine22.y - ptLine21.y) - (ptLine21.y - ptLine11.y)*(ptLine22.x - ptLine21.x)) / 
                  ((ptLine12.x - ptLine11.x)*(ptLine22.y - ptLine21.y) - (ptLine12.y - ptLine11.y)*(ptLine22.x - ptLine21.x));
        vIntersection.x += (ptLine12.x - ptLine11.x) * t;
        vIntersection.y += (ptLine12.y - ptLine11.y) * t;
        vIntersection.z += (ptLine12.z - ptLine11.z) * t;

        return true;
    }
    bool FMath::GetIntersectionPointFromLine2(const FSegment& segment1, const FSegment& segment2, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return GetIntersectionPointFromLine2(segment1.m_pt0, segment1.m_pt1, segment2.m_pt0, segment2.m_pt1, vIntersection, fEpsilon);
    }

    //Intersection Point From Line-Plane
    bool FMath::GetIntersectionPointFromLineTriangle(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        vIntersection = FMath::ms_v3Zero;
        if (LineTriangle_NotIntersect(ptLine1, ptLine2, pt1, pt2, pt3, true, fEpsilon))
            return false;

        FVector3 vNormal = GetNormal3WithoutNormalizeFromPoints3(pt1, pt2, pt3);
        //t = Dot(pt1 - ptLine1, vNormal)/Dot(ptLine2 - ptLine1, vNormal)
        float t = (vNormal.x * (pt1.x - ptLine1.x) + vNormal.y * (pt1.y - ptLine1.y) + vNormal.z * (pt1.z - ptLine1.z)) /
                  (vNormal.x * (ptLine2.x - ptLine1.x) + vNormal.y * (ptLine2.y - ptLine1.y) + vNormal.z * (ptLine2.z - ptLine1.z));
        vIntersection.x = ptLine1.x + (ptLine2.x - ptLine1.x) * t;
        vIntersection.y = ptLine1.y + (ptLine2.y - ptLine1.y) * t;
        vIntersection.z = ptLine1.z + (ptLine2.z - ptLine1.z) * t;
        
        return true;
    }
    bool FMath::GetIntersectionPointFromLineTriangle(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return GetIntersectionPointFromLineTriangle(segment.m_pt0, segment.m_pt1, pt1, pt2, pt3, vIntersection, fEpsilon);
    }

    bool FMath::GetIntersectionPointFromLineQuad(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, const FVector3& pt4, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        vIntersection = FMath::ms_v3Zero;
        if (!Points4_OnPlane(pt1, pt2, pt3, pt4, fEpsilon))
            return false;

        FVector3 vConcave; 
        FVector3 aV[4] = 
        {
            pt1, pt2, pt3, pt4
        };
        int nIndex = 0;
        if (Quad_IsConcave(pt1, pt2, pt3, pt4, vConcave, nIndex, fEpsilon))
        {
            FVector3& t11 = aV[nIndex%4]; nIndex++;
            FVector3& t12 = aV[nIndex%4]; nIndex++;
            FVector3& t13 = aV[nIndex%4]; 

            FVector3& t21 = aV[nIndex%4]; nIndex++;
            FVector3& t22 = aV[nIndex%4]; nIndex++;
            FVector3& t23 = aV[nIndex%4]; 

            if (GetIntersectionPointFromLineTriangle(ptLine1, ptLine2, t11, t12, t13, vIntersection, fEpsilon) ||
                GetIntersectionPointFromLineTriangle(ptLine1, ptLine2, t21, t22, t23, vIntersection, fEpsilon))
            {
                return true;
            }
        }   
        else
        {
            FVector3 vOri = ptLine1;
            FVector3 vDir = Normalize(ptLine2 - ptLine1);
            FVector3 vOA = pt1 - vOri;
            FVector3 vOB = pt2 - vOri;
            FVector3 vOC = pt3 - vOri;
            FVector3 vCross = FMath::Cross(vOC, vDir);
            float fDotV = FMath::Dot(vOA, vCross);
            if (fDotV >= 0.0f)
            {
                return GetIntersectionPointFromLineTriangle(ptLine1, ptLine2, pt1, pt2, pt3, vIntersection, fEpsilon);
            }
            else
            {
                return GetIntersectionPointFromLineTriangle(ptLine1, ptLine2, pt4, pt1, pt3, vIntersection, fEpsilon);
            }
        }
        return false;
    }
    bool FMath::GetIntersectionPointFromLineQuad(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, const FVector3& pt4, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return GetIntersectionPointFromLineQuad(segment.m_pt0, segment.m_pt1, pt1, pt2, pt3, pt4, vIntersection, fEpsilon);
    }

    bool FMath::GetIntersectionPointFromLinePlane(const FVector3& ptLine1, const FVector3& ptLine2, const FPlane& plane, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        vIntersection = FMath::ms_v3Zero;
        FVector3 vDir = ptLine2 - ptLine1;
        FVector3 vNormal = plane.GetNormal();
        if (Direction_IsPerpendicular(vDir, vNormal, fEpsilon))
            return false;

        float denom = FMath::Dot(vNormal, vDir);
        float nom = FMath::Dot(vNormal, ptLine1) + plane.GetDistance();
        float t = -(nom / denom);
        if (t >= 0)
        {
            vIntersection = ptLine1 + vDir * t;
            return true;
        }
        return false;
    }
    bool FMath::GetIntersectionPointFromLinePlane(const FSegment& segment, const FPlane& plane, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return GetIntersectionPointFromLinePlane(segment.m_pt0, segment.m_pt1, plane, vIntersection, fEpsilon);
    }

    //Intersection Point From Ray-Line/Triangle/Quad/Plane
    bool FMath::GetIntersectionPointFromRayLine(const FRay& ray, const FVector3& pt1, const FVector3& pt2, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 ptLine2 = ray.GetDirection() * ms_fRayFar;
        return GetIntersectionPointFromLine2(ray.GetOrigin(), ptLine2, pt1, pt2, vIntersection, fEpsilon);
    }
    bool FMath::GetIntersectionPointFromRayLine(const FRay& ray, const FSegment& segment, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return GetIntersectionPointFromRayLine(ray, segment.m_pt0, segment.m_pt1, vIntersection, fEpsilon);
    }
    bool FMath::GetIntersectionPointFromRayTriangle(const FRay& ray, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 ptLine2 = ray.GetDirection() * ms_fRayFar;
        return GetIntersectionPointFromLineTriangle(ray.GetOrigin(), ptLine2, pt1, pt2, pt3, vIntersection, fEpsilon);
    }
    bool FMath::GetIntersectionPointFromRayTriangle(const FRay& ray, const FTriangle& triangle, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return GetIntersectionPointFromRayTriangle(ray, triangle.m_pt0, triangle.m_pt1, triangle.m_pt2, vIntersection, fEpsilon);
    }
    bool FMath::GetIntersectionPointFromRayQuad(const FRay& ray, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, const FVector3& pt4, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 ptLine2 = ray.GetDirection() * ms_fRayFar;
        return GetIntersectionPointFromLineQuad(ray.GetOrigin(), ptLine2, pt1, pt2, pt3, pt4, vIntersection, fEpsilon);
    }
    bool FMath::GetIntersectionPointFromRayQuad(const FRay& ray, const FQuad& quad, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return GetIntersectionPointFromRayQuad(ray, quad.m_pt0, quad.m_pt1, quad.m_pt2, quad.m_pt3, vIntersection, fEpsilon);
    }
    bool FMath::GetIntersectionPointFromRayPlane(const FRay& ray, const FPlane& plane, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 ptLine2 = ray.GetDirection() * ms_fRayFar;
        return GetIntersectionPointFromLinePlane(ray.GetOrigin(), ptLine2, plane, vIntersection, fEpsilon);
    }
    bool FMath::GetIntersectionPointFromRayCircle(const FRay& ray, const FCircle& circle, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {   
        FPlane plane(circle.GetNormal(), circle.GetDistance());
        if (!GetIntersectionPointFromRayPlane(ray, plane, vIntersection, fEpsilon))
        {
            return false;
        }

        float radius = circle.GetRadius(); 
        if (FMath::Distance2(circle.GetCenter(), vIntersection) <= radius * radius)
        {
            return true;
        }
        return false;
    }
    bool FMath::GetIntersectionPointFromRayAABB(const FRay& ray, const FAABB& aabb, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return false;
    }
    bool FMath::GetIntersectionPointFromRaySphere(const FRay& ray, const FSphere& sphere, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return false;
    }

    //Intersection Line From Plane-Plane
    bool FMath::GetIntersectionLineFromPlane2(const FVector3& pP11, const FVector3& pP12, const FVector3& pP13, const FVector3& pP21, const FVector3& pP22, const FVector3& pP23, FVector3& pL1, FVector3& pL2, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        pL1 = FMath::ms_v3Zero;
        pL2 = FMath::ms_v3Zero;
        if (PlanePlane_Parallel(pP11, pP12, pP13, pP21, pP22, pP23, fEpsilon))
            return false;

        if (LinePlane_Parallel(pP21, pP22, pP11, pP12, pP13, fEpsilon))
        {
            if (!GetIntersectionPointFromLineTriangle(pP22, pP23, pP11, pP12, pP13, pL1, fEpsilon))
            {
                return false;
            }
        }
        else
        {
            if (!GetIntersectionPointFromLineTriangle(pP21, pP22, pP11, pP12, pP13, pL1, fEpsilon))
            {
                return false;
            }
        }

        if (LinePlane_Parallel(pP23, pP21, pP11, pP12, pP13))
        {
            if (!GetIntersectionPointFromLineTriangle(pP22, pP23, pP11, pP12, pP13, pL2, fEpsilon))
            {
                return false;
            }
        }
        else
        {
            if (!GetIntersectionPointFromLineTriangle(pP23, pP21, pP11, pP12, pP13, pL2, fEpsilon))
            {
                return false;
            }
        }

        return true;
    }
    bool FMath::GetIntersectionLineFromPlane2(const FVector3& pP11, const FVector3& pP12, const FVector3& pP13, const FVector3& pP21, const FVector3& pP22, const FVector3& pP23, FSegment& segment, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return GetIntersectionLineFromPlane2(pP11, pP12, pP13, pP21, pP22, pP23, segment.m_pt0, segment.m_pt1, fEpsilon);
    }

    //Radius From AABB
    float FMath::GetRadiusFromAABB(const FAABB& aabb)
    {
        const FVector3& max = aabb.GetMax();
        const FVector3& min = aabb.GetMin();

        FVector3 magnitude = max;
        FMath::MakeCeil(magnitude, -max);
        FMath::MakeCeil(magnitude,  min);
        FMath::MakeCeil(magnitude, -min);
        
        return FMath::Length(magnitude);
    }
    
    //Direction - IsParallel/IsPerpendicular
    bool FMath::Direction_IsParallel(const FVector3& vDir1, const FVector3& vDir2, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        float fLen = Length(Cross(vDir1, vDir2));
        if (fLen < fEpsilon)
            return true;
        return false;
    }
    bool FMath::Direction_IsPerpendicular(const FVector3& vDir1, const FVector3& vDir2, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        float fDot = Dot(vDir1, vDir2);
        if (Zero(fDot, fEpsilon))
            return true;
        return false;
    }
    bool FMath::Direction_IsSameDirection(const FVector3& vDir1, const FVector3& vDir2)
    {
        if (Dot(vDir1, vDir2) >= 0)
            return true;
        return false;
    }

    //Point - InPlaneNormalSide
    bool FMath::Point_InPlaneNormalSide(const FVector3& pt, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3)
    {
        FVector3 vNormal = GetNormal3WithoutNormalizeFromPoints3(pt1, pt2, pt3);
        FVector3 vDir = pt1 - pt;
        if (!Direction_IsSameDirection(vNormal, vDir))
            return true;
        return false;
    }
    bool FMath::Point_InPlaneNormalSide(const FVector3& pt, const FPlane& plane)
    {
        FVector3 vNormal = Normalize(plane.GetNormal());
        FVector3 ptPlane = vNormal * plane.GetDistance();
        FVector3 vDir = ptPlane - pt;
        if (!Direction_IsSameDirection(vNormal, vDir))
            return true;
        return false;
    }

    //Point - InLine
    bool FMath::Points3_InLine(const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 v21 = pt2 - pt1;
        FVector3 v31 = pt3 - pt1;
        FVector3 vCross = Cross(v21, v31);
        if (Zero(Abs(Length(Cross(v21, v31))), fEpsilon))
            return true;
        return false;
    }

    //Point - InLineSameSide/NotInLineSameSide
    bool FMath::Points2_InLineSameSide(const FVector3& pt1, const FVector3& pt2, const FVector3& ptLine1, const FVector3& ptLine2, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vLine12 = ptLine2 - ptLine1;
        FVector3 vLine1Pt1 = pt1 - ptLine1;
        FVector3 vLine1Pt2 = pt2 - ptLine1;
        float fDot = Dot(Cross(vLine12, vLine1Pt1), Cross(vLine12, vLine1Pt2));
        if (fDot > fEpsilon)
            return true;
        return false;
    }
    bool FMath::Points2_InLineSameSide(const FVector3& pt1, const FVector3& pt2, const FSegment& segment, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return Points2_InLineSameSide(pt1, pt2, segment.m_pt0, segment.m_pt1, fEpsilon);
    }
    bool FMath::Points2_NotInLineSameSide(const FVector3& pt1, const FVector3& pt2, const FVector3& ptLine1, const FVector3& ptLine2, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vLine12 = ptLine2 - ptLine1;
        FVector3 vLine1Pt1 = pt1 - ptLine1;
        FVector3 vLine1Pt2 = pt2 - ptLine1;
        float fDot = Dot(Cross(vLine12, vLine1Pt1), Cross(vLine12, vLine1Pt2));
        if (fDot < -fEpsilon)
            return true;
        return false;
    }
    bool FMath::Points2_NotInLineSameSide(const FVector3& pt1, const FVector3& pt2, const FSegment& segment, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return Points2_NotInLineSameSide(pt1, pt2, segment.m_pt0, segment.m_pt1, fEpsilon);
    }

    //Point - OnPlane
    bool FMath::Points4_OnPlane(const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, const FVector3& pt4, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vNormal = GetNormal3FromPoints3(pt1, pt2, pt3);
        FVector3 v14 = Normalize(pt4 - pt1);
        float fDot = Dot(vNormal, v14);
        if (Zero(fDot, fEpsilon))
            return true;
        return false;
    }

    //Point - InPlaneSameSide/NotInPlaneSameSide
    bool FMath::Points2_InPlaneSameSide(const FVector3& pt1, const FVector3& pt2, const FVector3& ptPlane, const FVector3& vPlaneNormal, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 v1 = pt1 - ptPlane;
        FVector3 v2 = pt2 - ptPlane;
        float fDot = Dot(vPlaneNormal, v1) * Dot(vPlaneNormal, v2);
        if (fDot > fEpsilon)
            return true;
        return false;
    }
    bool FMath::Points2_InPlaneSameSide(const FVector3& pt1, const FVector3& pt2, const FVector3& ptPlane1, const FVector3& ptPlane2, const FVector3& ptPlane3, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vPlaneNormal = GetNormal3WithoutNormalizeFromPoints3(ptPlane1, ptPlane2, ptPlane3);
        return Points2_InPlaneSameSide(pt1, pt2, ptPlane1, vPlaneNormal, fEpsilon);
    }
    bool FMath::Points2_InPlaneSameSide(const FVector3& pt1, const FVector3& pt2, const FPlane& plane, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vNormal = FMath::Normalize(plane.GetNormal());
        FVector3 ptPlane = vNormal * plane.GetDistance();
        return Points2_InPlaneSameSide(pt1, pt2, ptPlane, vNormal, fEpsilon);
    }
    bool FMath::Points2_NotInPlaneSameSide(const FVector3& pt1, const FVector3& pt2, const FVector3& ptPlane, const FVector3& vPlaneNormal, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 v1 = pt1 - ptPlane;
        FVector3 v2 = pt2 - ptPlane;
        float fDot = Dot(vPlaneNormal, v1) * Dot(vPlaneNormal, v2);
        if (fDot < -fEpsilon)
            return true;
        return false;
    }
    bool FMath::Points2_NotInPlaneSameSide(const FVector3& pt1, const FVector3& pt2, const FVector3& ptPlane1, const FVector3& ptPlane2, const FVector3& ptPlane3, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vPlaneNormal = GetNormal3WithoutNormalizeFromPoints3(ptPlane1, ptPlane2, ptPlane3);
        return Points2_InPlaneSameSide(pt1, pt2, ptPlane1, vPlaneNormal, fEpsilon);
    }
    bool FMath::Points2_NotInPlaneSameSide(const FVector3& pt1, const FVector3& pt2, const FPlane& plane, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vNormal = FMath::Normalize(plane.GetNormal());
        FVector3 ptPlane = vNormal * plane.GetDistance();
        return Points2_NotInPlaneSameSide(pt1, pt2, ptPlane, vNormal, fEpsilon);
    }

    //Line - Line Parallel/NotParallel
    bool FMath::LineLine_Parallel(const FVector3& ptL_11, const FVector3& ptL_12, const FVector3& ptL_21, const FVector3& ptL_22, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 v1 = ptL_12 - ptL_11;
        FVector3 v2 = ptL_22 - ptL_21;
        if (Direction_IsParallel(v1, v2, fEpsilon))
            return true;
        return false;
    }
    bool FMath::LineLine_Parallel(const FVector3& ptL_11, const FVector3& ptL_12, const FSegment& segment, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return LineLine_Parallel(ptL_11, ptL_12, segment.m_pt0, segment.m_pt1, fEpsilon);
    }
    bool FMath::LineLine_Parallel(const FSegment& segment1, const FSegment& segment2, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return LineLine_Parallel(segment1.m_pt0, segment1.m_pt1, segment2.m_pt0, segment2.m_pt1, fEpsilon);
    }
    bool FMath::LineLine_NotParallel(const FVector3& ptL_11, const FVector3& ptL_12, const FVector3& ptL_21, const FVector3& ptL_22, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return !LineLine_Parallel(ptL_11, ptL_12, ptL_21, ptL_22, fEpsilon);
    }
    bool FMath::LineLine_NotParallel(const FVector3& ptL_11, const FVector3& ptL_12, const FSegment& segment, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return LineLine_NotParallel(ptL_11, ptL_12, segment.m_pt0, segment.m_pt1, fEpsilon);
    }
    bool FMath::LineLine_NotParallel(const FSegment& segment1, const FSegment& segment2, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return LineLine_NotParallel(segment1.m_pt0, segment1.m_pt1, segment2.m_pt0, segment2.m_pt1, fEpsilon);
    }

    //Line - Line Perpendicular/NotPerpendicular
    bool FMath::LineLine_Perpendicular(const FVector3& ptL_11, const FVector3& ptL_12, const FVector3& ptL_21, const FVector3& ptL_22, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 v1 = ptL_12 - ptL_11;
        FVector3 v2 = ptL_22 - ptL_21;
        if (Direction_IsPerpendicular(v1, v2, fEpsilon))
            return true;
        return false;
    }
    bool FMath::LineLine_Perpendicular(const FVector3& ptL_11, const FVector3& ptL_12, const FSegment& segment, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return LineLine_Perpendicular(ptL_11, ptL_12, segment.m_pt0, segment.m_pt1, fEpsilon);
    }
    bool FMath::LineLine_Perpendicular(const FSegment& segment1, const FSegment& segment2, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return LineLine_Perpendicular(segment1.m_pt0, segment1.m_pt1, segment2.m_pt0, segment2.m_pt1, fEpsilon);
    }
    bool FMath::LineLine_NotPerpendicular(const FVector3& ptL_11, const FVector3& ptL_12, const FVector3& ptL_21, const FVector3& ptL_22, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return !LineLine_Perpendicular(ptL_11, ptL_12, ptL_21, ptL_22, fEpsilon);
    }
    bool FMath::LineLine_NotPerpendicular(const FVector3& ptL_11, const FVector3& ptL_12, const FSegment& segment, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return LineLine_NotPerpendicular(ptL_11, ptL_12, segment.m_pt0, segment.m_pt1, fEpsilon); 
    }
    bool FMath::LineLine_NotPerpendicular(const FSegment& segment1, const FSegment& segment2, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return LineLine_NotPerpendicular(segment1.m_pt0, segment1.m_pt1, segment2.m_pt0, segment2.m_pt1, fEpsilon);
    }

    //Line - Line Intersect/NotIntersect
    bool FMath::LineLine_Intersect(const FVector3& ptL_11, const FVector3& ptL_12, const FVector3& ptL_21, const FVector3& ptL_22, bool includeBorder /*= true*/, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        if (!Points4_OnPlane(ptL_11, ptL_12, ptL_21, ptL_22, fEpsilon))
            return false;

        bool isIntersect = false;
        if (includeBorder)
        {
            if (!Points3_InLine(ptL_11, ptL_12, ptL_21, fEpsilon) || !Points3_InLine(ptL_11, ptL_12, ptL_22, fEpsilon))
                return !Points2_InLineSameSide(ptL_11, ptL_12, ptL_21, ptL_22, fEpsilon) && !Points2_InLineSameSide(ptL_21, ptL_22, ptL_11, ptL_12, fEpsilon);

            isIntersect = Intersects_PointInLine(ptL_11, ptL_21, ptL_22, fEpsilon) || 
                          Intersects_PointInLine(ptL_12, ptL_21, ptL_22, fEpsilon) || 
                          Intersects_PointInLine(ptL_21, ptL_11, ptL_12, fEpsilon) || 
                          Intersects_PointInLine(ptL_22, ptL_11, ptL_12, fEpsilon);
        }
        else
        {
            isIntersect = Points2_NotInLineSameSide(ptL_11, ptL_12, ptL_21, ptL_22, fEpsilon) &&
                          Points2_NotInLineSameSide(ptL_21, ptL_22, ptL_11, ptL_12, fEpsilon);
        }
        return isIntersect;
    }
    bool FMath::LineLine_Intersect(const FVector3& ptL_11, const FVector3& ptL_12, const FSegment& segment, bool includeBorder /*= true*/, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return LineLine_Intersect(ptL_11, ptL_12, segment.m_pt0, segment.m_pt1, includeBorder, fEpsilon);
    }
    bool FMath::LineLine_Intersect(const FSegment& segment1, const FSegment& segment2, bool includeBorder /*= true*/, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return LineLine_Intersect(segment1.m_pt0, segment1.m_pt1, segment2.m_pt0, segment2.m_pt1, includeBorder, fEpsilon);
    }
    bool FMath::LineLine_NotIntersect(const FVector3& ptL_11, const FVector3& ptL_12, const FVector3& ptL_21, const FVector3& ptL_22, bool includeBorder /*= true*/, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return !LineLine_Intersect(ptL_11, ptL_12, ptL_21, ptL_22, includeBorder, fEpsilon);
    }
    bool FMath::LineLine_NotIntersect(const FVector3& ptL_11, const FVector3& ptL_12, const FSegment& segment, bool includeBorder /*= true*/, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return LineLine_NotIntersect(ptL_11, ptL_12, segment.m_pt0, segment.m_pt1, includeBorder, fEpsilon); 
    }
    bool FMath::LineLine_NotIntersect(const FSegment& segment1, const FSegment& segment2, bool includeBorder /*= true*/, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return LineLine_NotIntersect(segment1.m_pt0, segment1.m_pt1, segment2.m_pt0, segment2.m_pt1, includeBorder, fEpsilon);
    }

    //Line - Plane Parallel/NotParallel
    bool FMath::LinePlane_Parallel(const FVector3& vDir, const FVector3& vNormal, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        if (Direction_IsParallel(vDir, vNormal, fEpsilon))
            return true;
        return false;
    }
    bool FMath::LinePlane_Parallel(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vDir = ptLine2 - ptLine1;
        FVector3 vNormal = GetNormal3WithoutNormalizeFromPoints3(pt1, pt2, pt3);
        return LinePlane_Parallel(vDir, vNormal, fEpsilon);
    }
    bool FMath::LinePlane_Parallel(const FVector3& ptLine1, const FVector3& ptLine2, const FPlane& plane, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vDir = ptLine2 - ptLine1;
        return LinePlane_Parallel(vDir, plane.GetNormal(), fEpsilon);
    }
    bool FMath::LinePlane_Parallel(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return LinePlane_Parallel(segment.m_pt0, segment.m_pt1, pt1, pt2, pt3, fEpsilon);
    }
    bool FMath::LinePlane_Parallel(const FSegment& segment, const FPlane& plane, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return LinePlane_Parallel(segment.GetDirection(), plane.GetNormal(), fEpsilon);
    }
    bool FMath::LinePlane_NotParallel(const FVector3& vDir, const FVector3& vNormal, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return !LinePlane_Parallel(vDir, vNormal, fEpsilon);
    }
    bool FMath::LinePlane_NotParallel(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return !LinePlane_Parallel(ptLine1, ptLine2, pt1, pt2, pt3, fEpsilon);
    }
    bool FMath::LinePlane_NotParallel(const FVector3& ptLine1, const FVector3& ptLine2, const FPlane& plane, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vDir = ptLine2 - ptLine1;
        return LinePlane_NotParallel(vDir, plane.GetNormal(), fEpsilon);
    }
    bool FMath::LinePlane_NotParallel(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return LinePlane_NotParallel(segment.m_pt0, segment.m_pt1, pt1, pt2, pt3, fEpsilon);
    }
    bool FMath::LinePlane_NotParallel(const FSegment& segment, const FPlane& plane, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return LinePlane_NotParallel(segment.GetDirection(), plane.GetNormal(), fEpsilon);
    }

    //Line - Plane Perpendicular/NotPerpendicular
    bool FMath::LinePlane_Perpendicular(const FVector3& vDir, const FVector3& vNormal, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        if (Direction_IsPerpendicular(vDir, vNormal, fEpsilon))
            return true;
        return false;
    }
    bool FMath::LinePlane_Perpendicular(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vDir = ptLine2 - ptLine1;
        FVector3 vNormal = GetNormal3WithoutNormalizeFromPoints3(pt1, pt2, pt3);
        return LinePlane_Perpendicular(vDir, vNormal, fEpsilon);
    }
    bool FMath::LinePlane_Perpendicular(const FVector3& ptLine1, const FVector3& ptLine2, const FPlane& plane, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vDir = ptLine2 - ptLine1;
        return LinePlane_Perpendicular(vDir, plane.GetNormal(), fEpsilon);
    }
    bool FMath::LinePlane_Perpendicular(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return LinePlane_Perpendicular(segment.m_pt0, segment.m_pt1, pt1, pt2, pt3, fEpsilon);
    }
    bool FMath::LinePlane_Perpendicular(const FSegment& segment, const FPlane& plane, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return LinePlane_Perpendicular(segment.GetDirection(), plane.GetNormal(), fEpsilon);
    }
    bool FMath::LinePlane_NotPerpendicular(const FVector3& vDir, const FVector3& vNormal, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return !LinePlane_Perpendicular(vDir, vNormal, fEpsilon);
    }
    bool FMath::LinePlane_NotPerpendicular(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return !LinePlane_NotPerpendicular(ptLine1, ptLine2, pt1, pt2, pt3, fEpsilon);
    }
    bool FMath::LinePlane_NotPerpendicular(const FVector3& ptLine1, const FVector3& ptLine2, const FPlane& plane, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vDir = ptLine2 - ptLine1;
        return LinePlane_NotPerpendicular(vDir, plane.GetNormal(), fEpsilon);
    }
    bool FMath::LinePlane_NotPerpendicular(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return LinePlane_NotPerpendicular(segment.m_pt0, segment.m_pt1, pt1, pt2, pt3, fEpsilon);
    }
    bool FMath::LinePlane_NotPerpendicular(const FSegment& segment, const FPlane& plane, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return LinePlane_NotPerpendicular(segment.GetDirection(), plane.GetNormal(), fEpsilon);
    }

    //Line - Triangle Intersect/NotIntersect
    bool FMath::LineTriangle_Intersect(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, bool includeBorder /*= true*/, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        if (includeBorder)
        {
            if (!Points2_InPlaneSameSide(ptLine1, ptLine2, pt1, pt2, pt3, fEpsilon) &&
                !Points2_InPlaneSameSide(pt1, pt2, ptLine1, ptLine2, pt3, fEpsilon) &&
                !Points2_InPlaneSameSide(pt2, pt3, ptLine1, ptLine2, pt1, fEpsilon) &&
                !Points2_InPlaneSameSide(pt3, pt1, ptLine1, ptLine2, pt2, fEpsilon))
            {
                return true; 
            }
        }
        else
        {
            if (!Points2_NotInPlaneSameSide(ptLine1, ptLine2, pt1, pt2, pt3, fEpsilon) &&
                !Points2_NotInPlaneSameSide(pt1, pt2, ptLine1, ptLine2, pt3, fEpsilon) &&
                !Points2_NotInPlaneSameSide(pt2, pt3, ptLine1, ptLine2, pt1, fEpsilon) &&
                !Points2_NotInPlaneSameSide(pt3, pt1, ptLine1, ptLine2, pt2, fEpsilon))
            {
                return true; 
            }
        }
        return false;
    }
    bool FMath::LineTriangle_Intersect(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, bool includeBorder /*= true*/, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return LineTriangle_Intersect(segment.m_pt0, segment.m_pt1, pt1, pt2, pt3, includeBorder, fEpsilon);
    }
    bool FMath::LineTriangle_NotIntersect(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, bool includeBorder /*= true*/, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return !LineTriangle_Intersect(ptLine1, ptLine2, pt1, pt2, pt3, includeBorder, fEpsilon);
    }
    bool FMath::LineTriangle_NotIntersect(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, bool includeBorder /*= true*/, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return LineTriangle_NotIntersect(segment.m_pt0, segment.m_pt1, pt1, pt2, pt3, includeBorder, fEpsilon);
    }

    //Plane - Plane Parallel/NotParallel
    bool FMath::PlanePlane_Parallel(const FVector3& pP11, const FVector3& pP12, const FVector3& pP13, const FVector3& pP21, const FVector3& pP22, const FVector3& pP23, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vNormal1 = GetNormal3WithoutNormalizeFromPoints3(pP11, pP12, pP13);
        FVector3 vNormal2 = GetNormal3WithoutNormalizeFromPoints3(pP21, pP22, pP23);
        return Direction_IsParallel(vNormal1, vNormal2, fEpsilon);
    }
    bool FMath::PlanePlane_NotParallel(const FVector3& pP11, const FVector3& pP12, const FVector3& pP13, const FVector3& pP21, const FVector3& pP22, const FVector3& pP23, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return !PlanePlane_Parallel(pP11, pP12, pP13, pP21, pP22, pP23, fEpsilon);
    }
    bool FMath::PlanePlane_Parallel(const FPlane& plane1, const FPlane& plane2, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return Direction_IsParallel(plane1.GetNormal(), plane2.GetNormal(), fEpsilon);
    }
    bool FMath::PlanePlane_NotParallel(const FPlane& plane1, const FPlane& plane2, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return !PlanePlane_Parallel(plane1, plane2, fEpsilon);
    }

    //Plane - Plane Perpendicular/NotPerpendicular
    bool FMath::PlanePlane_Perpendicular(const FVector3& pP11, const FVector3& pP12, const FVector3& pP13, const FVector3& pP21, const FVector3& pP22, const FVector3& pP23, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vNormal1 = GetNormal3WithoutNormalizeFromPoints3(pP11, pP12, pP13);
        FVector3 vNormal2 = GetNormal3WithoutNormalizeFromPoints3(pP21, pP22, pP23);
        return Direction_IsPerpendicular(vNormal1, vNormal2, fEpsilon);
    }
    bool FMath::PlanePlane_NotPerpendicular(const FVector3& pP11, const FVector3& pP12, const FVector3& pP13, const FVector3& pP21, const FVector3& pP22, const FVector3& pP23, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return !PlanePlane_Perpendicular(pP11, pP12, pP13, pP21, pP22, pP23, fEpsilon);
    }
    bool FMath::PlanePlane_Perpendicular(const FPlane& plane1, const FPlane& plane2, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return Direction_IsPerpendicular(plane1.GetNormal(), plane2.GetNormal(), fEpsilon);
    }
    bool FMath::PlanePlane_NotPerpendicular(const FPlane& plane1, const FPlane& plane2, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return !PlanePlane_Perpendicular(plane1, plane2, fEpsilon);
    }

    //Quad - Convex
    bool FMath::Quad_IsConvex(const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, const FVector3& pt4, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        if (!Points4_OnPlane(pt1, pt2, pt3, pt4, fEpsilon))
            return false;

        if (!Intersects_PointInTriangle(pt1, pt2, pt3, pt4, false, fEpsilon) && //1 - 234
            !Intersects_PointInTriangle(pt2, pt1, pt3, pt4, false, fEpsilon) && //2 - 134
            !Intersects_PointInTriangle(pt3, pt1, pt2, pt4, false, fEpsilon) && //3 - 124
            !Intersects_PointInTriangle(pt4, pt1, pt2, pt3, false, fEpsilon)) //4 - 123
        {
            return true;
        }
        return false;
    }
    bool FMath::Quad_IsConvex(const FQuad& quad, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return Quad_IsConvex(quad.m_pt0, quad.m_pt1, quad.m_pt2, quad.m_pt3, fEpsilon); 
    }
    bool FMath::Quad_IsConcave(const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, const FVector3& pt4, FVector3& vConcave, int& nIndex, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        vConcave = pt1;
        nIndex = 0;
        if (!Points4_OnPlane(pt1, pt2, pt3, pt4, fEpsilon))
            return false;

        bool isPIT_1_234 = Intersects_PointInTriangle(pt1, pt2, pt3, pt4, false, fEpsilon); //1 - 234
        bool isPIT_2_134 = Intersects_PointInTriangle(pt2, pt1, pt3, pt4, false, fEpsilon); //2 - 134
        bool isPIT_3_124 = Intersects_PointInTriangle(pt3, pt1, pt2, pt4, false, fEpsilon); //3 - 124
        bool isPIT_4_123 = Intersects_PointInTriangle(pt4, pt1, pt2, pt3, false, fEpsilon); //4 - 123
        if (isPIT_1_234 && !isPIT_2_134 && !isPIT_3_124 && !isPIT_4_123)
        {
            vConcave = pt1;
            nIndex = 0;
            return true;
        }
        if (!isPIT_1_234 && isPIT_2_134 && !isPIT_3_124 && !isPIT_4_123)
        {
            vConcave = pt2;
            nIndex = 1;
            return true;
        }
        if (!isPIT_1_234 && !isPIT_2_134 && isPIT_3_124 && !isPIT_4_123)
        {
            vConcave = pt3;
            nIndex = 2;
            return true;
        }
        if (!isPIT_1_234 && !isPIT_2_134 && !isPIT_3_124 && isPIT_4_123)
        {
            vConcave = pt4;
            nIndex = 3;
            return true;
        }   
        return false;
    }
    bool FMath::Quad_IsConcave(const FVector3* pPt, FVector3& vConcave, int& nIndex, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return Quad_IsConcave(pPt[0], pPt[1], pPt[2], pPt[3], vConcave, nIndex, fEpsilon);
    }
    bool FMath::Quad_IsConcave(const FQuad& quad, FVector3& vConcave, int& nIndex, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return Quad_IsConcave(quad.m_pt0, quad.m_pt1, quad.m_pt2, quad.m_pt3, vConcave, nIndex, fEpsilon);
    }

    //Point - Line
    bool FMath::Intersects_PointInLine(const FVector3& pt, const FVector3& ptLineStart, const FVector3& ptLineEnd, bool includeSE /*= true*/, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        //Is In Line
        if (!Points3_InLine(ptLineStart, ptLineEnd, pt, fEpsilon))
        {
            return false;
        }

        if (!includeSE)
        {
            //Except ptLineStart/ptLineEnd
            if (IsEqual(pt, ptLineStart, fEpsilon) || IsEqual(pt, ptLineEnd, fEpsilon))
            {
                return false;
            }
        }

        //pt is between ptLineStart and ptLineEnd
        if (Zero((ptLineStart.x - pt.x)*(ptLineEnd.x - pt.x), fEpsilon) &&
            Zero((ptLineStart.y - pt.y)*(ptLineEnd.y - pt.y), fEpsilon) &&
            Zero((ptLineStart.z - pt.z)*(ptLineEnd.z - pt.z), fEpsilon))
        {
            return true;
        }

        return false;
    }
    bool FMath::Intersects_PointInLine(const FVector3& pt, const FSegment& segment, bool includeSE /*= true*/, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return Intersects_PointInLine(pt, segment.m_pt0, segment.m_pt1, includeSE, fEpsilon);
    }

    //Point - Triangle
    bool FMath::Intersects_PointInTriangle(const FVector3& pt, const FVector3& a, const FVector3& b, const FVector3& c, bool includeBorder /*= true*/, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vAB = b - a;
        FVector3 vAC = c - a;
        FVector3 vPA = a - pt;
        FVector3 vPB = b - pt;
        FVector3 vPC = c - pt;
        if (!includeBorder)
        {
            if (Points3_InLine(pt, a, b, fEpsilon) ||
                Points3_InLine(pt, b, c, fEpsilon) ||
                Points3_InLine(pt, c, a, fEpsilon))
            {
                return false;
            }
        }
        float fValue = Length(Cross(vAB, vAC)) - Length(Cross(vPA, vPB)) - Length(Cross(vPB, vPC)) - Length(Cross(vPC, vPA));
        if (Zero(fValue, fEpsilon))
            return true;
        return false;
    }

    //Point - Rect
    bool FMath::Intersects_PointInRect2DI(const FPointI& pt, const FPointI& a, const FPointI& b, const FPointI& c, const FPointI& d)
    {
        FVector3 pt3(pt.x, pt.y, 0);
        FVector3 a3(a.x, a.y, 0);
        FVector3 b3(b.x, b.y, 0);
        FVector3 c3(c.x, c.y, 0);
        FVector3 d3(d.x, d.y, 0);
        return Intersects_PointInRect(pt3, a3, b3, c3, d3);
    }
    bool FMath::Intersects_PointInRect2DF(const FPointF& pt, const FPointF& a, const FPointF& b, const FPointF& c, const FPointF& d)
    {
        FVector3 pt3(pt.x, pt.y, 0);
        FVector3 a3(a.x, a.y, 0);
        FVector3 b3(b.x, b.y, 0);
        FVector3 c3(c.x, c.y, 0);
        FVector3 d3(d.x, d.y, 0);
        return Intersects_PointInRect(pt3, a3, b3, c3, d3);
    }
    bool FMath::Intersects_PointInRect(const FVector3& pt, const FVector3& a, const FVector3& b, const FVector3& c, const FVector3& d, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        if (!Points4_OnPlane(a, b, c, d))
            return false;

        FVector3 vConcave; 
        FVector3 aV[4] = 
        {
            a, b, c, d
        };
        int nIndex = 0;
        if (Quad_IsConcave(a, b, c, d, vConcave, nIndex, fEpsilon))
        {
            FVector3& t11 = aV[nIndex%4]; nIndex++;
            FVector3& t12 = aV[nIndex%4]; nIndex++;
            FVector3& t13 = aV[nIndex%4]; 

            FVector3& t21 = aV[nIndex%4]; nIndex++;
            FVector3& t22 = aV[nIndex%4]; nIndex++;
            FVector3& t23 = aV[nIndex%4]; 

            if (Intersects_PointInTriangle(pt, t11, t12, t13, true, fEpsilon) ||
                Intersects_PointInTriangle(pt, t21, t22, t23, true, fEpsilon))
            {
                return true;
            }
        }   
        else
        {
            if (Intersects_PointInTriangle(pt, a, b, c, true, fEpsilon) ||
                Intersects_PointInTriangle(pt, a, c, d, true, fEpsilon))
            {
                return true;
            }
        }

        return false;
    }

    //Point - Circle
    bool FMath::Intersects_PointInCircle(const FVector3& pt, const FVector3& center, float radius)
    {
        
        return false;
    }

    //Ray - Shape
    std::pair<bool, float>FMath::Intersects_RaySegment(const FRay& ray, const FVector3& s, const FVector3& e, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        if (GetIntersectionPointFromRayLine(ray, s, e, vIntersection, fEpsilon))
        {
            return std::pair<bool, float>(true, ray.GetDistance(vIntersection));
        }
        return std::pair<bool, float>(false, 0);
    }
    std::pair<bool, float> FMath::Intersects_RaySegment(const FRay& ray, const FSegment& segment, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return Intersects_RaySegment(ray, segment.m_pt0, segment.m_pt1, vIntersection, fEpsilon);
    }

    std::pair<bool, float> FMath::Intersects_RayTriangle(const FRay& ray, const FVector3& a, const FVector3& b, const FVector3& c, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        if (GetIntersectionPointFromRayTriangle(ray, a, b, c, vIntersection, fEpsilon))
        {
            return std::pair<bool, float>(true, ray.GetDistance(vIntersection));
        }
        return std::pair<bool, float>(false, 0);
    }
    std::pair<bool, float> FMath::Intersects_RayTriangle(const FRay& ray, const FTriangle& triangle, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return Intersects_RayTriangle(ray, triangle.m_pt0, triangle.m_pt1, triangle.m_pt2, vIntersection, fEpsilon);
    }

    std::pair<bool, float> FMath::Intersects_RayQuad(const FRay& ray, const FVector3& a, const FVector3& b, const FVector3& c, const FVector3& d, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        if (GetIntersectionPointFromRayQuad(ray, a, b, c, d, vIntersection, fEpsilon))
        {
            return std::pair<bool, float>(true, ray.GetDistance(vIntersection));
        }
        return std::pair<bool, float>(false, 0);
    }
    std::pair<bool, float> FMath::Intersects_RayQuad(const FRay& ray, const FQuad& quad, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return Intersects_RayQuad(ray, quad.m_pt0, quad.m_pt1, quad.m_pt2, quad.m_pt3, vIntersection, fEpsilon);
    }

    std::pair<bool, float> FMath::Intersects_RayPlane(const FRay& ray, const FPlane& plane, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        if (GetIntersectionPointFromRayPlane(ray, plane, vIntersection, fEpsilon))
        {
            return std::pair<bool, float>(true, ray.GetDistance(vIntersection));
        }   
        return std::pair<bool, float>(false, 0);
    }

    std::pair<bool, float> FMath::Intersects_RayPlaneVector(const FRay& ray, const FPlaneVector& aPlanes, bool normalIsOutside)
    {
        FPlaneList listPlanes;
        for (FPlaneVector::const_iterator it = aPlanes.begin(); 
             it != aPlanes.end(); ++it)
        {
            listPlanes.push_back(*it);
        }
        return Intersects_RayPlaneList(ray, listPlanes, normalIsOutside);
    }

    std::pair<bool, float> FMath::Intersects_RayPlaneList(const FRay& ray, FPlaneList& listPlanes, bool normalIsOutside)
    {
        bool allInside = true;
        std::pair<bool, float> ret;
        std::pair<bool, float> end;
        ret.first = false;
        ret.second = 0.0f;
        end.first = false;
        end.second = 0;

        FPlaneSideType ePlaneSide = normalIsOutside ? F_PlaneSide_Positive : F_PlaneSide_Negative;
        for (FPlaneList::iterator it = listPlanes.begin();
             it != listPlanes.end(); ++it)
        {
            const FPlane& plane = *it;
            if (plane.GetSide(ray.GetOrigin()) == ePlaneSide)
            {
                allInside = false;
                std::pair<bool, float> planeRes = ray.Intersects_Plane(plane);
                if (planeRes.first)
                {
                    ret.first = true;
                    ret.second = FMath::Max<float>(ret.second, planeRes.second);
                }
                else
                {
                    ret.first =false;
                    ret.second = 0.0f;
                    return ret;
                }
            }
            else
            {
                std::pair<bool, float> planeRes = ray.Intersects_Plane(plane);
                if (planeRes.first)
                {
                    if (!end.first)
                    {
                        end.first = true;
                        end.second = planeRes.second;
                    }
                    else
                    {
                        end.second = FMath::Min<float>(planeRes.second, end.second);
                    }
                }
            }
        }

        if (allInside)
        {
            ret.first = true;
            ret.second = 0.0f;
            return ret;
        }

        if (end.first)
        {
            if(end.second < ret.second)
            {
                ret.first = false;
                return ret;
            }
        }
        return ret;
    }

    std::pair<bool, float> FMath::Intersects_RayCircle(const FRay& ray, const FPlane& plane, const FVector3& center, float radius, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FCircle circle(plane.GetNormal(), plane.GetDistance(), center, radius);
        return Intersects_RayCircle(ray, circle, vIntersection, fEpsilon);
    }
    std::pair<bool, float> FMath::Intersects_RayCircle(const FRay& ray, const FCircle& circle, FVector3& vIntersection, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        if (GetIntersectionPointFromRayCircle(ray, circle, vIntersection, fEpsilon))
        {
            return std::pair<bool, float>(true, ray.GetDistance(vIntersection));
        }  
        return std::pair<bool, float>(false, 0);
    }

    std::pair<bool, float> FMath::Intersects_RayAABB(const FRay& ray, const FAABB& aabb)
    {
        if (!aabb.IsValid()) 
            return std::pair<bool, float>(false, 0);

        const FVector3& min = aabb.GetMin();
        const FVector3& max = aabb.GetMax();
        const FVector3& rayOrig = ray.GetOrigin();
        const FVector3& rayDir = ray.GetDirection();

        bool isHit = false;
        float tLow = 0.0f;
        float t;
        FVector3 vHit;

        //1> Check origin inside first
        if (FMath::IsGreat(rayOrig, min) &&  FMath::IsLess(rayOrig, max))
        {
            return std::pair<bool, float>(true, 0);
        }

        //2> Check each face in turn, only check closest 3
        //Min x
        if (rayOrig.x <= min.x && rayDir.x > 0)
        {
            t = (min.x - rayOrig.x) / rayDir.x;
            if (t >= 0)
            {
                vHit = rayOrig + rayDir * t;
                if (vHit.y >= min.y && vHit.y <= max.y &&
                    vHit.z >= min.z && vHit.z <= max.z &&
                    (!isHit || t < tLow))
                {
                    isHit = true;
                    tLow = t;
                }
            }
        }
        //Max x
        if (rayOrig.x >= max.x && rayDir.x < 0)
        {
            t = (max.x - rayOrig.x) / rayDir.x;
            if (t >= 0)
            {
                vHit = rayOrig + rayDir * t;
                if (vHit.y >= min.y && vHit.y <= max.y &&
                    vHit.z >= min.z && vHit.z <= max.z &&
                    (!isHit || t < tLow))
                {
                    isHit = true;
                    tLow = t;
                }
            }
        }

        //Min y
        if (rayOrig.y <= min.y && rayDir.y > 0)
        {
            t = (min.y - rayOrig.y) / rayDir.y;
            if (t >= 0)
            {
                vHit = rayOrig + rayDir * t;
                if (vHit.x >= min.x && vHit.x <= max.x &&
                    vHit.z >= min.z && vHit.z <= max.z &&
                    (!isHit || t < tLow))
                {
                    isHit = true;
                    tLow = t;
                }
            }
        }
        //Max y
        if (rayOrig.y >= max.y && rayDir.y < 0)
        {
            t = (max.y - rayOrig.y) / rayDir.y;
            if (t >= 0)
            {
                vHit = rayOrig + rayDir * t;
                if (vHit.x >= min.x && vHit.x <= max.x &&
                    vHit.z >= min.z && vHit.z <= max.z &&
                    (!isHit || t < tLow))
                {
                    isHit = true;
                    tLow = t;
                }
            }
        }

        //Min z
        if (rayOrig.z <= min.z && rayDir.z > 0)
        {
            t = (min.z - rayOrig.z) / rayDir.z;
            if (t >= 0)
            {
                vHit = rayOrig + rayDir * t;
                if (vHit.x >= min.x && vHit.x <= max.x &&
                    vHit.y >= min.y && vHit.y <= max.y &&
                    (!isHit || t < tLow))
                {
                    isHit = true;
                    tLow = t;
                }
            }
        }
        //Max z
        if (rayOrig.z >= max.z && rayDir.z < 0)
        {
            t = (max.z - rayOrig.z) / rayDir.z;
            if (t >= 0)
            {
                vHit = rayOrig + rayDir * t;
                if (vHit.x >= min.x && vHit.x <= max.x &&
                    vHit.y >= min.y && vHit.y <= max.y &&
                    (!isHit || t < tLow))
                {
                    isHit = true;
                    tLow = t;
                }
            }
        }

        return std::pair<bool, float>(isHit, tLow);
    }

    bool FMath::Intersects_RayAABB(const FRay& ray, const FAABB& aabb, float* d1, float* d2)
    {
         if (!aabb.IsValid())
            return false;

        const FVector3& min = aabb.GetMin();
        const FVector3& max = aabb.GetMax();
        const FVector3& rayOrig = ray.GetOrigin();
        const FVector3& rayDir = ray.GetDirection();
        FVector3 rayDirAbs = FMath::Abs(rayDir);

        //1> Sort the axis, ensure check minimize floating error axis first
        int32 nMax = 0, nMid = 1, nMin = 2;
        if (rayDirAbs[0] < rayDirAbs[2])
        {
            nMax = 2;
            nMin = 0;
        }
        if (rayDirAbs[1] < rayDirAbs[nMin])
        {
            nMid = nMin;
            nMin = 1;
        }
        else if (rayDirAbs[1] > rayDirAbs[nMax])
        {
            nMid = nMax;
            nMax = 1;
        }

        //2> Check each axis in turn
        float fStart = 0;
        float fEnd = FMath::ms_fPosInfinity;
        if (!GetDistanceFromRayAABBAxis(rayOrig,
                                        rayDir,
                                        min,
                                        max,
                                        nMax,
                                        fStart,
                                        fEnd))
        {
            return false;
        }

        if (rayDirAbs[nMid] < FMath::ms_fEpsilon)
        {
            //Parallel with middle and minimize axis, check bounds only
            if (rayOrig[nMid] < min[nMid] || rayOrig[nMid] > max[nMid] ||
                rayOrig[nMin] < min[nMin] || rayOrig[nMin] > max[nMin])
            {
                return false;
            }
        }
        else
        {
            if (!GetDistanceFromRayAABBAxis(rayOrig,
                                            rayDir,
                                            min,
                                            max,
                                            nMid,
                                            fStart,
                                            fEnd))
            {
                return false;
            }

            if (rayDirAbs[nMin] < FMath::ms_fEpsilon)
            {
                //Parallel with minimize axis, check bounds only
                if (rayOrig[nMin] < min[nMin] || rayOrig[nMin] > max[nMin])
                {
                    return false;
                }
            }
            else
            {
                if (!GetDistanceFromRayAABBAxis(rayOrig,
                                                rayDir,
                                                min,
                                                max,
                                                nMin,
                                                fStart,
                                                fEnd))
                {
                    return false;
                }
            }
        }

        if (d1) *d1 = fStart;
        if (d2) *d2 = fEnd;
        return true;
    }
    bool FMath::Intersects_RayAABB(const FRay& ray, const FAABB& aabb, FVector3& vIntersection1, FVector3& vIntersection2)
    { 
        float fStart, fEnd;
        if (Intersects_RayAABB(ray, aabb, &fStart, &fEnd))
        {
            vIntersection1 = ray.GetPoint(fStart);
            vIntersection2 = ray.GetPoint(fEnd);
            return true;
        }
        return false;
    }

    std::pair<bool, float> FMath::Intersects_RaySphere(const FRay& ray, const FSphere& sphere, bool discardInside /*= true*/)		
    {
        const FVector3& sphereCenter = sphere.GetCenter();
        float sphereRadius = sphere.GetRadius();
        const FVector3& rayPos = ray.GetOrigin();
        const FVector3& rayDir = ray.GetDirection();
        const FVector3& rayOrig = rayPos - sphereCenter;

        //1> rayPos In Sphere
        float disRay2Center = FMath::Length2(rayOrig);
        if (discardInside && disRay2Center <= sphereRadius * sphereRadius)
        {
            return std::pair<bool, float>(true, 0);
        }

        //2> t = (-b +/- sqrt(b*b + 4ac)) / 2a
        float a = FMath::Dot(rayDir, rayDir);
        float b = 2 * FMath::Dot(rayOrig, rayDir);
        float c = FMath::Dot(rayOrig, rayOrig) - sphereRadius * sphereRadius;

        float d = b*b - 4*a*c;
        if (d < 0)
        {
            return std::pair<bool, float>(false, 0);
        }
        
        float t = (-b - FMath::Sqrt(d)) / (2*a);
        if (t < 0)
            t = (-b + FMath::Sqrt(d)) / (2*a);
        return std::pair<bool, float>(true, t);
    }
    int FMath::Intersects_RaySphere(const FRay& ray, const FSphere& sphere, bool& isInside, float* d1, float* d2)
    {
        *d1 = 0.0f;
        *d2 = 0.0f;

        const FVector3& sphereCenter = sphere.GetCenter();
        float sphereRadius = sphere.GetRadius();
        const FVector3& rayPos = ray.GetOrigin();
        const FVector3& rayDir = ray.GetDirection();
        const FVector3& rayOrig = rayPos - sphereCenter;

        //1> t = (-b +/- sqrt(b * b + 4ac)) / 2a
        float a = FMath::Dot(rayDir, rayDir);
        float b = 2 * FMath::Dot(rayOrig, rayDir);
        float c = FMath::Dot(rayOrig, rayOrig) - sphereRadius * sphereRadius;

        int count = 0;
        float d = b*b - 4*a*c;
        if (d < 0)
        {
            return count;
        }
        
        float t1 = (-b - FMath::Sqrt(d)) / (2*a);
        if (t1 >= 0)
        {
            count ++;
            *d1 = t1;
        }
        float t2 = (-b + FMath::Sqrt(d)) / (2*a);
        if (t2 >= 0)
        {
            count ++;
            *d2 = t2;
        }
        return count;
    }
    int FMath::Intersects_RaySphere(const FRay& ray, const FSphere& sphere, bool& isInside, FVector3& vIntersection1, FVector3& vIntersection2)
    {
        float fStart, fEnd;
        int nNum = Intersects_RaySphere(ray, sphere, isInside, &fStart, &fEnd);
        if (nNum > 0)
        {
            vIntersection1 = ray.GetPoint(fStart);
            vIntersection2 = ray.GetPoint(fEnd);
        }
        else
        {
            vIntersection1 = FMath::ms_v3Zero;
            vIntersection2 = FMath::ms_v3Zero;
        }
        return nNum;
    }

    std::pair<bool, float> FMath::Intersects_RayCylinder(const FRay& ray, const FCylinder& cylinder, bool discardInside /*= true*/)
    {
        float t = 0;
        const FVector3& rayPos = ray.GetOrigin();
        //1> rayPos In Cylinder
        if (discardInside && cylinder.Intersects_Point(rayPos))
        {
            return std::pair<bool, float>(true, 0);
        }
        
        //Ray far point
        FVector3 rayPosFar = ray.GetPoint(ms_fRayFar);

        const FVector3& vCenterTop = cylinder.GetCenterTop();
        const FVector3& vCenterBottom = cylinder.GetCenterBottom();
        float fRadius = cylinder.GetRadius();
        float fHeight = FMath::Distance(vCenterTop, vCenterBottom);

        FVector3 d = vCenterTop - vCenterBottom;
		FVector3 m = rayPos - vCenterBottom;
        FVector3 n = rayPosFar - rayPos;
        float md = FMath::Dot(m, d);
        float nd = FMath::Dot(n, d);
        float dd = FMath::Dot(d, d);

        //2> Test if segment rayPos-rayPosFar fully outside either endcap of cylinder
        if (md < 0.0f && md + nd < 0.0f) //outside 'rayPos' side of cylinder
            return std::pair<bool, float>(false, 0);
        if (md > dd && md + nd > dd) //outside 'rayPosFar' side of cylinder
            return std::pair<bool, float>(false, 0);

        float nn = FMath::Dot(n, n);
        float mn = FMath::Dot(m, n);
        float a = dd * nn - nd * nd;
        float k = FMath::Dot(m, m) - fRadius * fRadius;
        float c = dd * k - md * md;
        //segment rayPos-rayPosFar parallel to cylinder axis
        if (FMath::Abs(a) < FMath::ms_fEpsilon)
        {
            //out of r range
            if (c > 0.0f)
            {
                return std::pair<bool, float>(false, 0);
            }

            //segment rayPos-rayPosFar intersect cylinder
            if (md < 0.0f) //intersect segment against 'rayPos' endcap
                t = - mn / nn; 
            else if (md > dd) //intersect segment against 'rayPosFar' endcap
                t = (nd - mn) / nn; 
            else //lies inside cylinder
                t = 0.0f; 
            return std::pair<bool, float>(true, t * FMath::Sqrt(nn));
        }

        float b = dd * mn - nd * md;
        float discr = b * b - a * c;
        if (discr < 0.0f) //no real roots; no intersection
            return std::pair<bool, float>(false, 0); 

        t = (-b - FMath::Sqrt(discr)) / a;
        if (t < 0.0f || t > 1.0f) //intersection lies outside segment
            return std::pair<bool, float>(false, 0); 

        if (md + t * nd < 0.0f)
        {
            //intersection outside cylinder on 'rayPos' side
            if (nd <= 0.0f) //segment pointing away from endcap
                return std::pair<bool, float>(false, 0); 
            
            t = -md / nd;

            //keep intersection if Dot(L(t) - P, L(t) - P) <= r^2
            bool isInter = k + 2 * t * mn + t * t * nn <= 0.0f;
            return std::pair<bool, float>(isInter, t * FMath::Sqrt(nn)); 
        }
        else if (md + t * nd > dd)
        {
            //intersection outside cylinder on 'rayPosFar' side
            if (nd >= 0.0f) //segment pointing away from endcap
                return std::pair<bool, float>(false, 0);
            
            t = (dd - md) / nd;

            //keep intersection if Dot(L(t) - Q, L(t) - Q) <= r^2
            bool isInter = k + dd - 2 * md + 2 * t * (mn - nd) + t * t * nn <= 0.0f;
            return std::pair<bool, float>(isInter, t * FMath::Sqrt(nn));
        }

        //segment intersects cylinder between endcaps; t is correct
        return std::pair<bool, float>(true, t * FMath::Sqrt(nn));
    }
    int FMath::Intersects_RayCylinder(const FRay& ray, const FCylinder& cylinder, bool& isInside, float* d1, float* d2)
    {
        *d1 = 0;
        *d2 = 0;
        bool hasD1 = false;
        int count = 0;

        const FVector3& rayPos = ray.GetOrigin();  
        FVector3 rayPosFar = ray.GetPoint(ms_fRayFar); //Ray far point

        const FVector3& vCenterTop = cylinder.GetCenterTop();
        const FVector3& vCenterBottom = cylinder.GetCenterBottom();
        float fRadius = cylinder.GetRadius();
        float fHeight = FMath::Distance(vCenterTop, vCenterBottom);

        FVector3 d = vCenterTop - vCenterBottom;
		FVector3 m = rayPos - vCenterBottom;
        FVector3 n = rayPosFar - rayPos;
        float md = FMath::Dot(m, d);
        float nd = FMath::Dot(n, d);
        float dd = FMath::Dot(d, d);

        //1> Test if segment rayPos-rayPosFar fully outside either endcap of cylinder
        if (md < 0.0f && md + nd < 0.0f) //outside 'rayPos' side of cylinder
            return count;
        if (md > dd && md + nd > dd) //outside 'rayPosFar' side of cylinder
            return count;

        float nn = FMath::Dot(n, n);
        float mn = FMath::Dot(m, n);
        float a = dd * nn - nd * nd;
        float k = FMath::Dot(m, m) - fRadius * fRadius;
        float c = dd * k - md * md;
        //segment rayPos-rayPosFar parallel to cylinder axis
        if (FMath::Abs(a) < FMath::ms_fEpsilon)
        {
            //out of r range
            if (c > 0.0f)
            {
                return count;
            }

            //segment rayPos-rayPosFar intersect cylinder
            if (md < 0.0f) //intersect segment against 'rayPos' endcap
            {
                float nl = FMath::Sqrt(nn);
                *d1 = (-mn / nn) * nl;
                count ++;

                *d2 = *d1 + fHeight;
                if (*d2 <= nl)
                {
                    count ++;
                }
                else
                {
                    *d2 = 0.0f;
                }
                return count;
            }
            else if (md > dd) //intersect segment against 'rayPosFar' endcap
            {
                float nl = FMath::Sqrt(nn);
                *d1 = ((nd - mn) / nn) * nl; 
                count ++;

                *d2 = *d1 + fHeight;
                if (*d2 <= nl)
                {
                    count ++;
                }
                else
                {
                    *d2 = 0.0f;
                }
                return count;
            }
            else //lies inside cylinder
            {
                *d1 = 0.0f;
                *d2 = 0.0f;
                return count;
            }
        }

        float b = dd * mn - nd * md;
        float discr = b * b - a * c;
        if (discr < 0.0f) //no real roots; no intersection
        {
            return count;
        }

        //2> t1
        {
            float t1 = (-b - FMath::Sqrt(discr)) / a;
            if (t1 < 0.0f || t1 > 1.0f) //intersection lies outside segment
                return count;

            bool isInter = false;
            if (md + t1 * nd < 0.0f)
            {
                //intersection outside cylinder on 'rayPos' side
                if (nd <= 0.0f) //segment pointing away from endcap
                {
                    return count;
                }
                
                t1 = -md / nd;

                //keep intersection if Dot(L(t) - P, L(t) - P) <= r^2
                isInter = k + 2 * t1 * mn + t1 * t1 * nn <= 0.0f;
            }
            else if (md + t1 * nd > dd)
            {
                //intersection outside cylinder on 'rayPosFar' side
                if (nd >= 0.0f) //segment pointing away from endcap
                {
                    return count;
                }
                
                t1 = (dd - md) / nd;

                //keep intersection if Dot(L(t) - Q, L(t) - Q) <= r^2
                isInter = k + dd - 2 * md +  2 * t1 * (mn - nd) + t1 * t1 * nn <= 0.0f;
            }
            else
            {
                //segment intersects cylinder between endcaps; t is correct
                isInter = true;
            }

            if (isInter)
            {
                hasD1 = true;
                *d1 = t1 * FMath::Sqrt(nn);
                count ++;   
            }
        }
        
        //3> t2
        {
            float t2 = (-b + FMath::Sqrt(discr)) / a;
            if (t2 < 0.0f || t2 > 1.0f) //intersection lies outside segment
                return count;

            bool isInter = false;
            if (md + t2 * nd < 0.0f)
            {
                //intersection outside cylinder on 'rayPos' side
                if (nd <= 0.0f) //segment pointing away from endcap
                {
                    return count;
                }
                
                t2 = -md / nd;

                //keep intersection if Dot(L(t) - P, L(t) - P) <= r^2
                isInter = k + 2 * t2 * mn + t2 * t2 * nn <= 0.0f;
            }
            else if (md + t2 * nd > dd)
            {
                //intersection outside cylinder on 'rayPosFar' side
                if (nd >= 0.0f) //segment pointing away from endcap
                {
                    return count;
                }
                
                t2 = (dd - md) / nd;

                //keep intersection if Dot(L(t) - Q, L(t) - Q) <= r^2
                isInter = k + dd - 2 * md + 2 * t2 * (mn - nd) + t2 * t2 * nn <= 0.0f;
            }
            else
            {
                //segment intersects cylinder between endcaps; t is correct
                isInter = true;
            }

            if (isInter)
            {
                if (hasD1)
                {
                    *d2 = t2 * FMath::Sqrt(nn);
                    count ++;
                }
                else
                {
                    *d1 = t2 * FMath::Sqrt(nn);
                    count ++;
                }
            }
        }

        return count;
    }
    int FMath::Intersects_RayCylinder(const FRay& ray, const FCylinder& cylinder, bool& isInside, FVector3& vIntersection1, FVector3& vIntersection2)
    {   
        float fStart, fEnd;
        int nNum = Intersects_RayCylinder(ray, cylinder, isInside, &fStart, &fEnd);
        if (nNum > 0)
        {
            vIntersection1 = ray.GetPoint(fStart);
            vIntersection2 = ray.GetPoint(fEnd);
        }
        else
        {
            vIntersection1 = FMath::ms_v3Zero;
            vIntersection2 = FMath::ms_v3Zero;
        }
        return nNum;
    }
    
    std::pair<bool, float> FMath::Intersects_RayCapsule(const FRay& ray, const FCapsule& capsule, bool discardInside /*= true*/)
    {
        float t = 0;
        const FVector3& rayPos = ray.GetOrigin();
        //1> rayPos In Capsule
        if (discardInside && capsule.Intersects_Point(rayPos))
        {
            return std::pair<bool, float>(true, 0);
        }
        
        //Ray far point
        FVector3 rayPosFar = ray.GetPoint(ms_fRayFar);

        const FVector3& vCenterTop = capsule.GetCenterTop();
        const FVector3& vCenterBottom = capsule.GetCenterBottom();
        float fRadius = capsule.GetRadius();
        float fHeight = FMath::Distance(vCenterTop, vCenterBottom);

        FVector3 d = vCenterTop - vCenterBottom;
		FVector3 m = rayPos - vCenterBottom;
        FVector3 n = rayPosFar - rayPos;
        float md = FMath::Dot(m, d);
        float nd = FMath::Dot(n, d);
        float dd = FMath::Dot(d, d);

        //2> Test if segment rayPos-rayPosFar fully outside either endsphere of capsule
        if (md < 0.0f && md + nd < fRadius) //outside 'rayPos' side of capsule
            return std::pair<bool, float>(false, 0);
        if (md > dd && md + nd > dd + fRadius) //outside 'rayPosFar' side of capsule
            return std::pair<bool, float>(false, 0);

        float mm = FMath::Dot(m, m);
        float rr = fRadius * fRadius;
        float nn = FMath::Dot(n, n);
        float mn = FMath::Dot(m, n);
        float a = dd * nn - nd * nd;
        float k = mm - rr;
        float c = dd * k - md * md;
        //segment rayPos-rayPosFar parallel to capsule axis
        if (FMath::Abs(a) < FMath::ms_fEpsilon)
        {
            //out of r range
            if (c > 0.0f)
            {
                return std::pair<bool, float>(false, 0);
            }

            //segment rayPos-rayPosFar intersect capsule
            if (md < 0.0f) //intersect segment against 'rayPos' endcap
            {
                t = (- mn / nn) * FMath::Sqrt(nn); 
                t = t - FMath::Sqrt(rr - (mm - t*t));
            }
            else if (md > dd) //intersect segment against 'rayPosFar' endcap
            {
                t = ((nd - mn) / nn) * FMath::Sqrt(nn); 
                t = t - FMath::Sqrt(rr - (mm - (t + fHeight)*(t + fHeight)));
            }
            else //lies inside capsule
            {
                t = 0.0f; 
            }
            return std::pair<bool, float>(true, t);
        }

        float b = dd * mn - nd * md;
        float discr = b * b - a * c;
        if (discr < 0.0f) //no real roots; no intersection
            return std::pair<bool, float>(false, 0); 

        t = (-b - FMath::Sqrt(discr)) / a;
        if (t < 0.0f || t > 1.0f) //intersection lies outside segment
            return std::pair<bool, float>(false, 0); 

        if (md + t * nd < 0.0f)
        {
            return Intersects_RaySphere(ray, FSphere(vCenterBottom, fRadius));
        }
        else if (md + t * nd > dd)
        {
            return Intersects_RaySphere(ray, FSphere(vCenterTop, fRadius));
        }

        //segment intersects capsule between endcaps; t is correct
        return std::pair<bool, float>(true, t * FMath::Sqrt(nn));
    }
    int FMath::Intersects_RayCapsule(const FRay& ray, const FCapsule& capsule, bool& isInside, float* d1, float* d2)
    {
        *d1 = 0;
        *d2 = 0;
        bool hasD1 = false;
        int count = 0;

        const FVector3& rayPos = ray.GetOrigin();  
        FVector3 rayPosFar = ray.GetPoint(ms_fRayFar); //Ray far point

        const FVector3& vCenterTop = capsule.GetCenterTop();
        const FVector3& vCenterBottom = capsule.GetCenterBottom();
        float fRadius = capsule.GetRadius();
        float fHeight = FMath::Distance(vCenterTop, vCenterBottom);

        FVector3 d = vCenterTop - vCenterBottom;
		FVector3 m = rayPos - vCenterBottom;
        FVector3 n = rayPosFar - rayPos;
        float md = FMath::Dot(m, d);
        float nd = FMath::Dot(n, d);
        float dd = FMath::Dot(d, d);

        //1> Test if segment rayPos-rayPosFar fully outside either endcap of capsule
        if (md < 0.0f && md + nd < 0.0f) //outside 'rayPos' side of capsule
            return count;
        if (md > dd && md + nd > dd) //outside 'rayPosFar' side of capsule
            return count;

        float nn = FMath::Dot(n, n);
        float mn = FMath::Dot(m, n);
        float a = dd * nn - nd * nd;
        float k = FMath::Dot(m, m) - fRadius * fRadius;
        float c = dd * k - md * md;
        //segment rayPos-rayPosFar parallel to capsule axis
        if (FMath::Abs(a) < FMath::ms_fEpsilon)
        {
            //out of r range
            if (c > 0.0f)
            {
                return count;
            }

            //segment rayPos-rayPosFar intersect capsule
            if (md < 0.0f) //intersect segment against 'rayPos' endcap
            {
                float nl = FMath::Sqrt(nn);
                *d1 = (-mn / nn) * nl;
                count ++;

                *d2 = *d1 + fHeight;
                if (*d2 <= nl)
                {
                    count ++;
                }
                else
                {
                    *d2 = 0.0f;
                }
                return count;
            }
            else if (md > dd) //intersect segment against 'rayPosFar' endcap
            {
                float nl = FMath::Sqrt(nn);
                *d1 = ((nd - mn) / nn) * nl; 
                count ++;

                *d2 = *d1 + fHeight;
                if (*d2 <= nl)
                {
                    count ++;
                }
                else
                {
                    *d2 = 0.0f;
                }
                return count;
            }
            else //lies inside capsule
            {
                *d1 = 0.0f;
                *d2 = 0.0f;
                return count;
            }
        }

        float b = dd * mn - nd * md;
        float discr = b * b - a * c;
        if (discr < 0.0f) //no real roots; no intersection
        {
            return count;
        }

        //2> t1
        {
            float t1 = (-b - FMath::Sqrt(discr)) / a;
            if (t1 < 0.0f || t1 > 1.0f) //intersection lies outside segment
                return count;

            bool isInter = false;
            if (md + t1 * nd < 0.0f)
            {
                std::pair<bool, float> tt1 = Intersects_RaySphere(ray, FSphere(vCenterBottom, fRadius));
                if (tt1.first)
                {
                    t1 = tt1.second;
                }
            }
            else if (md + t1 * nd > dd)
            {
                std::pair<bool, float> tt1 = Intersects_RaySphere(ray, FSphere(vCenterTop, fRadius));
                if (tt1.first)
                {
                    t1 = tt1.second;
                }
            }
            else
            {
                //segment intersects capsule between endcaps; t is correct
                isInter = true;
            }

            if (isInter)
            {
                hasD1 = true;
                *d1 = t1;
                count ++;   
            }
        }
        
        //3> t2
        {
            float t2 = (-b + FMath::Sqrt(discr)) / a;
            if (t2 < 0.0f || t2 > 1.0f) //intersection lies outside segment
                return count;

            bool isInter = false;
            if (md + t2 * nd < 0.0f)
            {
                std::pair<bool, float> tt2 = Intersects_RaySphere(ray, FSphere(vCenterBottom, fRadius));
                if (tt2.first)
                {
                    t2 = tt2.second;
                }
            }
            else if (md + t2 * nd > dd)
            {
                std::pair<bool, float> tt2 = Intersects_RaySphere(ray, FSphere(vCenterTop, fRadius));
                if (tt2.first)
                {
                    t2 = tt2.second;
                }
            }
            else
            {
                //segment intersects capsule between endcaps; t is correct
                isInter = true;
            }

            if (isInter)
            {
                if (hasD1)
                {
                    *d2 = t2;
                    count ++;
                }
                else
                {
                    *d1 = t2;
                    count ++;
                }
            }
        }

        return count;
    }
    int FMath::Intersects_RayCapsule(const FRay& ray, const FCapsule& capsule, bool& isInside, FVector3& vIntersection1, FVector3& vIntersection2)
    {   
        float fStart, fEnd;
        int nNum = Intersects_RayCapsule(ray, capsule, isInside, &fStart, &fEnd);
        if (nNum > 0)
        {
            vIntersection1 = ray.GetPoint(fStart);
            vIntersection2 = ray.GetPoint(fEnd);
        }
        else
        {
            vIntersection1 = FMath::ms_v3Zero;
            vIntersection2 = FMath::ms_v3Zero;
        }
        return nNum;
    }

    std::pair<bool, float> FMath::Intersects_RayCone(const FRay& ray, const FCone& cone, bool discardInside /*= true*/)
    {
        float t = 0;
        const FVector3& rayPos = ray.GetOrigin();
        //1> rayPos In Cone
        if (discardInside && cone.Intersects_Point(rayPos))
        {
            return std::pair<bool, float>(true, 0);
        }

        //2> Ray - Cone Caps
        const FVector3& vCenterTop = cone.GetTop();
        const FVector3& vCenterBottom = cone.GetCenter();
        float fRadiusTop = cone.GetRadiusTop();
        float fRadius = cone.GetRadius();

        FVector3 v = FMath::Normalize(vCenterBottom - vCenterTop);
        FVector3 m = rayPos - vCenterBottom;
        float mv = FMath::Dot(m, v);
        //rayPos under vCenterBottom
        if (mv > 0.0f)
        {
            FVector3 vIntersection;
            float dis = FMath::Sqrt(FMath::Abs(FMath::Dot(vCenterBottom, v)));
            std::pair<bool, float> ret = Intersects_RayCircle(ray, FCircle(v, dis, vCenterBottom, fRadius), vIntersection);
            if (ret.first)
            {
                return std::pair<bool, float>(true, ret.second);
            }
        }
        //rayPos above vCenterTop and top is a circle
        if (fRadiusTop > 0.0f)
        {
            FVector3 n = rayPos - vCenterTop;
            float nv = FMath::Dot(n, v);
            if (nv < 0.0f)
            {
                FVector3 vIntersection;
                float dis = FMath::Sqrt(FMath::Abs(FMath::Dot(vCenterTop, v)));
                std::pair<bool, float> ret = Intersects_RayCircle(ray, FCircle(-v, dis, vCenterBottom, fRadiusTop), vIntersection);
                if (ret.first)
                {
                    return std::pair<bool, float>(true, ret.second);
                }
            }
        }

        //3> Ray - Cone 
        const FVector3& d = ray.GetDirection();
        FVector3 vRealTop = cone.GetRealTop();
        FVector3 x = rayPos - vRealTop;
        float dd = FMath::Dot(d, d);
        float vv = FMath::Dot(v, v);
        float xx = FMath::Dot(x, x);
        float xv = FMath::Dot(x, v);
        float dv = FMath::Dot(d, v);
        float dx = FMath::Dot(d, x);

        float fHeight = FMath::Distance(vRealTop, vCenterBottom);
        float k = fRadius / fHeight;
        float k2 = k * k;

        float a = dd - (1.0f + k2) * dv * dv;
        float b = dx - (1.0f + k2) * dv * xv;
        float c = xx - (1.0f + k2) * xv * xv;

        float discr = b * b - a * c;
        if (discr < 0.0f) //no real roots; no intersection
            return std::pair<bool, float>(false, 0); 

        t = (-b - FMath::Sqrt(discr)) / a;
        if (t < 0.0f) //intersection lies outside segment
            return std::pair<bool, float>(false, 0); 

        float y = dv * t + xv;
        if (y < (fRadiusTop / k) || y > (fRadius / k)) //not between [fRadiusTop/k, fRadius/k]
            return std::pair<bool, float>(false, 0); 

        return std::pair<bool, float>(true, t);
    } 
    int FMath::Intersects_RayCone(const FRay& ray, const FCone& cone, bool& isInside, float* d1, float* d2)
    {
        *d1 = 0;
        *d2 = 0;

        const FVector3& rayPos = ray.GetOrigin();
        const FVector3& d = ray.GetDirection();
        FVector3 vRealTop = cone.GetRealTop();
        const FVector3& vCenterTop = cone.GetTop();
        const FVector3& vCenterBottom = cone.GetCenter();
        float fRadiusTop = cone.GetRadiusTop();
        float fRadius = cone.GetRadius();
        float fHeight = FMath::Distance(vRealTop, vCenterBottom);

        FVector3 v = FMath::Normalize(vCenterBottom - vCenterTop);
        FVector3 m = rayPos - vCenterBottom;
        FVector3 x = rayPos - vRealTop;
        float dd = FMath::Dot(d, d);
        float vv = FMath::Dot(v, v);
        float xx = FMath::Dot(x, x);
        float xv = FMath::Dot(x, v);
        float dv = FMath::Dot(d, v);
        float dx = FMath::Dot(d, x);

        std::vector<float> aTs;

        //1> Ray - Cone
        float k = fRadius / fHeight;
        float k2 = k * k;

        float a = dd - (1.0f + k2) * dv * dv;
        float b = dx - (1.0f + k2) * dv * xv;
        float c = xx - (1.0f + k2) * xv * xv;

        float t = 0;
        float discr = b * b - a * c;
        if (discr >= 0.0f) 
        {
            //t1
            t = (-b - FMath::Sqrt(discr)) / a;
            if (t >= 0.0f) 
            {
                float y = dv * t + xv;
                if (y >= (fRadiusTop / k) && y <= (fRadius / k)) //between [fRadiusTop/k, fRadius/k]
                    aTs.push_back(t);
            }

            //t2
            t = (-b + FMath::Sqrt(discr)) / a;
            if (t >= 0.0f)
            {
                float y = dv * t + xv;
                if (y >= (fRadiusTop / k) && y <= (fRadius / k)) //between [fRadiusTop/k, fRadius/k]
                    aTs.push_back(t);
            }
        }

        //2> Ray - Cone Caps
        FVector3 vIntersection;
        float dis = FMath::Sqrt(FMath::Abs(FMath::Dot(vCenterBottom, v)));
        std::pair<bool, float> ret = Intersects_RayCircle(ray, FCircle(v, dis, vCenterBottom, fRadius), vIntersection);
        if (ret.first)
        {
            aTs.push_back(ret.first);
        }

        if (fRadiusTop > 0.0f)
        {
            dis = FMath::Sqrt(FMath::Abs(FMath::Dot(vCenterTop, v)));
            ret = Intersects_RayCircle(ray, FCircle(-v, dis, vCenterBottom, fRadiusTop), vIntersection);
            if (ret.first)
            {
                aTs.push_back(ret.first);
            }
        }

        std::sort(aTs.begin(), aTs.end(), float_less);
        int count = (int)aTs.size();
        if (count > 0)
            *d1 = aTs[0];
        if (count > 1)
            *d2 = aTs[1];
        return count;
    }
    int FMath::Intersects_RayCone(const FRay& ray, const FCone& cone, bool& isInside, FVector3& vIntersection1, FVector3& vIntersection2)
    {   
        float fStart, fEnd;
        int nNum = Intersects_RayCone(ray, cone, isInside, &fStart, &fEnd);
        if (nNum > 0)
        {
            vIntersection1 = ray.GetPoint(fStart);
            vIntersection2 = ray.GetPoint(fEnd);
        }
        else
        {
            vIntersection1 = FMath::ms_v3Zero;
            vIntersection2 = FMath::ms_v3Zero;
        }
        return nNum;
    }

    std::pair<bool, float> FMath::Intersects_RayTorus(const FRay& ray, const FTorus& torus, bool discardInside /*= true*/, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        float t = 0;
        const FVector3& rayPos = ray.GetOrigin();
        //1> rayPos In Torus
        if (discardInside && torus.Intersects_Point(rayPos))
        {
            return std::pair<bool, float>(true, 0);
        }

        //2> Ray - Torus
        float fD1, fD2, fD3, fD4;
        int nNum = Intersects_RayTorus(ray, torus, discardInside, &fD1, &fD2, &fD3, &fD4, fEpsilon);
        if (nNum > 0)
        {
            return std::pair<bool, float>(true, fD1);
        }

        return std::pair<bool, float>(false, 0);
    }
    int FMath::Intersects_RayTorus(const FRay& ray, const FTorus& torus, bool& isInside, float* d1, float* d2, float* d3, float* d4, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        *d1 = 0;
        *d2 = 0;
        *d3 = 0;
        *d4 = 0;

        const FVector3& rayPos = ray.GetOrigin();
        const FVector3& rayDir = ray.GetDirection();
        const FVector3& vCenter = torus.GetCenter();
        const FVector3& vDir = torus.GetDirection();
        float R = torus.GetRadius();
        float r = torus.GetSectionRadius();
        FVector3 x = rayPos - vCenter;

        float m = FMath::Dot(rayDir, rayDir);
        float n = FMath::Dot(rayDir, x);
        float o = FMath::Dot(x, x);
        float p = FMath::Dot(rayDir, vDir);
        float q = FMath::Dot(x, vDir);

        float a = m*m;
        float b = 4*m*n;
        float c = 4*n*n + 2*m*o - 2*(R*R + r*r)*m + 4*R*R*p*p;
        float d = 4*n*o - 4*(R*R + r*r)*n + 8*R*R*p*q;
        float e = o*o - 2*(R*R + r*r)*o + 4*R*R*q*q + (R*R - r*r)*(R*R - r*r);

        int count = Equation_Quartic(a, b, c, d, e, *d1, *d2, *d3, *d4, fEpsilon);
        return count;
    }
    int FMath::Intersects_RayTorus(const FRay& ray, const FTorus& torus, bool& isInside, FVector3& vIntersection1, FVector3& vIntersection2, FVector3& vIntersection3, FVector3& vIntersection4, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {   
        float fD1, fD2, fD3, fD4;
        int nNum = Intersects_RayTorus(ray, torus, isInside, &fD1, &fD2, &fD3, &fD4, fEpsilon);
        if (nNum > 0)
        {
            vIntersection1 = ray.GetPoint(fD1);
            vIntersection2 = ray.GetPoint(fD2);
            vIntersection3 = ray.GetPoint(fD3);
            vIntersection4 = ray.GetPoint(fD4);
        }
        else
        {
            vIntersection1 = FMath::ms_v3Zero;
            vIntersection2 = FMath::ms_v3Zero;
            vIntersection3 = FMath::ms_v3Zero;
            vIntersection4 = FMath::ms_v3Zero;
        }
        return nNum;
    }

    bool FMath::Intersects_RaySegment_Test(const FRay& ray, const FVector3& s, const FVector3& e, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vIntersection;
        std::pair<bool, float> ret = Intersects_RaySegment(ray, s, e, vIntersection, fEpsilon);
        return ret.first;
    }
    bool FMath::Intersects_RaySegment_Test(const FRay& ray, const FSegment& segment, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vIntersection;
        std::pair<bool, float> ret = Intersects_RaySegment(ray, segment, vIntersection, fEpsilon);
        return ret.first;
    }
    bool FMath::Intersects_RayTriangle_Test(const FRay& ray, const FVector3& a, const FVector3& b, const FVector3& c, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vIntersection;
        std::pair<bool, float> ret = Intersects_RayTriangle(ray, a, b, c, vIntersection, fEpsilon);
        return ret.first;
    }
    bool FMath::Intersects_RayTriangle_Test(const FRay& ray, const FTriangle& triangle, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vIntersection;
        std::pair<bool, float> ret = Intersects_RayTriangle(ray, triangle, vIntersection, fEpsilon);
        return ret.first;
    }
    bool FMath::Intersects_RayQuad_Test(const FRay& ray, const FVector3& a, const FVector3& b, const FVector3& c, const FVector3& d, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vIntersection;
        std::pair<bool, float> ret = Intersects_RayQuad(ray, a, b, c, d, vIntersection, fEpsilon);
        return ret.first;
    }
    bool FMath::Intersects_RayQuad_Test(const FRay& ray, const FQuad& quad, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vIntersection;
        std::pair<bool, float> ret = Intersects_RayQuad(ray, quad, vIntersection, fEpsilon);
        return ret.first;
    }
    bool FMath::Intersects_RayPlane_Test(const FRay& ray, const FPlane& plane, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vIntersection;
        std::pair<bool, float> ret = Intersects_RayPlane(ray, plane, vIntersection, fEpsilon);
        return ret.first;
    }
    bool FMath::Intersects_RayCircle_Test(const FRay& ray, const FPlane& plane, const FVector3& center, float radius, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vIntersection;
        std::pair<bool, float> ret = Intersects_RayCircle(ray, plane, center, radius, vIntersection, fEpsilon);
        return ret.first;
    }
    bool FMath::Intersects_RayCircle_Test(const FRay& ray, const FCircle& circle, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        FVector3 vIntersection;
        std::pair<bool, float> ret = Intersects_RayCircle(ray, circle, vIntersection, fEpsilon);
        return ret.first;
    }
    bool FMath::Intersects_RayAABB_Test(const FRay& ray, const FAABB& aabb)
    {   
        std::pair<bool, float> ret = Intersects_RayAABB(ray, aabb);
        return ret.first;
    }
    bool FMath::Intersects_RayAABB_Test(const FRay& ray, const FAABB& aabb, float* d1, float* d2)
    {
        return Intersects_RayAABB(ray, aabb, d1, d2);
    }
    bool FMath::Intersects_RayAABB_Test(const FRay& ray, const FAABB& aabb, FVector3& vIntersection1, FVector3& vIntersection2)
    {
        return Intersects_RayAABB(ray, aabb, vIntersection1, vIntersection2);
    }
    bool FMath::Intersects_RaySphere_Test(const FRay& ray, const FSphere& sphere, bool discardInside /*= true*/)
    {
        std::pair<bool, float> ret = Intersects_RaySphere(ray, sphere, discardInside);
        return ret.first;
    }
    bool FMath::Intersects_RaySphere_Test(const FRay& ray, const FSphere& sphere, bool& isInside, float* d1, float* d2)
    {
        return Intersects_RaySphere(ray, sphere, isInside, d1, d2) > 0;
    }
    bool FMath::Intersects_RaySphere_Test(const FRay& ray, const FSphere& sphere, bool& isInside, FVector3& vIntersection1, FVector3& vIntersection2)
    {
        return Intersects_RaySphere(ray, sphere, isInside, vIntersection1, vIntersection2) > 0;
    }
    bool FMath::Intersects_RayCylinder_Test(const FRay& ray, const FCylinder& cylinder, bool discardInside /*= true*/)
    {
        std::pair<bool, float> ret = Intersects_RayCylinder(ray, cylinder, discardInside);
        return ret.first;
    }
    bool FMath::Intersects_RayCylinder_Test(const FRay& ray, const FCylinder& cylinder, bool& isInside, float* d1, float* d2)
    {
        return Intersects_RayCylinder(ray, cylinder, isInside, d1, d2) > 0;
    }   
    bool FMath::Intersects_RayCylinder_Test(const FRay& ray, const FCylinder& cylinder, bool& isInside, FVector3& vIntersection1, FVector3& vIntersection2)
    {
        return Intersects_RayCylinder(ray, cylinder, isInside, vIntersection1, vIntersection2) > 0;
    }
    bool FMath::Intersects_RayCapsule_Test(const FRay& ray, const FCapsule& capsule, bool discardInside /*= true*/)
    {
        std::pair<bool, float> ret = Intersects_RayCapsule(ray, capsule, discardInside);
        return ret.first;
    }
    bool FMath::Intersects_RayCapsule_Test(const FRay& ray, const FCapsule& capsule, bool& isInside, float* d1, float* d2)
    {
        return Intersects_RayCapsule(ray, capsule, isInside, d1, d2) > 0;
    }   
    bool FMath::Intersects_RayCapsule_Test(const FRay& ray, const FCapsule& capsule, bool& isInside, FVector3& vIntersection1, FVector3& vIntersection2)
    {
        return Intersects_RayCapsule(ray, capsule, isInside, vIntersection1, vIntersection2) > 0;
    }
    bool FMath::Intersects_RayCone_Test(const FRay& ray, const FCone& cone, bool discardInside /*= true*/)
    {
        std::pair<bool, float> ret = Intersects_RayCone(ray, cone, discardInside);
        return ret.first;
    }
    bool FMath::Intersects_RayCone_Test(const FRay& ray, const FCone& cone, bool& isInside, float* d1, float* d2)
    {
        return Intersects_RayCone(ray, cone, isInside, d1, d2) > 0;
    }   
    bool FMath::Intersects_RayCone_Test(const FRay& ray, const FCone& cone, bool& isInside, FVector3& vIntersection1, FVector3& vIntersection2)
    {
        return Intersects_RayCone(ray, cone, isInside, vIntersection1, vIntersection2) > 0;
    }
    bool FMath::Intersects_RayTorus_Test(const FRay& ray, const FTorus& torus, bool discardInside /*= true*/, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        std::pair<bool, float> ret = Intersects_RayTorus(ray, torus, discardInside, fEpsilon);
        return ret.first;
    }   
    bool FMath::Intersects_RayTorus_Test(const FRay& ray, const FTorus& torus, bool& isInside, float* d1, float* d2, float* d3, float* d4, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return Intersects_RayTorus(ray, torus, isInside, d1, d2, d3, d4, fEpsilon) > 0;
    }   
    bool FMath::Intersects_RayTorus_Test(const FRay& ray, const FTorus& torus, bool& isInside, FVector3& vIntersection1, FVector3& vIntersection2, FVector3& vIntersection3, FVector3& vIntersection4, float fEpsilon /*= FMath::ms_fEpsilon*/)
    {
        return Intersects_RayTorus(ray, torus, isInside, vIntersection1, vIntersection2, vIntersection3, vIntersection4, fEpsilon) > 0;
    }

    //Sphere - Shape
    bool FMath::Intersects_SpherePlane(const FSphere& sphere, const FPlane& plane)
    {
        return (FMath::Abs(plane.GetDistance(sphere.GetCenter())) <= sphere.GetRadius());
    }

    bool FMath::Intersects_SphereAABB(const FSphere& sphere, const FAABB& aabb)
    {
        if (!aabb.IsValid())
            return false;

        const FVector3& center = sphere.GetCenter();
        float radius = sphere.GetRadius();
        const FVector3& min = aabb.GetMin();
        const FVector3& max = aabb.GetMax();

        float s, d = 0;
        for (int32 i = 0; i < 3; ++i)
        {
            if (center[i] < min[i])
            {
                s = center[i] - min[i];
                d += s * s; 
            }
            else if(center[i] > max[i])
            {
                s = center[i] - max[i];
                d += s * s; 
            }
        }
        return d <= radius * radius;
    }

    bool FMath::Intersects_SphereFrustum(const FSphere& sphere, const FFrustum& frustum)
    {
        if (frustum.GetPlane(F_FrustumPlane_Left).GetDistance(sphere.GetCenter()) > sphere.GetRadius())
            return false;
        if (frustum.GetPlane(F_FrustumPlane_Right).GetDistance(sphere.GetCenter()) > sphere.GetRadius())
            return false;
        if (frustum.GetPlane(F_FrustumPlane_Top).GetDistance(sphere.GetCenter()) > sphere.GetRadius())
            return false;
        if (frustum.GetPlane(F_FrustumPlane_Bottom).GetDistance(sphere.GetCenter()) > sphere.GetRadius())
            return false;
        if (frustum.GetPlane(F_FrustumPlane_Near).GetDistance(sphere.GetCenter()) > sphere.GetRadius())
            return false;
        if (frustum.GetPlane(F_FrustumPlane_Far).GetDistance(sphere.GetCenter()) > sphere.GetRadius())
            return false;

        return true;
    }

    //bool FMath::Intersects_SegmentAABB(const FSegment& s, const FAABB& aabb)
    //{
    //  return true;
    //}

    //Plane - Shape
    bool FMath::Intersects_PlaneAABB(const FPlane& plane, const FAABB& aabb)
    {
        return (plane.GetSide(aabb) == F_PlaneSide_Both);
    }


    float FMath::GaussianDistribution(float x, float offset /*= 0.0f*/, float scale /*= 1.0f*/)
    {
        float nom = FMath::Exp(-FMath::Square(x - offset) / (2 * FMath::Square(scale)));
		float denom = scale * FMath::Sqrt(2 * FMath::ms_fPI);
		return nom / denom;
    }

    FMatrix4 FMath::BuildReflectionMatrix(const FPlane& p)
    {
        return FMatrix4(
			-2 * p.m_vNormal.x * p.m_vNormal.x + 1,   -2 * p.m_vNormal.x * p.m_vNormal.y,       -2 * p.m_vNormal.x * p.m_vNormal.z,       -2 * p.m_vNormal.x * p.m_fDistance, 
			-2 * p.m_vNormal.y * p.m_vNormal.x,       -2 * p.m_vNormal.y * p.m_vNormal.y + 1,   -2 * p.m_vNormal.y * p.m_vNormal.z,       -2 * p.m_vNormal.y * p.m_fDistance, 
			-2 * p.m_vNormal.z * p.m_vNormal.x,       -2 * p.m_vNormal.z * p.m_vNormal.y,       -2 * p.m_vNormal.z * p.m_vNormal.z + 1,   -2 * p.m_vNormal.z * p.m_fDistance, 
												 0,                                        0,                                        0,                                    1);
    }

    FVector3 FMath::CalculateTangentSpaceVector(const FVector3& position1, const FVector3& position2, const FVector3& position3,
												float u1, float v1, float u2, float v2, float u3, float v3)
    {
		FVector3 side0 = position1 - position2;
		FVector3 side1 = position3 - position1;
		FVector3 normal = FMath::Cross(side1, side0);
		normal = FMath::Normalize(normal);
		//Calculate tangent. 
		float deltaV0 = v1 - v2;
		float deltaV1 = v3 - v1;
		FVector3 tangent = FMath::Normalize(deltaV1 * side0 - deltaV0 * side1);
		//Calculate binormal
		float deltaU0 = u1 - u2;
		float deltaU1 = u3 - u1;
		FVector3 binormal = FMath::Normalize(deltaU1 * side0 - deltaU0 * side1);
        
		FVector3 tangentCross = FMath::Cross(tangent, binormal);
		if (FMath::Dot(tangentCross, normal) < 0.0f)
		{
			tangent = -tangent;
			binormal = -binormal;
		}

		return tangent;
    }

    FMatrix4 FMath::MakeMatrix4ViewLH(const FVector3& vPos, const FQuaternion& qRot, const FMatrix4* pReflectMatrix /*= nullptr*/)
    {
        FMatrix4 mat4View = FMath::ms_mat4Unit;

		// View matrix is: Use Left-Hand Coordinate, HLSL (matrix4 * vector4)
		//  [ Px  Py  Pz   0  ]
		//  [ Qx  Qy  Qz   0  ]
		//  [ Rx  Ry  Rz   0  ]
		//  [ Tx  Ty  Tz   1  ]
		// T = -Transpose(Matrix3(Rot)) * Pos

		FMatrix3 mat3Rot = FMath::TransposeMatrix3(FMath::ToMatrix3(qRot));
        FVector3 trans = -mat3Rot * vPos;

		mat4View = FMatrix4(mat3Rot);
		mat4View[3][0] = trans.x;
		mat4View[3][1] = trans.y;
		mat4View[3][2] = trans.z;

		// Reflection
		if (pReflectMatrix)
		{
			mat4View = mat4View * (*pReflectMatrix);
		}
		return mat4View;
    }

    FMatrix4 FMath::MakeMatrix4ProjectionPerspectiveLH(float rFovY, float fAspect, float fNear, float fFar)
    {
        return glm::perspectiveLH(rFovY,
                                  fAspect,
                                  fNear, 
                                  fFar);
    }

    FVector3 FMath::TransformFromScreenToWorld(const FVector3& vScreenCoord, const FMatrix4& mat4ModelView, const FMatrix4& mat4Proj, const FVector4& vViewport)
    {
        FVector3 vWorldCoord;
        TransformFromScreenToWorld(vScreenCoord, mat4ModelView, mat4Proj, vViewport, vWorldCoord);
        return vWorldCoord;
    }

    void FMath::TransformFromScreenToWorld(const FVector3& vScreenCoord, const FMatrix4& mat4ModelView, const FMatrix4& mat4Proj, const FVector4& vViewport, FVector3& vWorldCoord)
    {
        vWorldCoord = glm::unProject(vScreenCoord, mat4ModelView, mat4Proj, vViewport);
    }

}; //LostPeterFoundation