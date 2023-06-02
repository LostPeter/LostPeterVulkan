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

#include "../include/FPreInclude.h"
#include "../include/FMath.h"

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

    bool FMath::IsAffine(const FMatrix4& mat4)
    {
        return mat4[3][0] == 0.0f && mat4[3][1] == 0.0f && mat4[3][2] == 0.0f && mat4[3][3] == 1.0f;
    }
    FVector3 FMath::TransformAffine(const FMatrix4& mat4, const FVector3& v)
    {
        assert(FMath::IsAffine(mat4) && "FMath::TransformAffine");

		return FVector3(mat4[0][0] * v.x + mat4[0][1] * v.y + mat4[0][2] * v.z + mat4[0][3], 
                        mat4[1][0] * v.x + mat4[1][1] * v.y + mat4[1][2] * v.z + mat4[1][3],
                        mat4[2][0] * v.x + mat4[2][1] * v.y + mat4[2][2] * v.z + mat4[2][3]);
    }
	FVector3 FMath::TransformAffine(const FMatrix4& mat4, const FVector4& v)
    {
        assert(FMath::IsAffine(mat4) && "FMath::TransformAffine");

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
    
}; //LostPeterFoundation