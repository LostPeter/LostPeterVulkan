#include "../include/preinclude.h"
#include "../include/camera.h"
#include "../include/mathutil.h"

namespace LibUtil
{
    Camera::Camera()
		: m_vPos(MathUtil::ms_v3Zero)
		, m_qRot(MathUtil::ms_qUnit)

		, m_fNearZ(0.0f)
		, m_fFarZ(0.0f)
		, m_fAspect(0.0f)
		, m_fFovY(0.0f)
		, m_fNearWindowHeight(0.0f)
		, m_fFarWindowHeight(0.0f)
		, m_bIsViewDirty(true)
		, m_mat4View(MathUtil::Identity4x4())
		, m_mat4Projection(MathUtil::Identity4x4())
	{
		PerspectiveLH(0.25f * MathUtil::ms_fPI, 1.0f, 1.0f, 1000.0f);
	}

	Camera::~Camera()
	{

	}	

	void Camera::SetOrientation(const glm::quat& qRot)
	{
		this->m_qRot = MathUtil::Normalize(qRot);
		this->m_bIsViewDirty = true;
	}

	glm::vec3 Camera::GetEulerAngles() const
	{
		return MathUtil::ToEulerAngles(m_qRot);
	}
	void Camera::SetEulerAngles(const glm::vec3& vEulerAngles)
	{
		SetOrientation(MathUtil::ToQuaternion(vEulerAngles));
	}

	glm::vec3 Camera::GetRight() const 
	{ 
		return m_qRot * MathUtil::ms_v3UnitX; 
	}
	glm::vec3 Camera::GetUp() const 
	{ 
		return m_qRot * MathUtil::ms_v3UnitY; 
	}
	glm::vec3 Camera::GetDir() const 
	{ 
		return m_qRot * MathUtil::ms_v3UnitZ; 
	}
	void Camera::SetDir(const glm::vec3& vDir)
	{
		if (vDir == MathUtil::ms_v3Zero) 
			return;

		glm::vec3 zAdjustVec = MathUtil::Normalize(vDir);
		glm::vec3 axes[3];
		UpdateViewMatrix();
		MathUtil::ToAxes(this->m_qRot, axes[0], axes[1], axes[2]);
		glm::quat rotQuat = MathUtil::ms_qUnit;
		if (MathUtil::Length2(axes[2] + zAdjustVec) <  0.00005f) 
		{
			rotQuat = MathUtil::ToQuaternionFromRadianAxis(rotQuat, MathUtil::ms_fPI, axes[1]);
		}
		else
		{
			rotQuat = MathUtil::ToQuaternionFromSrc2Dst(axes[2], zAdjustVec);
		}
		SetOrientation(rotQuat * this->m_qRot);
	}

	glm::mat4 Camera::GetMatrix4World() const
	{
		return MathUtil::Translate(this->m_vPos);
	}

	void Camera::LookAtLH(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& worldUp)
	{
		this->m_vPos = pos;
		glm::mat4 mat = glm::lookAtLH(pos, target, worldUp);
		glm::quat qRot = MathUtil::ToQuaternion(MathUtil::TransposeMatrix3(mat));

		SetOrientation(qRot);
	}

	void Camera::PerspectiveLH(float fFovY, float fAspect, float fNearZ, float fFarZ)
	{
		this->m_fFovY = fFovY;
		this->m_fAspect = fAspect;
		this->m_fNearZ = fNearZ;
		this->m_fFarZ = fFarZ;

		float rad = glm::radians(this->m_fFovY);
		this->m_fNearWindowHeight = 2.0f * this->m_fNearZ * tanf(0.5f * rad);
		this->m_fFarWindowHeight = 2.0f * this->m_fFarZ * tanf(0.5f * rad);

		this->m_mat4Projection = glm::perspectiveLH(rad, this->m_fAspect, this->m_fNearZ, this->m_fFarZ);
	}

	void Camera::Strafe(float dis)
	{
		glm::vec3 vPos = this->m_vPos + dis * GetRight();
		SetPos(vPos);
	}

	void Camera::Walk(float dis)
	{
		glm::vec3 vPos = this->m_vPos + dis * GetDir();
		SetPos(vPos);
	}

	void Camera::Pitch(float angle)
	{
		glm::vec3 xAxis = m_qRot * MathUtil::ms_v3UnitX;
		Rotate(xAxis, angle);
	}
	void Camera::Yaw(float angle)
	{
		glm::vec3 yAxis = m_qRot * MathUtil::ms_v3UnitY;
		Rotate(yAxis, angle);
	}
	void Camera::Roll(float angle)
	{
		glm::vec3 zAxis = m_qRot * MathUtil::ms_v3UnitZ;
		Rotate(zAxis, angle);
	}

	void Camera::Rotate(const glm::vec3& axis, float angle)
	{
		glm::quat qRot = MathUtil::ToQuaternionFromAngleAxis(angle, axis);
		Rotate(qRot);
	}
	void Camera::Rotate(const glm::quat& qRot)
	{
		glm::quat qNorm = MathUtil::Normalize(qRot);
		SetOrientation(qNorm * m_qRot);
	}

	void Camera::UpdateViewMatrix()
	{
		if (this->m_bIsViewDirty)
		{
			// View matrix is: Use Left-Hand Coordinate, HLSL (matrix4 * vector4)
			//  [ Px  Py  Pz   0  ]
			//  [ Qx  Qy  Qz   0  ]
			//  [ Rx  Ry  Rz   0  ]
			//  [ Tx  Ty  Tz   1  ]
			// T = -Transpose(Matrix3(Rot)) * Pos

			glm::mat3 mat3Rot = MathUtil::TransposeMatrix3(MathUtil::ToMatrix3(m_qRot));
        	glm::vec3 trans = -mat3Rot * this->m_vPos;

			this->m_mat4View = glm::mat4(mat3Rot);
			this->m_mat4View[3][0] = trans.x;
			this->m_mat4View[3][1] = trans.y;
			this->m_mat4View[3][2] = trans.z;

			this->m_bIsViewDirty = false;
		}
	}

	void Camera::UpdateProjectionMatrix()
	{
		PerspectiveLH(this->m_fFovY, this->m_fAspect, this->m_fNearZ, this->m_fFarZ);
	}

}; //LibUtil