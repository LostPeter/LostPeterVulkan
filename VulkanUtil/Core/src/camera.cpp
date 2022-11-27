#include "../include/preinclude.h"
#include "../include/camera.h"
#include "../include/mathutil.h"

namespace LibUtil
{
    Camera::Camera()
		: m_vPos(0.0f, 0.0f, 0.0f)
		, m_vRight(1.0f, 0.0f, 0.0f)
		, m_vUp(0.0f, 1.0f, 0.0f)
		, m_vDir(0.0f, 0.0f, 1.0f)
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

	glm::mat4 Camera::GetMatrix4World() const
	{
		return MathUtil::Translate(this->m_vPos);
	}

	glm::quat Camera::GetOrientation() const
	{
		return MathUtil::ToQuaternion(this->m_vRight, this->m_vUp, this->m_vDir);
	}
	void Camera::SetOrientation(const glm::quat& quat)
	{
		this->m_vRight = quat * MathUtil::ms_v3UnitX;
		this->m_vUp = quat * MathUtil::ms_v3UnitY;
		this->m_vDir = quat * MathUtil::ms_v3UnitZ;

		m_bIsViewDirty = true;
	}

	glm::vec3 Camera::GetEulerAngles() const
	{
		glm::quat qRot = MathUtil::ToQuaternion(this->m_vRight, this->m_vUp, this->m_vDir);
		return MathUtil::ToEulerAngles(qRot);
	}
	void Camera::SetEulerAngles(const glm::vec3& vEulerAngles)
	{
		glm::quat qRot = MathUtil::ToQuaternion(vEulerAngles);
		SetOrientation(qRot);
	}

	void Camera::LookAtLH(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& worldUp)
	{
		glm::mat4 mat = glm::lookAtLH(pos, target, worldUp);
		this->m_vPos = pos;
		MathUtil::FromMatrix4(mat, this->m_vRight, this->m_vUp, this->m_vDir);

		this->m_bIsViewDirty = true;
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
		this->m_vPos += dis * this->m_vRight;

		this->m_bIsViewDirty = true;
	}

	void Camera::Walk(float dis)
	{
		this->m_vPos += dis * this->m_vDir;

		this->m_bIsViewDirty = true;
	}

	void Camera::Pitch(float angle)
	{
		glm::quat qRot = MathUtil::ToQuaternionFromAngleAxis(angle, m_vRight);
		this->m_vUp = MathUtil::Normalize(qRot * this->m_vUp);
		this->m_vDir = MathUtil::Normalize(qRot * this->m_vDir);

		this->m_bIsViewDirty = true;
	}

	void Camera::RotateY(float angle)
	{
		glm::quat qRot = MathUtil::ToQuaternionFromAngleAxis(angle, m_vUp);
		this->m_vRight = MathUtil::Normalize(qRot * this->m_vRight);
		this->m_vDir = MathUtil::Normalize(qRot * this->m_vDir);

		this->m_bIsViewDirty = true;
	}

	void Camera::UpdateViewMatrix()
	{
		if (this->m_bIsViewDirty)
		{
			this->m_vRight = MathUtil::Normalize(this->m_vRight);
			this->m_vDir = MathUtil::Normalize(this->m_vDir);
			this->m_vUp = MathUtil::Normalize(MathUtil::Cross(this->m_vDir, this->m_vRight));
			this->m_vRight = MathUtil::Cross(this->m_vUp, this->m_vDir);

			// View matrix is: Use Left-Hand Coordinate, HLSL (matrix4 * vector4)
			//  [ Px  Py  Pz   0  ]
			//  [ Qx  Qy  Qz   0  ]
			//  [ Rx  Ry  Rz   0  ]
			//  [ Tx  Ty  Tz   1  ]
			// T = -Matrix3(Rot) * Pos

			glm::mat3 mat3Rot = MathUtil::ToMatrix3(this->m_vRight, this->m_vUp, this->m_vDir);
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