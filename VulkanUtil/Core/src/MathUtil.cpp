#include "../include/PreInclude.h"
#include "../include/MathUtil.h"

namespace LostPeter
{
	const float MathUtil::ms_fPI_Half = glm::pi<float>() / 2.0f;
    const float MathUtil::ms_fPI = glm::pi<float>();
    const float MathUtil::ms_fPI_Two = glm::pi<float>() * 2.0f;
    const float MathUtil::ms_fDeg2Rad = MathUtil::ms_fPI / float(180.0f);		
    const float MathUtil::ms_fRad2Deg = float(180.0f) / MathUtil::ms_fPI;	
    const float MathUtil::ms_fLog2 = log(float(2.0f));

    const float MathUtil::ms_fPosInfinity =  std::numeric_limits<float>::infinity();	
    const float MathUtil::ms_fNegInfinity = -std::numeric_limits<float>::infinity();

    const glm::vec2 MathUtil::ms_v2Zero = glm::vec2(0.0f, 0.0f);
    const glm::vec2 MathUtil::ms_v2One = glm::vec2(1.0f, 1.0f);
    const glm::vec2 MathUtil::ms_v2UnitX = glm::vec2(1.0f, 0.0f);
    const glm::vec2 MathUtil::ms_v2UnitY = glm::vec2(0.0f, 1.0f);
    const glm::vec2 MathUtil::ms_v2UnitNegX = glm::vec2(-1.0f, 0.0f);
    const glm::vec2 MathUtil::ms_v2UnitNegY = glm::vec2(0.0f, -1.0f);

    const glm::vec3 MathUtil::ms_v3Zero = glm::vec3(0.0f, 0.0f, 0.0f);
    const glm::vec3 MathUtil::ms_v3One = glm::vec3(1.0f, 1.0f, 1.0f);
    const glm::vec3 MathUtil::ms_v3Centi = glm::vec3(1.0f / 100.f, 1.0f / 100.f, 1.0f / 100.f);
    const glm::vec3 MathUtil::ms_v3UnitX = glm::vec3(1.0f, 0.0f, 0.0f);
    const glm::vec3 MathUtil::ms_v3UnitY = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::vec3 MathUtil::ms_v3UnitZ = glm::vec3(0.0f, 0.0f, 1.0f);
    const glm::vec3 MathUtil::ms_v3UnitNegX = glm::vec3(-1.0f, 0.0f, 0.0f);
    const glm::vec3 MathUtil::ms_v3UnitNegY = glm::vec3(0.0f, -1.0f, 0.0f);
    const glm::vec3 MathUtil::ms_v3UnitNegZ = glm::vec3(0.0f, 0.0f, -1.0f);

    const glm::vec4 MathUtil::ms_v4Zero = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    const glm::vec4 MathUtil::ms_v4One = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    const glm::vec4 MathUtil::ms_v4UnitX = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
    const glm::vec4 MathUtil::ms_v4UnitY = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    const glm::vec4 MathUtil::ms_v4UnitZ = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
    const glm::vec4 MathUtil::ms_v4UnitW = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    const glm::vec4 MathUtil::ms_v4UnitNegX = glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f);
    const glm::vec4 MathUtil::ms_v4UnitNegY = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
    const glm::vec4 MathUtil::ms_v4UnitNegZ = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    const glm::vec4 MathUtil::ms_v4UnitNegW = glm::vec4(0.0f, 0.0f, 0.0f, -1.0f);

    const glm::quat MathUtil::ms_qUnit = glm::quat(glm::radians(glm::vec3(0.0f, 0.0f, 0.0f)));

    const glm::mat3 MathUtil::ms_mat3Zero = glm::mat3(0.0f, 0.0f, 0.0f,
                                                      0.0f, 0.0f, 0.0f,
                                                      0.0f, 0.0f, 0.0);
    const glm::mat3 MathUtil::ms_mat3Unit = glm::mat3(1.0f, 0.0f, 0.0f,
                                                      0.0f, 1.0f, 0.0f,
                                                      0.0f, 0.0f, 1.0);

    const glm::mat4 MathUtil::ms_mat4Zero = glm::mat4(0.0f, 0.0f, 0.0f, 0.0f,
                                                      0.0f, 0.0f, 0.0f, 0.0f,
                                                      0.0f, 0.0f, 0.0f, 0.0f,
                                                      0.0f, 0.0f, 0.0f, 0.0f);                             
    const glm::mat4 MathUtil::ms_mat4Unit = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
                                                      0.0f, 1.0f, 0.0f, 0.0f,
                                                      0.0f, 0.0f, 1.0f, 0.0f,
                                                      0.0f, 0.0f, 0.0f, 1.0f);

    const glm::vec4 MathUtil::ms_clBlack = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    const glm::vec4 MathUtil::ms_clWhite = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    const glm::vec4 MathUtil::ms_clRed = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    const glm::vec4 MathUtil::ms_clGreen = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    const glm::vec4 MathUtil::ms_clBlue = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    const glm::vec4 MathUtil::ms_clGray = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    const glm::vec4 MathUtil::ms_clDarkRed = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);
    const glm::vec4 MathUtil::ms_clDarkGreen = glm::vec4(0.0f, 0.5f, 0.0f, 1.0f);
    const glm::vec4 MathUtil::ms_clDarkBlue = glm::vec4(0.0f, 0.0f, 0.5f, 1.0f);
    const glm::vec4 MathUtil::ms_clDarkGray = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
    const glm::vec4 MathUtil::ms_clYellow = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
    const glm::vec4 MathUtil::ms_clCyan = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
    const glm::vec4 MathUtil::ms_clMagenta = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
    const glm::vec4 MathUtil::ms_clTransparent = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);

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
				theta += 2.0f * ms_fPI; // in [0, 2*pi).
		}

		// Quadrant II or III
		else
			theta = atanf(y / x) + ms_fPI; // in [0, 2*pi).

		return theta;
	}

	uint32 MathUtil::Power2PlusOne(uint32 number)
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

	bool MathUtil::IsPower2(size_t nValue, size_t& nNearestPow)
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


	glm::mat4 MathUtil::Translate(float x, float y, float z)
    {
        return Translate(glm::vec3(x, y, z));
    }
    glm::mat4 MathUtil::Translate(const glm::vec3& vTranslate)
    {
        return glm::translate(vTranslate);
    }

    glm::mat4 MathUtil::Translate(const glm::mat4& mat4, float x, float y, float z)
    {
        return Translate(mat4, glm::vec3(x, y, z));
    }
    glm::mat4 MathUtil::Translate(const glm::mat4& mat4, const glm::vec3& vTranslate)
    {
        return glm::translate(mat4, vTranslate);
    }


    glm::mat4 MathUtil::RotateX(float angleX)
    {
        float rad = glm::radians(angleX);
        return glm::eulerAngleX(rad);
    }
    glm::mat4 MathUtil::RotateY(float angleY)
    {
        float rad = glm::radians(angleY);
        return glm::eulerAngleY(rad);
    }
    glm::mat4 MathUtil::RotateZ(float angleZ)
    {
        float rad = glm::radians(angleZ);
        return glm::eulerAngleZ(rad);
    }
    glm::mat4 MathUtil::Rotate(float angleX, float angleY, float angleZ)
    {
        float radX = glm::radians(angleX);
        float radY = glm::radians(angleY);
        float radZ = glm::radians(angleZ);
        return glm::eulerAngleXYZ(radX, radY, radZ);
    }
    glm::mat4 MathUtil::Rotate(const glm::vec3& vAngle)
    {
        return Rotate(vAngle.x, vAngle.y, vAngle.z);
    }

    glm::mat4 MathUtil::Rotate(const glm::quat& qRot)
    {
        return glm::mat4_cast(qRot);
    }

    glm::mat4 MathUtil::Scale(float scaleX, float scaleY, float scaleZ)
    {
        return Scale(glm::vec3(scaleX, scaleY, scaleZ));
    }
    glm::mat4 MathUtil::Scale(const glm::vec3& vScale)
    {
        return glm::scale(vScale);
    }
    void MathUtil::Scale(glm::mat3& mat3, const glm::vec3& vScale)
    {
        mat3[0][0] *= vScale.x;	mat3[0][1] *= vScale.x;	mat3[0][2] *= vScale.x;
        mat3[1][0] *= vScale.y;	mat3[1][1] *= vScale.y;	mat3[1][2] *= vScale.y;
        mat3[2][0] *= vScale.z;	mat3[2][1] *= vScale.z;	mat3[2][2] *= vScale.z;
    }

    glm::mat4 MathUtil::FromTRS(const glm::vec3& vTranslate, const glm::vec3& vAngle, const glm::vec3& vScale)
    {
        // Scale -> Rotate -> Translate
        glm::mat4 matScale = MathUtil::Scale(vScale);
        glm::mat4 matRotate = MathUtil::Rotate(vAngle);
        glm::mat4 matTranslate = MathUtil::Translate(vTranslate);
        return matTranslate * matRotate * matScale;
    }

    glm::mat4 MathUtil::FromTQS(const glm::vec3& vTranslate, const glm::quat& qRot, const glm::vec3& vScale)
    {
        // Scale -> Rotate -> Translate
        glm::mat4 matScale = MathUtil::Scale(vScale);
        glm::mat4 matRotate = MathUtil::Rotate(qRot);
        glm::mat4 matTranslate = MathUtil::Translate(vTranslate);
        return matTranslate * matRotate * matScale;
    }

    void MathUtil::FromMatrix3(glm::mat3& mat3, glm::vec3& xAxis, glm::vec3& yAxis, glm::vec3& zAxis)
    {
        xAxis.x = mat3[0][0]; xAxis.y = mat3[0][1]; xAxis.z = mat3[0][2]; 
        yAxis.x = mat3[1][0]; yAxis.y = mat3[1][1]; yAxis.z = mat3[1][2];
        zAxis.x = mat3[2][0]; zAxis.y = mat3[2][1]; zAxis.z = mat3[2][2];
    }
    glm::mat3 MathUtil::ToMatrix3(const glm::vec3& xAxis, const glm::vec3& yAxis, const glm::vec3& zAxis)
    {
        return glm::mat3(xAxis.x, xAxis.y, xAxis.z,
                         yAxis.x, yAxis.y, yAxis.z,
                         zAxis.x, zAxis.y, zAxis.z);
    }
    glm::mat3 MathUtil::ToMatrix3(const glm::quat& qRot)
    {
        return glm::toMat3(qRot);
    }
    glm::mat3 MathUtil::ToMatrix3(const glm::mat4& mat4)
    {
        return glm::mat3(mat4[0][0], mat4[0][1], mat4[0][2],
                         mat4[1][0], mat4[1][1], mat4[1][2],
                         mat4[2][0], mat4[2][1], mat4[2][2]);
    }

    void MathUtil::FromMatrix4(glm::mat4& mat4, glm::vec3& xAxis, glm::vec3& yAxis, glm::vec3& zAxis)
    {
        glm::mat3 mat3 = MathUtil::ToMatrix3(mat4);
        MathUtil::FromMatrix3(mat3, xAxis, yAxis, zAxis);
    }
    glm::mat4 MathUtil::ToMatrix4(const glm::vec3& xAxis, const glm::vec3& yAxis, const glm::vec3& zAxis)
    {
        return glm::mat4(xAxis.x, xAxis.y, xAxis.z, 0,
                         yAxis.x, yAxis.y, yAxis.z, 0,
                         zAxis.x, zAxis.y, zAxis.z, 0,
                               0,       0,       0, 1);
    }
    glm::mat4 MathUtil::ToMatrix4(const glm::quat& qRot)
    {
        return glm::toMat4(qRot);
    }
    glm::mat4 MathUtil::ToMatrix4(const glm::vec3& vPos, const glm::vec3& vScale, const glm::quat& qRot)
    {
        // Scale -> Rotate-> Translate
        glm::mat4 mat4Scale = Scale(vScale);
        glm::mat4 mat4Rot = Rotate(qRot);
        glm::mat4 mat4Trans = Translate(vPos);
        glm::mat4 mat4 = mat4Trans * mat4Rot * mat4Scale;

		// No projection term
		mat4[0][3] = 0; mat4[1][3] = 0; mat4[2][3] = 0; mat4[3][3] = 1;

        return mat4;
    }   
    glm::mat4 MathUtil::ToMatrix4(const glm::mat3& mat3)
    {
        return glm::mat4(mat3[0][0], mat3[0][1], mat3[0][2], 0.0f,
                         mat3[1][0], mat3[1][1], mat3[1][2], 0.0f,
                         mat3[2][0], mat3[2][1], mat3[2][2], 0.0f,
                               0.0f,       0.0f,       0.0f, 1.0f);
    }

    glm::vec3 MathUtil::ToEulerAngles(const glm::quat& qRot)
    {
        glm::vec3 vEulerAngles(0, 0, 0);
        glm::extractEulerAngleYXZ(glm::toMat4(qRot), vEulerAngles.y, vEulerAngles.x, vEulerAngles.z);
        vEulerAngles.x = glm::degrees(vEulerAngles.x);
        vEulerAngles.y = glm::degrees(vEulerAngles.y);
        vEulerAngles.z = glm::degrees(vEulerAngles.z);
        return vEulerAngles;
    }

    glm::quat MathUtil::ToQuaternion(const glm::mat3& mat3)
    {
        return glm::toQuat(mat3);
    }
    glm::quat MathUtil::ToQuaternion(const glm::mat4& mat4)
    {
        return glm::toQuat(mat4);
    }
    glm::quat MathUtil::ToQuaternion(const glm::vec3& vEulerAngles)
    {
        glm::mat3 mat3 = glm::eulerAngleYXZ(glm::radians(vEulerAngles.y), glm::radians(vEulerAngles.x), glm::radians(vEulerAngles.z));
        return glm::toQuat(mat3);
    }
    glm::quat MathUtil::ToQuaternion(const glm::vec3& xAxis, const glm::vec3& yAxis, const glm::vec3& zAxis)
    {
        glm::mat3 mat3 = ToMatrix3(xAxis, yAxis, zAxis);
        return ToQuaternion(mat3);
    }
    glm::quat MathUtil::ToQuaternionFromAngleAxis(float fAngle, const glm::vec3& vAxis)
    {
        return glm::angleAxis(glm::radians(fAngle), vAxis);
    }
    glm::quat MathUtil::ToQuaternionFromAngleAxis(const glm::quat& qSrc, float fAngle, const glm::vec3& vAxis)
    {
        return glm::rotate(qSrc, glm::radians(fAngle), vAxis);
    }
    glm::quat MathUtil::ToQuaternionFromRadianAxis(float fRadian, const glm::vec3& vAxis)
    {
        return glm::angleAxis(fRadian, vAxis);
    }
    glm::quat MathUtil::ToQuaternionFromRadianAxis(const glm::quat& qSrc, float fRadian, const glm::vec3& vAxis)
    {
        return glm::rotate(qSrc, fRadian, vAxis);
    }
    glm::quat MathUtil::ToQuaternionFromSrc2Dst(const glm::vec3& vSrc, const glm::vec3& vDst, const glm::vec3& vFallbackAxis /*= MathUtil::ms_v3Zero*/)
    {
        glm::quat q = MathUtil::ms_qUnit;
		glm::vec3 v0 = MathUtil::Normalize(vSrc);
		glm::vec3 v1 = MathUtil::Normalize(vDst);

		float d = MathUtil::Dot(v0, v1);
		if (d >= 1.0f)
		{
			return q;
		}
		if (d < (1e-6f - 1.0f))
		{
			if (vFallbackAxis != MathUtil::ms_v3Zero)
			{
                q = MathUtil::ToQuaternionFromRadianAxis(q, MathUtil::ms_fPI, vFallbackAxis);
			}
			else
			{
				glm::vec3 vAxis = MathUtil::Cross(MathUtil::ms_v3UnitX, vSrc);
				if (MathUtil::IsZeroLength(vAxis)) 
					vAxis = MathUtil::Cross(MathUtil::ms_v3UnitY, vSrc);
                vAxis = MathUtil::Normalize(vAxis);
                q = MathUtil::ToQuaternionFromRadianAxis(q, MathUtil::ms_fPI, vAxis);
			}
		}
		else
		{
			float s = MathUtil::Sqrt((1 + d) * 2);
			float fInv = 1.0f / s;

			glm::vec3 c = MathUtil::Cross(v0, v1);
			q.x = c.x * fInv;
			q.y = c.y * fInv;
			q.z = c.z * fInv;
			q.w = s * 0.5f;
            q = MathUtil::Normalize(q);
		}
		return q;
    }

    void MathUtil::ToAxes(const glm::quat& qRot, glm::vec3& xAxis, glm::vec3& yAxis, glm::vec3& zAxis)
    {
        glm::mat3 mat3 = glm::toMat3(qRot);
        MathUtil::FromMatrix3(mat3, xAxis, yAxis, zAxis);
    }
    void MathUtil::ToAngleAxis(const glm::quat& qRot, float& fAngle, glm::vec3& vAxis)
    {
        fAngle = glm::degrees(glm::angle(qRot));
        vAxis = glm::axis(qRot);
    }

    glm::vec3 MathUtil::Transform(const glm::mat4& mat4, const glm::vec3& v)
    {
        glm::vec4 vRet = mat4 * glm::vec4(v.x, v.y, v.z, 1.0f);
        return glm::vec3(vRet.x, vRet.y, vRet.z);
    }
    glm::vec3 MathUtil::Transform(const glm::quat& qRot, const glm::vec3& v)
    {
        return glm::rotate(qRot, v);
    }

    bool MathUtil::IsAffine(const glm::mat4& mat4)
    {
        return mat4[3][0] == 0.0f && mat4[3][1] == 0.0f && mat4[3][2] == 0.0f && mat4[3][3] == 1.0f;
    }
    glm::vec3 MathUtil::TransformAffine(const glm::mat4& mat4, const glm::vec3& v)
    {
        assert(MathUtil::IsAffine(mat4) && "MathUtil::TransformAffine");

		return glm::vec3(mat4[0][0] * v.x + mat4[0][1] * v.y + mat4[0][2] * v.z + mat4[0][3], 
                         mat4[1][0] * v.x + mat4[1][1] * v.y + mat4[1][2] * v.z + mat4[1][3],
                         mat4[2][0] * v.x + mat4[2][1] * v.y + mat4[2][2] * v.z + mat4[2][3]);
    }
	glm::vec3 MathUtil::TransformAffine(const glm::mat4& mat4, const glm::vec4& v)
    {
        assert(MathUtil::IsAffine(mat4) && "MathUtil::TransformAffine");

		return glm::vec4(mat4[0][0] * v.x + mat4[0][1] * v.y + mat4[0][2] * v.z + mat4[0][3] * v.w, 
                         mat4[1][0] * v.x + mat4[1][1] * v.y + mat4[1][2] * v.z + mat4[1][3] * v.w,
                         mat4[2][0] * v.x + mat4[2][1] * v.y + mat4[2][2] * v.z + mat4[2][3] * v.w,
                         v.w);
    }
    glm::mat4 MathUtil::AffineInverse(const glm::mat4& mat4)
    {
        return glm::affineInverse(mat4);
    }

    glm::quat MathUtil::InverseQuaternion(const glm::quat& qRot)
    {
        return glm::inverse(qRot);
    }
    glm::mat3 MathUtil::InverseMatrix3(const glm::mat3& mat3)
    {
        return glm::inverse(mat3);
    }
    glm::mat4 MathUtil::InverseMatrix4(const glm::mat4& mat4)
    {
        return glm::inverse(mat4);
    }

    glm::mat3 MathUtil::TransposeMatrix3(const glm::mat3& mat3)
    {
        return glm::transpose(mat3);
    }
    glm::mat4 MathUtil::TransposeMatrix4(const glm::mat4& mat4)
    {
        return glm::transpose(mat4);
    }

}; //LostPeter