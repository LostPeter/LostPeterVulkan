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
#include "../include/FPlane.h"
#include "../include/FPlaneBoundedVolume.h"
#include "../include/FSphere.h"
#include "../include/FAABB.h"
#include "../include/FFrustum.h"
#include "../include/FCylinder.h"
#include "../include/FCapsule.h"
#include "../include/FCone.h"
#include "../include/FTorus.h"

namespace LostPeterFoundation
{
    const float FMath::ms_fPI_Half = glm::pi<float>() / 2.0f;
    const float FMath::ms_fPI = glm::pi<float>();
    const float FMath::ms_fPI_Two = glm::pi<float>() * 2.0f;
    const float FMath::ms_fDeg2Rad = FMath::ms_fPI / float(180.0f);		
    const float FMath::ms_fRad2Deg = float(180.0f) / FMath::ms_fPI;	
    const float FMath::ms_fLog2 = log(float(2.0f));

    const float FMath::ms_fPosInfinity =  std::numeric_limits<float>::infinity();	
    const float FMath::ms_fNegInfinity = -std::numeric_limits<float>::infinity();
    const float FMath::ms_fEpsilon = std::numeric_limits<float>::epsilon();

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
                                                 0.0f, 0.0f, 0.0);
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

    const FVector4 FMath::ms_clBlack = FVector4(0.0f, 0.0f, 0.0f, 1.0f);
    const FVector4 FMath::ms_clWhite = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
    const FVector4 FMath::ms_clRed = FVector4(1.0f, 0.0f, 0.0f, 1.0f);
    const FVector4 FMath::ms_clGreen = FVector4(0.0f, 1.0f, 0.0f, 1.0f);
    const FVector4 FMath::ms_clBlue = FVector4(0.0f, 0.0f, 1.0f, 1.0f);
    const FVector4 FMath::ms_clGray = FVector4(0.5f, 0.5f, 0.5f, 1.0f);
    const FVector4 FMath::ms_clDarkRed = FVector4(0.5f, 0.0f, 0.0f, 1.0f);
    const FVector4 FMath::ms_clDarkGreen = FVector4(0.0f, 0.5f, 0.0f, 1.0f);
    const FVector4 FMath::ms_clDarkBlue = FVector4(0.0f, 0.0f, 0.5f, 1.0f);
    const FVector4 FMath::ms_clDarkGray = FVector4(0.25f, 0.25f, 0.25f, 1.0f);
    const FVector4 FMath::ms_clYellow = FVector4(1.0f, 1.0f, 0.0f, 1.0f);
    const FVector4 FMath::ms_clCyan = FVector4(0.0f, 1.0f, 1.0f, 1.0f);
    const FVector4 FMath::ms_clMagenta = FVector4(1.0f, 0.0f, 1.0f, 1.0f);
    const FVector4 FMath::ms_clTransparent = FVector4(1.0f, 1.0f, 1.0f, 0.0f);

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
        return GetAngleSinFromLinePlane(segment.m_P0, segment.m_P1, pt1, pt2, pt3);
    }

    //Distance From Point-Line
    float FMath::GetDistanceFromPointLine(const FVector3& pt, const FVector3& ptLine11, const FVector3& ptLine12)
    {
        FVector3 v12 = ptLine12 - ptLine11;
        FVector3 vPL1 = ptLine11 - pt;
        FVector3 vPL2 = ptLine11 - pt;
        return Length(Cross(vPL1, v12)) / Distance(ptLine11, ptLine12);
    }
    float FMath::GetDistanceFromPointLine(const FVector3& pt, const FSegment& segment)
    {
        return GetDistanceFromPointLine(pt, segment.m_P0, segment.m_P1);
    }

    //Distance From Point-Plane
    float FMath::GetDistanceFromPointPlane(const FVector3& pt, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3)
    {
        FVector3 vNormal = GetNormal3WithoutNormalizeFromPoints3(pt1, pt2, pt3);
        return Abs(Dot(vNormal, (pt - pt1))) / Length(vNormal);
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
        return GetDistanceFromLine2(segment1.m_P0, segment1.m_P1, segment2.m_P0, segment2.m_P1);
    }

    //Intersection Point From Line2
    bool FMath::GetIntersectionPointFromLine2(const FVector3& ptLine11, const FVector3& ptLine12, const FVector3& ptLine21, const FVector3& ptLine22, FVector3& vIntersection)
    {
        vIntersection = FMath::ms_v3Zero;
        if (LineLine_NotIntersect(ptLine11, ptLine12, ptLine21, ptLine22, true))
            return false;
        
        vIntersection = ptLine11;
        float t = ((ptLine11.x - ptLine21.x)*(ptLine21.y - ptLine22.y) - (ptLine11.y - ptLine21.y)*(ptLine21.x - ptLine22.x)) / 
                  ((ptLine11.x - ptLine12.x)*(ptLine21.y - ptLine22.y) - (ptLine11.y - ptLine12.y)*(ptLine21.x - ptLine22.x));
        vIntersection.x += (ptLine12.x - ptLine11.x) * t;
        vIntersection.y += (ptLine12.y - ptLine11.y) * t;
        vIntersection.z += (ptLine12.z - ptLine11.z) * t;

        return true;
    }
    bool FMath::GetIntersectionPointFromLine2(const FSegment& segment1, const FSegment& segment2, FVector3& vIntersection)
    {
        return GetIntersectionPointFromLine2(segment1.m_P0, segment1.m_P1, segment2.m_P0, segment2.m_P1, vIntersection);
    }

    //Intersection Point From Line-Plane
    bool FMath::GetIntersectionPointFromLinePlane(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, FVector3& vIntersection)
    {
        vIntersection = FMath::ms_v3Zero;
        if (LineTriangle_NotIntersect(ptLine1, ptLine2, pt1, pt2, pt3, true))
            return false;

        FVector3 vNormal = GetNormal3WithoutNormalizeFromPoints3(pt1, pt2, pt3);
        vIntersection = vNormal;
        float t = (vNormal.x * (pt1.x - ptLine1.x) + vNormal.y * (pt1.y - ptLine1.y) + vNormal.z * (pt1.z - ptLine1.z)) /
                  (vNormal.x * (ptLine2.x - ptLine1.x) + vNormal.y * (ptLine2.y - ptLine1.y) + vNormal.z * (ptLine2.z - ptLine1.z));
        vIntersection.x = ptLine1.x + (ptLine2.x - ptLine1.x) * t;
        vIntersection.y = ptLine1.y + (ptLine2.y - ptLine1.y) * t;
        vIntersection.z = ptLine1.z + (ptLine2.z - ptLine1.z) * t;

        return true;
    }
    bool FMath::GetIntersectionPointFromLinePlane(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, FVector3& vIntersection)
    {
        return GetIntersectionPointFromLinePlane(segment.m_P0, segment.m_P1, pt1, pt2, pt3, vIntersection);
    }

    //Intersection Line From Plane-Plane
    bool FMath::GetIntersectionLineFromPlane2(const FVector3& pP11, const FVector3& pP12, const FVector3& pP13, const FVector3& pP21, const FVector3& pP22, const FVector3& pP23, FVector3& pL1, FVector3& pL2)
    {
        pL1 = FMath::ms_v3Zero;
        pL2 = FMath::ms_v3Zero;
        if (PlanePlane_Parallel(pP11, pP12, pP13, pP21, pP22, pP23))
            return false;

        if (LinePlane_Parallel(pP21, pP22, pP11, pP12, pP13))
        {
            if (!GetIntersectionPointFromLinePlane(pP22, pP23, pP11, pP12, pP13, pL1))
            {
                return false;
            }
        }
        else
        {
            if (!GetIntersectionPointFromLinePlane(pP21, pP22, pP11, pP12, pP13, pL1))
            {
                return false;
            }
        }

        if (LinePlane_Parallel(pP23, pP21, pP11, pP12, pP13))
        {
            if (!GetIntersectionPointFromLinePlane(pP22, pP23, pP11, pP12, pP13, pL2))
            {
                return false;
            }
        }
        else
        {
            if (!GetIntersectionPointFromLinePlane(pP23, pP21, pP11, pP12, pP13, pL2))
            {
                return false;
            }
        }

        return true;
    }
    bool FMath::GetIntersectionLineFromPlane2(const FVector3& pP11, const FVector3& pP12, const FVector3& pP13, const FVector3& pP21, const FVector3& pP22, const FVector3& pP23, FSegment& segment)
    {
        return GetIntersectionLineFromPlane2(pP11, pP12, pP13, pP21, pP22, pP23, segment.m_P0, segment.m_P1);
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
    bool FMath::Direction_IsParallel(const FVector3& vDir1, const FVector3& vDir2)
    {
        float fLen = Length(Cross(vDir1, vDir2));
        if (fLen < FMath::ms_fEpsilon)
            return true;
        return false;
    }
    bool FMath::Direction_IsPerpendicular(const FVector3& vDir1, const FVector3& vDir2)
    {
        float fDot = Dot(vDir1, vDir2);
        if (Zero(fDot))
            return true;
        return false;
    }

    //Point - InLine
    bool FMath::Points3_InLine(const FVector3& pt1, const FVector3& pt2, const FVector3& pt3)
    {
        FVector3 v21 = pt2 - pt1;
        FVector3 v31 = pt3 - pt1;
        FVector3 vCross = Cross(v21, v31);
        if (Zero(Abs(Length(Cross(v21, v31)))))
            return true;
        return false;
    }

    //Point - InLineSameSide/NotInLineSameSide
    bool FMath::Points2_InLineSameSide(const FVector3& pt1, const FVector3& pt2, const FVector3& ptLine1, const FVector3& ptLine2)
    {
        FVector3 vLine12 = ptLine2 - ptLine1;
        FVector3 vLine1Pt1 = pt1 - ptLine1;
        FVector3 vLine1Pt2 = pt2 - ptLine1;
        float fDot = Dot(Cross(vLine12, vLine1Pt1), Cross(vLine12, vLine1Pt2));
        if (fDot > FMath::ms_fEpsilon)
            return true;
        return false;
    }
    bool FMath::Points2_InLineSameSide(const FVector3& pt1, const FVector3& pt2, const FSegment& segment)
    {
        return Points2_InLineSameSide(pt1, pt2, segment.m_P0, segment.m_P1);
    }
    bool FMath::Points2_NotInLineSameSide(const FVector3& pt1, const FVector3& pt2, const FVector3& ptLine1, const FVector3& ptLine2)
    {
        FVector3 vLine12 = ptLine2 - ptLine1;
        FVector3 vLine1Pt1 = pt1 - ptLine1;
        FVector3 vLine1Pt2 = pt2 - ptLine1;
        float fDot = Dot(Cross(vLine12, vLine1Pt1), Cross(vLine12, vLine1Pt2));
        if (fDot < -FMath::ms_fEpsilon)
            return true;
        return false;
    }
    bool FMath::Points2_NotInLineSameSide(const FVector3& pt1, const FVector3& pt2, const FSegment& segment)
    {
        return Points2_NotInLineSameSide(pt1, pt2, segment.m_P0, segment.m_P1);
    }

    //Point - OnPlane
    bool FMath::Points4_OnPlane(const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, const FVector3& pt4)
    {
        if (Zero(Dot(GetNormal3WithoutNormalizeFromPoints3(pt1, pt2, pt3), GetDirectionWithoutNormalizeFromPoint2(pt1, pt4))))
            return true;
        return false;
    }

    //Point - InPlaneSameSide/NotInPlaneSameSide
    bool FMath::Points2_InPlaneSameSide(const FVector3& pt1, const FVector3& pt2, const FVector3& ptPlane, const FVector3& vPlaneNormal)
    {
        FVector3 v1 = pt1 - ptPlane;
        FVector3 v2 = pt2 - ptPlane;
        float fDot = Dot(vPlaneNormal, v1) * Dot(vPlaneNormal, v2);
        if (fDot > FMath::ms_fEpsilon)
            return true;
        return false;
    }
    bool FMath::Points2_InPlaneSameSide(const FVector3& pt1, const FVector3& pt2, const FVector3& ptPlane1, const FVector3& ptPlane2, const FVector3& ptPlane3)
    {
        FVector3 vPlaneNormal = GetNormal3WithoutNormalizeFromPoints3(ptPlane1, ptPlane2, ptPlane3);
        return Points2_InPlaneSameSide(pt1, pt2, ptPlane1, vPlaneNormal);
    }
    bool FMath::Points2_InPlaneSameSide(const FVector3& pt1, const FVector3& pt2, const FPlane& plane)
    {
        FVector3 vNormal = FMath::Normalize(plane.GetNormal());
        FVector3 ptPlane = vNormal * plane.GetDistance();
        return Points2_InPlaneSameSide(pt1, pt2, ptPlane, vNormal);
    }
    bool FMath::Points2_NotInPlaneSameSide(const FVector3& pt1, const FVector3& pt2, const FVector3& ptPlane, const FVector3& vPlaneNormal)
    {
        FVector3 v1 = pt1 - ptPlane;
        FVector3 v2 = pt2 - ptPlane;
        float fDot = Dot(vPlaneNormal, v1) * Dot(vPlaneNormal, v2);
        if (fDot < -FMath::ms_fEpsilon)
            return true;
        return false;
    }
    bool FMath::Points2_NotInPlaneSameSide(const FVector3& pt1, const FVector3& pt2, const FVector3& ptPlane1, const FVector3& ptPlane2, const FVector3& ptPlane3)
    {
        FVector3 vPlaneNormal = GetNormal3WithoutNormalizeFromPoints3(ptPlane1, ptPlane2, ptPlane3);
        return Points2_InPlaneSameSide(pt1, pt2, ptPlane1, vPlaneNormal);
    }
    bool FMath::Points2_NotInPlaneSameSide(const FVector3& pt1, const FVector3& pt2, const FPlane& plane)
    {
        FVector3 vNormal = FMath::Normalize(plane.GetNormal());
        FVector3 ptPlane = vNormal * plane.GetDistance();
        return Points2_NotInPlaneSameSide(pt1, pt2, ptPlane, vNormal);
    }

    //Line - Line Parallel/NotParallel
    bool FMath::LineLine_Parallel(const FVector3& ptL_11, const FVector3& ptL_12, const FVector3& ptL_21, const FVector3& ptL_22)
    {
        FVector3 v1 = ptL_12 - ptL_11;
        FVector3 v2 = ptL_22 - ptL_21;
        if (Direction_IsParallel(v1, v2))
            return true;
        return false;
    }
    bool FMath::LineLine_Parallel(const FVector3& ptL_11, const FVector3& ptL_12, const FSegment& segment)
    {
        return LineLine_Parallel(ptL_11, ptL_12, segment.m_P0, segment.m_P1);
    }
    bool FMath::LineLine_Parallel(const FSegment& segment1, const FSegment& segment2)
    {
        return LineLine_Parallel(segment1.m_P0, segment1.m_P1, segment2.m_P0, segment2.m_P1);
    }
    bool FMath::LineLine_NotParallel(const FVector3& ptL_11, const FVector3& ptL_12, const FVector3& ptL_21, const FVector3& ptL_22)
    {
        return !LineLine_Parallel(ptL_11, ptL_12, ptL_21, ptL_22);
    }
    bool FMath::LineLine_NotParallel(const FVector3& ptL_11, const FVector3& ptL_12, const FSegment& segment)
    {
        return LineLine_NotParallel(ptL_11, ptL_12, segment.m_P0, segment.m_P1);
    }
    bool FMath::LineLine_NotParallel(const FSegment& segment1, const FSegment& segment2)
    {
        return LineLine_NotParallel(segment1.m_P0, segment1.m_P1, segment2.m_P0, segment2.m_P1);
    }

    //Line - Line Perpendicular/NotPerpendicular
    bool FMath::LineLine_Perpendicular(const FVector3& ptL_11, const FVector3& ptL_12, const FVector3& ptL_21, const FVector3& ptL_22)
    {
        FVector3 v1 = ptL_12 - ptL_11;
        FVector3 v2 = ptL_22 - ptL_21;
        if (Direction_IsPerpendicular(v1, v2))
            return true;
        return false;
    }
    bool FMath::LineLine_Perpendicular(const FVector3& ptL_11, const FVector3& ptL_12, const FSegment& segment)
    {
        return LineLine_Perpendicular(ptL_11, ptL_12, segment.m_P0, segment.m_P1);
    }
    bool FMath::LineLine_Perpendicular(const FSegment& segment1, const FSegment& segment2)
    {
        return LineLine_Perpendicular(segment1.m_P0, segment1.m_P1, segment2.m_P0, segment2.m_P1);
    }
    bool FMath::LineLine_NotPerpendicular(const FVector3& ptL_11, const FVector3& ptL_12, const FVector3& ptL_21, const FVector3& ptL_22)
    {
        return !LineLine_Perpendicular(ptL_11, ptL_12, ptL_21, ptL_22);
    }
    bool FMath::LineLine_NotPerpendicular(const FVector3& ptL_11, const FVector3& ptL_12, const FSegment& segment)
    {
        return LineLine_NotPerpendicular(ptL_11, ptL_12, segment.m_P0, segment.m_P1); 
    }
    bool FMath::LineLine_NotPerpendicular(const FSegment& segment1, const FSegment& segment2)
    {
        return LineLine_NotPerpendicular(segment1.m_P0, segment1.m_P1, segment2.m_P0, segment2.m_P1);
    }

    //Line - Line Intersect/NotIntersect
    bool FMath::LineLine_Intersect(const FVector3& ptL_11, const FVector3& ptL_12, const FVector3& ptL_21, const FVector3& ptL_22, bool includeBorder /*= true*/)
    {
        if (!Points4_OnPlane(ptL_11, ptL_12, ptL_21, ptL_22))
            return false;

        bool isIntersect = false;
        if (includeBorder)
        {
            if (!Points3_InLine(ptL_11, ptL_12, ptL_21) || !Points3_InLine(ptL_11, ptL_12, ptL_22))
                return !Points2_InLineSameSide(ptL_11, ptL_12, ptL_21, ptL_22) && !Points2_InLineSameSide(ptL_12, ptL_22, ptL_11, ptL_12);

            isIntersect = Intersects_PointInLine(ptL_11, ptL_21, ptL_22) || 
                          Intersects_PointInLine(ptL_12, ptL_21, ptL_22) || 
                          Intersects_PointInLine(ptL_21, ptL_11, ptL_12) || 
                          Intersects_PointInLine(ptL_22, ptL_11, ptL_12);
        }
        else
        {
            isIntersect = Points2_NotInLineSameSide(ptL_11, ptL_12, ptL_21, ptL_22) &&
                          Points2_NotInLineSameSide(ptL_21, ptL_22, ptL_11, ptL_12);
        }
        return isIntersect;
    }
    bool FMath::LineLine_Intersect(const FVector3& ptL_11, const FVector3& ptL_12, const FSegment& segment, bool includeBorder /*= true*/)
    {
        return LineLine_Intersect(ptL_11, ptL_12, segment.m_P0, segment.m_P1, includeBorder);
    }
    bool FMath::LineLine_Intersect(const FSegment& segment1, const FSegment& segment2, bool includeBorder /*= true*/)
    {
        return LineLine_Intersect(segment1.m_P0, segment1.m_P1, segment2.m_P0, segment2.m_P1, includeBorder);
    }
    bool FMath::LineLine_NotIntersect(const FVector3& ptL_11, const FVector3& ptL_12, const FVector3& ptL_21, const FVector3& ptL_22, bool includeBorder /*= true*/)
    {
        return !LineLine_Intersect(ptL_11, ptL_12, ptL_21, ptL_22, includeBorder);
    }
    bool FMath::LineLine_NotIntersect(const FVector3& ptL_11, const FVector3& ptL_12, const FSegment& segment, bool includeBorder /*= true*/)
    {
        return LineLine_NotIntersect(ptL_11, ptL_12, segment.m_P0, segment.m_P1, includeBorder); 
    }
    bool FMath::LineLine_NotIntersect(const FSegment& segment1, const FSegment& segment2, bool includeBorder /*= true*/)
    {
        return LineLine_NotIntersect(segment1.m_P0, segment1.m_P1, segment2.m_P0, segment2.m_P1, includeBorder);
    }

    //Line - Plane Parallel/NotParallel
    bool FMath::LinePlane_Parallel(const FVector3& vDir, const FVector3& vNormal)
    {
        if (Direction_IsParallel(vDir, vNormal))
            return true;
        return false;
    }
    bool FMath::LinePlane_Parallel(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3)
    {
        FVector3 vDir = ptLine2 - ptLine1;
        FVector3 vNormal = GetNormal3WithoutNormalizeFromPoints3(pt1, pt2, pt3);
        return LinePlane_Parallel(vDir, vNormal);
    }
    bool FMath::LinePlane_Parallel(const FVector3& ptLine1, const FVector3& ptLine2, const FPlane& plane)
    {
        FVector3 vDir = ptLine2 - ptLine1;
        return LinePlane_Parallel(vDir, plane.GetNormal());
    }
    bool FMath::LinePlane_Parallel(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3)
    {
        return LinePlane_Parallel(segment.m_P0, segment.m_P1, pt1, pt2, pt3);
    }
    bool FMath::LinePlane_Parallel(const FSegment& segment, const FPlane& plane)
    {
        return LinePlane_Parallel(segment.GetDirection(), plane.GetNormal());
    }
    bool FMath::LinePlane_NotParallel(const FVector3& vDir, const FVector3& vNormal)
    {
        return !LinePlane_Parallel(vDir, vNormal);
    }
    bool FMath::LinePlane_NotParallel(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3)
    {
        return !LinePlane_Parallel(ptLine1, ptLine2, pt1, pt2, pt3);
    }
    bool FMath::LinePlane_NotParallel(const FVector3& ptLine1, const FVector3& ptLine2, const FPlane& plane)
    {
        FVector3 vDir = ptLine2 - ptLine1;
        return LinePlane_NotParallel(vDir, plane.GetNormal());
    }
    bool FMath::LinePlane_NotParallel(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3)
    {
        return LinePlane_NotParallel(segment.m_P0, segment.m_P1, pt1, pt2, pt3);
    }
    bool FMath::LinePlane_NotParallel(const FSegment& segment, const FPlane& plane)
    {
        return LinePlane_NotParallel(segment.GetDirection(), plane.GetNormal());
    }

    //Line - Plane Perpendicular/NotPerpendicular
    bool FMath::LinePlane_Perpendicular(const FVector3& vDir, const FVector3& vNormal)
    {
        if (Direction_IsPerpendicular(vDir, vNormal))
            return true;
        return false;
    }
    bool FMath::LinePlane_Perpendicular(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3)
    {
        FVector3 vDir = ptLine2 - ptLine1;
        FVector3 vNormal = GetNormal3WithoutNormalizeFromPoints3(pt1, pt2, pt3);
        return LinePlane_Perpendicular(vDir, vNormal);
    }
    bool FMath::LinePlane_Perpendicular(const FVector3& ptLine1, const FVector3& ptLine2, const FPlane& plane)
    {
        FVector3 vDir = ptLine2 - ptLine1;
        return LinePlane_Perpendicular(vDir, plane.GetNormal());
    }
    bool FMath::LinePlane_Perpendicular(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3)
    {
        return LinePlane_Perpendicular(segment.m_P0, segment.m_P1, pt1, pt2, pt3);
    }
    bool FMath::LinePlane_Perpendicular(const FSegment& segment, const FPlane& plane)
    {
        return LinePlane_Perpendicular(segment.GetDirection(), plane.GetNormal());
    }
    bool FMath::LinePlane_NotPerpendicular(const FVector3& vDir, const FVector3& vNormal)
    {
        return !LinePlane_Perpendicular(vDir, vNormal);
    }
    bool FMath::LinePlane_NotPerpendicular(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3)
    {
        return !LinePlane_NotPerpendicular(ptLine1, ptLine2, pt1, pt2, pt3);
    }
    bool FMath::LinePlane_NotPerpendicular(const FVector3& ptLine1, const FVector3& ptLine2, const FPlane& plane)
    {
        FVector3 vDir = ptLine2 - ptLine1;
        return LinePlane_NotPerpendicular(vDir, plane.GetNormal());
    }
    bool FMath::LinePlane_NotPerpendicular(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3)
    {
        return LinePlane_NotPerpendicular(segment.m_P0, segment.m_P1, pt1, pt2, pt3);
    }
    bool FMath::LinePlane_NotPerpendicular(const FSegment& segment, const FPlane& plane)
    {
        return LinePlane_NotPerpendicular(segment.GetDirection(), plane.GetNormal());
    }

    //Line - Triangle Intersect/NotIntersect
    bool FMath::LineTriangle_Intersect(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, bool includeBorder /*= true*/)
    {
        if (includeBorder)
        {
            if (!Points2_InPlaneSameSide(ptLine1, ptLine2, pt1, pt2, pt3) &&
                !Points2_InPlaneSameSide(pt1, pt2, ptLine1, ptLine2, pt3) &&
                !Points2_InPlaneSameSide(pt2, pt3, ptLine1, ptLine2, pt1) &&
                !Points2_InPlaneSameSide(pt3, pt1, ptLine1, ptLine2, pt2))
            {
                return true; 
            }
        }
        else
        {
            if (!Points2_NotInPlaneSameSide(ptLine1, ptLine2, pt1, pt2, pt3) &&
                !Points2_NotInPlaneSameSide(pt1, pt2, ptLine1, ptLine2, pt3) &&
                !Points2_NotInPlaneSameSide(pt2, pt3, ptLine1, ptLine2, pt1) &&
                !Points2_NotInPlaneSameSide(pt3, pt1, ptLine1, ptLine2, pt2))
            {
                return true; 
            }
        }
        return false;
    }
    bool FMath::LineTriangle_Intersect(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, bool includeBorder /*= true*/)
    {
        return LineTriangle_Intersect(segment.m_P0, segment.m_P1, pt1, pt2, pt3, includeBorder);
    }
    bool FMath::LineTriangle_NotIntersect(const FVector3& ptLine1, const FVector3& ptLine2, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, bool includeBorder /*= true*/)
    {
        return !LineTriangle_Intersect(ptLine1, ptLine2, pt1, pt2, pt3, includeBorder);
    }
    bool FMath::LineTriangle_NotIntersect(const FSegment& segment, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, bool includeBorder /*= true*/)
    {
        return LineTriangle_NotIntersect(segment.m_P0, segment.m_P1, pt1, pt2, pt3, includeBorder);
    }

    //Plane - Plane Parallel/NotParallel
    bool FMath::PlanePlane_Parallel(const FVector3& pP11, const FVector3& pP12, const FVector3& pP13, const FVector3& pP21, const FVector3& pP22, const FVector3& pP23)
    {
        FVector3 vNormal1 = GetNormal3WithoutNormalizeFromPoints3(pP11, pP12, pP13);
        FVector3 vNormal2 = GetNormal3WithoutNormalizeFromPoints3(pP21, pP22, pP23);
        return Direction_IsParallel(vNormal1, vNormal2);
    }
    bool FMath::PlanePlane_NotParallel(const FVector3& pP11, const FVector3& pP12, const FVector3& pP13, const FVector3& pP21, const FVector3& pP22, const FVector3& pP23)
    {
        return !PlanePlane_Parallel(pP11, pP12, pP13, pP21, pP22, pP23);
    }
    bool FMath::PlanePlane_Parallel(const FPlane& plane1, const FPlane& plane2)
    {
        return Direction_IsParallel(plane1.GetNormal(), plane2.GetNormal());
    }
    bool FMath::PlanePlane_NotParallel(const FPlane& plane1, const FPlane& plane2)
    {
        return !PlanePlane_Parallel(plane1, plane2);
    }

    //Plane - Plane Perpendicular/NotPerpendicular
    bool FMath::PlanePlane_Perpendicular(const FVector3& pP11, const FVector3& pP12, const FVector3& pP13, const FVector3& pP21, const FVector3& pP22, const FVector3& pP23)
    {
        FVector3 vNormal1 = GetNormal3WithoutNormalizeFromPoints3(pP11, pP12, pP13);
        FVector3 vNormal2 = GetNormal3WithoutNormalizeFromPoints3(pP21, pP22, pP23);
        return Direction_IsPerpendicular(vNormal1, vNormal2);
    }
    bool FMath::PlanePlane_NotPerpendicular(const FVector3& pP11, const FVector3& pP12, const FVector3& pP13, const FVector3& pP21, const FVector3& pP22, const FVector3& pP23)
    {
        return !PlanePlane_Perpendicular(pP11, pP12, pP13, pP21, pP22, pP23);
    }
    bool FMath::PlanePlane_Perpendicular(const FPlane& plane1, const FPlane& plane2)
    {
        return Direction_IsPerpendicular(plane1.GetNormal(), plane2.GetNormal());
    }
    bool FMath::PlanePlane_NotPerpendicular(const FPlane& plane1, const FPlane& plane2)
    {
        return !PlanePlane_Perpendicular(plane1, plane2);
    }

    //Point - Line
    bool FMath::Intersects_PointInLine(const FVector3& pt, const FVector3& ptLineStart, const FVector3& ptLineEnd, bool includeSE /*= true*/)
    {
        //Is In Line
        if (!Points3_InLine(ptLineStart, ptLineEnd, pt))
        {
            return false;
        }

        if (!includeSE)
        {
            //Except ptLineStart/ptLineEnd
            if (IsEqual(pt, ptLineStart) || IsEqual(pt, ptLineEnd))
            {
                return false;
            }
        }

        //pt is between ptLineStart and ptLineEnd
        if (Zero((ptLineStart.x - pt.x)*(ptLineEnd.x - pt.x)) &&
            Zero((ptLineStart.y - pt.y)*(ptLineEnd.y - pt.y)) &&
            Zero((ptLineStart.z - pt.z)*(ptLineEnd.z - pt.z)))
        {
            return true;
        }

        return false;
    }
    bool FMath::Intersects_PointInLine(const FVector3& pt, const FSegment& segment, bool includeSE /*= true*/)
    {
        return Intersects_PointInLine(pt, segment.m_P0, segment.m_P1, includeSE);
    }

    //Point - Triangle
    bool FMath::Intersects_PointInTriangle(const FVector3& pt, const FVector3& a, const FVector3& b, const FVector3& c, bool includeBorder /*= true*/)
    {
        FVector3 vAB = b - a;
        FVector3 vAC = c - a;
        FVector3 vPA = a - pt;
        FVector3 vPB = b - pt;
        FVector3 vPC = c - pt;
        if (!includeBorder)
        {
            if (Points3_InLine(pt, a, b) ||
                Points3_InLine(pt, b, c) ||
                Points3_InLine(pt, c, a))
            {
                return false;
            }
        }
        float fValue = Length(Cross(vAB, vAC)) - Length(Cross(vPA, vPB)) - Length(Cross(vPB, vPC)) - Length(Cross(vPC, vPA));
        if (Zero(fValue))
            return true;
        return false;
    }

    //Point - Rect
    bool FMath::Intersects_PointInRect(const FVector3& pt, const FVector3& rtLeftTop, const FVector3& rtLeftBottom, const FVector3& rtRightTop, const FVector3& rtRightBottom)
    {
        
        return false;
    }

    //Point - Circle
    bool FMath::Intersects_PointInCircle(const FVector3& pt, const FVector3& center, float radius)
    {
        
        return false;
    }

    //Ray - Shape
    std::pair<bool, float>FMath::Intersects_RaySegment(const FRay& ray, const FVector3& s, const FVector3& e)
    {
        FVector3 vIntersection;
        FVector3 vDir = ray.GetPoint(ms_fPosInfinity);
        if (GetIntersectionPointFromLine2(ray.GetOrigin(), vDir, s, e, vIntersection))
        {
            return std::pair<bool, float>(true, ray.GetDistance(vIntersection));
        }
        return std::pair<bool, float>(false, 0);
    }
    std::pair<bool, float> FMath::Intersects_RaySegment(const FRay& ray, const FSegment& segment)
    {
        return Intersects_RaySegment(ray, segment.m_P0, segment.m_P1);
    }

    std::pair<bool, float> FMath::Intersects_RayTriangle(const FRay& ray, const FVector3& a, const FVector3& b, const FVector3& c,
                                                         bool positiveSide /*= true*/, bool negativeSide /*= true*/)
    {
        FVector3 vNormal = GetNormal3WithoutNormalizeFromPoints3(a, b, c);
        return Intersects_RayTriangle(ray, a, b, c, vNormal, positiveSide, negativeSide);
    }

    std::pair<bool, float> FMath::Intersects_RayTriangle(const FRay& ray, const FVector3& a, const FVector3& b, const FVector3& c, 
                                                         const FVector3& normal, bool positiveSide /*= true*/, bool negativeSide /*= true*/)
    {
        float t;
        {
            float denom = FMath::Dot(normal, ray.GetDirection());
            if (denom > + FMath::ms_fEpsilon)
            {
                if (!negativeSide)
                    return std::pair<bool, float>(false, 0);
            }
            else if (denom < - FMath::ms_fEpsilon)
            {
                if (!positiveSide)
                    return std::pair<bool, float>(false, 0);
            }
            else
            {
                return std::pair<bool, float>(false, 0);
            }

            t = FMath::Dot(normal, a - ray.GetOrigin()) / denom;
            if (t < 0)
            {
                return std::pair<bool, float>(false, 0);
            }
        }

        int i0, i1;
        {
            float n0 = FMath::Abs(normal[0]);
            float n1 = FMath::Abs(normal[1]);
            float n2 = FMath::Abs(normal[2]);

            i0 = 1; i1 = 2;
            if (n1 > n2)
            {
                if (n1 > n0) 
                    i0 = 0;
            }
            else
            {
                if (n2 > n0) 
                    i1 = 0;
            }
        }

        {
            float u1 = b[i0] - a[i0];
            float v1 = b[i1] - a[i1];
            float u2 = c[i0] - a[i0];
            float v2 = c[i1] - a[i1];
            float u0 = t * ray.GetDirection()[i0] + ray.GetOrigin()[i0] - a[i0];
            float v0 = t * ray.GetDirection()[i1] + ray.GetOrigin()[i1] - a[i1];

            float alpha = u0 * v2 - u2 * v0;
            float beta  = u1 * v0 - u0 * v1;
            float area  = u1 * v2 - u2 * v1;

            const float EPSILON = 1e-6f;

            float tolerance = - EPSILON * area;

            if (area > 0)
            {
                if (alpha < tolerance || beta < tolerance || alpha+beta > area-tolerance)
                    return std::pair<bool, float>(false, 0);
            }
            else
            {
                if (alpha > tolerance || beta > tolerance || alpha+beta < area-tolerance)
                    return std::pair<bool, float>(false, 0);
            }
        }

        return std::pair<bool, float>(true, t);
    }

    std::pair<bool, float> FMath::Intersects_RayQuad(const FRay& ray, const FVector3& a, const FVector3& b, 
                                                     const FVector3& c, const FVector3& d)
    {
        const FVector3& vRayOri = ray.m_vOrigin;
        const FVector3& vRayDir = ray.m_vDirection;
        FVector3 vOA = a - vRayOri;
        FVector3 vOB = b - vRayOri;
        FVector3 vOC = c - vRayOri;
        FVector3 vCross = FMath::Cross(vOC, vRayDir);
        float fDotV = FMath::Dot(vOA, vCross);
        if (fDotV >= 0.0f)
        {
            //abc
            return Intersects_RayTriangle(ray, a, b, c);
        }
        else
        {
            //dac
            return Intersects_RayTriangle(ray, d, a, c);
        }
        return std::pair<bool, float>(false, 0);
    }

    std::pair<bool, float> FMath::Intersects_RayCircle(const FRay& ray, const FPlane& plane, const FVector3& center, float radius)
    {
        std::pair<bool, float> ret = Intersects_RayPlane(ray, plane);
        if (!ret.first)
        {
            return ret;
        }
        FVector3 vInter = ray.GetPoint(ret.second);
        if (FMath::Distance2(center, vInter) <= radius * radius)
        {
            return ret;
        }
        return std::pair<bool, float>(false, 0);
    }

    std::pair<bool, float> FMath::Intersects_RayPlane(const FRay& ray, const FPlane& plane)
    {
        float denom = FMath::Dot(plane.m_vNormal, ray.GetDirection());
        if (FMath::Abs(denom) < FMath::ms_fEpsilon)
        {
            return std::pair<bool, float>(false, 0);
        }
        else
        {
            float nom = FMath::Dot(plane.m_vNormal, ray.GetOrigin()) + plane.m_fDistance;
            float t = -(nom/denom);
            return std::pair<bool, float>(t >= 0, t);
        }
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

    std::pair<bool, float> FMath::Intersects_RayAABB(const FRay& ray, const FAABB& aabb)
    {
        if (!aabb.IsValid()) 
            return std::pair<bool, float>(false, 0);

        float lowt = 0.0f;
        float t;
        bool hit = false;
        FVector3 hitpoint;
        const FVector3& min = aabb.GetMin();
        const FVector3& max = aabb.GetMax();
        const FVector3& rayorig = ray.GetOrigin();
        const FVector3& raydir = ray.GetDirection();

        //1> Check origin inside first
        if (FMath::IsGreat(rayorig, min) &&  FMath::IsLess(rayorig, max))
        {
            return std::pair<bool, float>(true, 0);
        }

        //2> Check each face in turn, only check closest 3
        // Min x
        if (rayorig.x <= min.x && raydir.x > 0)
        {
            t = (min.x - rayorig.x) / raydir.x;
            if (t >= 0)
            {
                hitpoint = rayorig + raydir * t;
                if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
                    hitpoint.z >= min.z && hitpoint.z <= max.z &&
                    (!hit || t < lowt))
                {
                    hit = true;
                    lowt = t;
                }
            }
        }
        // Max x
        if (rayorig.x >= max.x && raydir.x < 0)
        {
            t = (max.x - rayorig.x) / raydir.x;
            if (t >= 0)
            {
                hitpoint = rayorig + raydir * t;
                if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
                    hitpoint.z >= min.z && hitpoint.z <= max.z &&
                    (!hit || t < lowt))
                {
                    hit = true;
                    lowt = t;
                }
            }
        }
        // Min y
        if (rayorig.y <= min.y && raydir.y > 0)
        {
            t = (min.y - rayorig.y) / raydir.y;
            if (t >= 0)
            {
                hitpoint = rayorig + raydir * t;
                if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
                    hitpoint.z >= min.z && hitpoint.z <= max.z &&
                    (!hit || t < lowt))
                {
                    hit = true;
                    lowt = t;
                }
            }
        }
        // Max y
        if (rayorig.y >= max.y && raydir.y < 0)
        {
            t = (max.y - rayorig.y) / raydir.y;
            if (t >= 0)
            {
                hitpoint = rayorig + raydir * t;
                if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
                    hitpoint.z >= min.z && hitpoint.z <= max.z &&
                    (!hit || t < lowt))
                {
                    hit = true;
                    lowt = t;
                }
            }
        }
        // Min z
        if (rayorig.z <= min.z && raydir.z > 0)
        {
            t = (min.z - rayorig.z) / raydir.z;
            if (t >= 0)
            {
                hitpoint = rayorig + raydir * t;
                if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
                    hitpoint.y >= min.y && hitpoint.y <= max.y &&
                    (!hit || t < lowt))
                {
                    hit = true;
                    lowt = t;
                }
            }
        }
        // Max z
        if (rayorig.z >= max.z && raydir.z < 0)
        {
            t = (max.z - rayorig.z) / raydir.z;
            if (t >= 0)
            {
                hitpoint = rayorig + raydir * t;
                if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
                    hitpoint.y >= min.y && hitpoint.y <= max.y &&
                    (!hit || t < lowt))
                {
                    hit = true;
                    lowt = t;
                }
            }
        }

        return std::pair<bool, float>(hit, lowt);
    }

    std::pair<bool, float> FMath::Intersects_RaySphere(const FRay& ray, const FSphere& sphere, bool discardInside /*= true*/)		
    {
        const FVector3& raydir = ray.GetDirection();
        const FVector3& rayorig = ray.GetOrigin() - sphere.GetCenter();
        float radius = sphere.GetRadius();

        if (FMath::Length2(rayorig) <= radius * radius && discardInside)
        {
            return std::pair<bool, float>(true, 0);
        }

        // ie t = (-b +/- sqrt(b*b + 4ac)) / 2a
        float a = FMath::Dot(raydir, raydir);
        float b = 2 * FMath::Dot(rayorig, raydir);
        float c = FMath::Dot(rayorig, rayorig) - radius * radius;

        float d = (b*b) - (4 * a * c);
        if (d < 0)
        {
            return std::pair<bool, float>(false, 0);
        }
        else
        {
            float t = ( -b - FMath::Sqrt(d) ) / (2 * a);
            if (t < 0)
                t = ( -b + FMath::Sqrt(d) ) / (2 * a);
            return std::pair<bool, float>(true, t);
        }
    }

    std::pair<bool, float> FMath::Intersects_RayCylinder(const FRay& ray, const FCylinder& cylinder, bool discardInside /*= true*/)
    {   

        return std::pair<bool, float>(true, 0);
    }

    std::pair<bool, float> FMath::Intersects_RayCapsule(const FRay& ray, const FCapsule& capsule, bool discardInside /*= true*/)
    {

        return std::pair<bool, float>(true, 0);
    }

    std::pair<bool, float> FMath::Intersects_RayCone(const FRay& ray, const FCone& cone, bool discardInside /*= true*/)
    {

        return std::pair<bool, float>(true, 0);
    }

    std::pair<bool, float> FMath::Intersects_RayTorus(const FRay& ray, const FTorus& torus, bool discardInside /*= true*/)
    {

        return std::pair<bool, float>(true, 0);
    }


    bool FMath::Intersects_RaySegment_Test(const FRay& ray, const FVector3& s, const FVector3& e)
    {
        std::pair<bool, float> ret = Intersects_RaySegment(ray, s, e);
        return ret.first;
    }
    bool FMath::Intersects_RaySegment_Test(const FRay& ray, const FSegment& segment)
    {
        std::pair<bool, float> ret = Intersects_RaySegment(ray, segment);
        return ret.first;
    }
    bool FMath::Intersects_RayTriangle_Test(const FRay& ray, const FVector3& a, const FVector3& b, const FVector3& c,
                                            bool positiveSide /*= true*/, bool negativeSide /*= true*/)
    {
        std::pair<bool, float> ret = Intersects_RayTriangle(ray, a, b, c, positiveSide, negativeSide);
        return ret.first;
    }
    bool FMath::Intersects_RayQuad_Test(const FRay& ray, const FVector3& a, const FVector3& b, 
                                        const FVector3& c, const FVector3& d)
    {
        std::pair<bool, float> ret = Intersects_RayQuad(ray, a, b, c, d);
        return ret.first;
    }
    bool FMath::Intersects_RayCircle_Test(const FRay& ray, const FPlane& plane, const FVector3& center, float radius)
    {
        std::pair<bool, float> ret = Intersects_RayCircle(ray, plane, center, radius);
        return ret.first;
    }
    bool FMath::Intersects_RayAABB_Test(const FRay& ray, const FAABB& aabb)
    {   
        std::pair<bool, float> ret = Intersects_RayAABB(ray, aabb);
        return ret.first;
    }
    bool FMath::Intersects_RaySphere_Test(const FRay& ray, const FSphere& sphere, bool discardInside /*= true*/)
    {
        std::pair<bool, float> ret = Intersects_RaySphere(ray, sphere, discardInside);
        return ret.first;
    }
    bool FMath::Intersects_RayCylinder_Test(const FRay& ray, const FCylinder& cylinder, bool discardInside /*= true*/)
    {
        std::pair<bool, float> ret = Intersects_RayCylinder(ray, cylinder, discardInside);
        return ret.first;
    }
    bool FMath::Intersects_RayCapsule_Test(const FRay& ray, const FCapsule& capsule, bool discardInside /*= true*/)
    {
        std::pair<bool, float> ret = Intersects_RayCapsule(ray, capsule, discardInside);
        return ret.first;
    }
    bool FMath::Intersects_RayCone_Test(const FRay& ray, const FCone& cone, bool discardInside /*= true*/)
    {
        std::pair<bool, float> ret = Intersects_RayCone(ray, cone, discardInside);
        return ret.first;
    }
    bool FMath::Intersects_RayTorus_Test(const FRay& ray, const FTorus& torus, bool discardInside /*= true*/)
    {
        std::pair<bool, float> ret = Intersects_RayTorus(ray, torus, discardInside);
        return ret.first;
    }   


    static bool s_CheckAxis(const FVector3& rayorig, 
                            const FVector3& raydir,
                            const FVector3& min,
                            const FVector3& max,
                            int32 iAxis,
                            float& start,
                            float& end)
    {
        float denom = 1 / raydir[iAxis];
        float newstart = (min[iAxis] - rayorig[iAxis]) * denom;	
        float newend = (max[iAxis] - rayorig[iAxis]) * denom;
        if (newstart > newend) 
            std::swap(newstart, newend);		
        if (newstart > end || newend < start) 
            return false;		
        if (newstart > start) 
            start = newstart;					
        if (newend < end) 
            end = newend;
        return true;
    }
    bool FMath::Intersects_RayAABB_Test(const FRay& ray, const FAABB& aabb, float* d1, float* d2)
    {
        if (!aabb.IsValid())
            return false;

        const FVector3& min = aabb.GetMin();
        const FVector3& max = aabb.GetMax();
        const FVector3& rayorig = ray.GetOrigin();
        const FVector3& raydir = ray.GetDirection();

        FVector3 absDir;
        absDir[0] = FMath::Abs(raydir[0]);
        absDir[1] = FMath::Abs(raydir[1]);
        absDir[2] = FMath::Abs(raydir[2]);

        // Sort the axis, ensure check minimise floating error axis first
        int32 imax = 0, imid = 1, imin = 2;
        if (absDir[0] < absDir[2])
        {
            imax = 2;
            imin = 0;
        }
        if (absDir[1] < absDir[imin])
        {
            imid = imin;
            imin = 1;
        }
        else if (absDir[1] > absDir[imax])
        {
            imid = imax;
            imax = 1;
        }

        float start = 0;
        float end = FMath::ms_fPosInfinity;
        // Check each axis in turn
        if (!s_CheckAxis(rayorig,
                         raydir,
                         min,
                         max,
                         imax,
                         start,
                         end))
        {
            return false;
        }

        if (absDir[imid] < FMath::ms_fEpsilon)
        {
            // Parallel with middle and minimise axis, check bounds only
            if (rayorig[imid] < min[imid] || rayorig[imid] > max[imid] ||
                rayorig[imin] < min[imin] || rayorig[imin] > max[imin])
            {
                return false;
            }
        }
        else
        {
            if (!s_CheckAxis(rayorig,
                             raydir,
                             min,
                             max,
                             imid,
                             start,
                             end))
            {
                return false;
            }

            if (absDir[imin] < FMath::ms_fEpsilon)
            {
                // Parallel with minimise axis, check bounds only
                if (rayorig[imin] < min[imin] || rayorig[imin] > max[imin])
                {
                    return false;
                }
            }
            else
            {
                if (!s_CheckAxis(rayorig,
                                 raydir,
                                 min,
                                 max,
                                 imin,
                                 start,
                                 end))
                {
                    return false;
                }
            }
        }

        if (d1) *d1 = start;
        if (d2) *d2 = end;

        return true;
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