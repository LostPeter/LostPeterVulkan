/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-10-30
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/PreInclude.h"
#include "../include/VulkanCamera.h"
#include "../include/FMath.h"

namespace LostPeter
{
    VulkanCamera::VulkanCamera()
		: m_vPos(FMath::ms_v3Zero)
		, m_qRot(FMath::ms_qUnit)

		, m_fNearZ(0.0f)
		, m_fFarZ(0.0f)
		, m_fAspect(0.0f)
		, m_fFovY(0.0f)
		, m_fNearWindowHeight(0.0f)
		, m_fFarWindowHeight(0.0f)
		, m_bIsViewDirty(true)
		, m_mat4View(FMath::Identity4x4())
		, m_mat4Projection(FMath::Identity4x4())
	{
		PerspectiveLH(0.25f * FMath::ms_fPI, 1.0f, 1.0f, 1000.0f);
	}

	VulkanCamera::~VulkanCamera()
	{

	}	

	void VulkanCamera::SetOrientation(const glm::quat& qRot)
	{
		this->m_qRot = FMath::Normalize(qRot);
		this->m_bIsViewDirty = true;
	}

	glm::vec3 VulkanCamera::GetEulerAngles() const
	{
		return FMath::ToEulerAngles(m_qRot);
	}
	void VulkanCamera::SetEulerAngles(const glm::vec3& vEulerAngles)
	{
		SetOrientation(FMath::ToQuaternion(vEulerAngles));
	}

	glm::vec3 VulkanCamera::GetRight() const 
	{ 
		return m_qRot * FMath::ms_v3UnitX; 
	}
	glm::vec3 VulkanCamera::GetUp() const 
	{ 
		return m_qRot * FMath::ms_v3UnitY; 
	}
	glm::vec3 VulkanCamera::GetDir() const 
	{ 
		return m_qRot * FMath::ms_v3UnitZ; 
	}
	void VulkanCamera::SetDir(const glm::vec3& vDir)
	{
		if (vDir == FMath::ms_v3Zero) 
			return;

		glm::vec3 zAdjustVec = FMath::Normalize(vDir);
		glm::vec3 axes[3];
		UpdateViewMatrix();
		FMath::ToAxes(this->m_qRot, axes[0], axes[1], axes[2]);
		glm::quat rotQuat = FMath::ms_qUnit;
		if (FMath::Length2(axes[2] + zAdjustVec) <  0.00005f) 
		{
			rotQuat = FMath::ToQuaternionFromRadianAxis(rotQuat, FMath::ms_fPI, axes[1]);
		}
		else
		{
			rotQuat = FMath::ToQuaternionFromSrc2Dst(axes[2], zAdjustVec);
		}
		SetOrientation(rotQuat * this->m_qRot);
	}

	glm::mat4 VulkanCamera::GetMatrix4World() const
	{
		return FMath::Translate(this->m_vPos);
	}

	void VulkanCamera::LookAtLH(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& worldUp)
	{
		this->m_vPos = pos;
		glm::mat4 mat = glm::lookAtLH(pos, target, worldUp);
		glm::quat qRot = FMath::ToQuaternion(FMath::TransposeMatrix3(mat));

		SetOrientation(qRot);
	}

	void VulkanCamera::PerspectiveLH(float fFovY, float fAspect, float fNearZ, float fFarZ)
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

	void VulkanCamera::Strafe(float dis)
	{
		glm::vec3 vPos = this->m_vPos + dis * GetRight();
		SetPos(vPos);
	}

	void VulkanCamera::Walk(float dis)
	{
		glm::vec3 vPos = this->m_vPos + dis * GetDir();
		SetPos(vPos);
	}

	void VulkanCamera::Pitch(float angle)
	{
		glm::vec3 xAxis = m_qRot * FMath::ms_v3UnitX;
		Rotate(xAxis, angle);
	}
	void VulkanCamera::Yaw(float angle)
	{
		glm::vec3 yAxis = m_qRot * FMath::ms_v3UnitY;
		Rotate(yAxis, angle);
	}
	void VulkanCamera::Roll(float angle)
	{
		glm::vec3 zAxis = m_qRot * FMath::ms_v3UnitZ;
		Rotate(zAxis, angle);
	}

	void VulkanCamera::Rotate(const glm::vec3& axis, float angle)
	{
		glm::quat qRot = FMath::ToQuaternionFromAngleAxis(angle, axis);
		Rotate(qRot);
	}
	void VulkanCamera::Rotate(const glm::quat& qRot)
	{
		glm::quat qNorm = FMath::Normalize(qRot);
		SetOrientation(qNorm * m_qRot);
	}

	void VulkanCamera::UpdateViewMatrix()
	{
		if (this->m_bIsViewDirty)
		{
			// View matrix is: Use Left-Hand Coordinate, HLSL (matrix4 * vector4)
			//  [ Px  Py  Pz   0  ]
			//  [ Qx  Qy  Qz   0  ]
			//  [ Rx  Ry  Rz   0  ]
			//  [ Tx  Ty  Tz   1  ]
			// T = -Transpose(Matrix3(Rot)) * Pos

			glm::mat3 mat3Rot = FMath::TransposeMatrix3(FMath::ToMatrix3(m_qRot));
        	glm::vec3 trans = -mat3Rot * this->m_vPos;

			this->m_mat4View = glm::mat4(mat3Rot);
			this->m_mat4View[3][0] = trans.x;
			this->m_mat4View[3][1] = trans.y;
			this->m_mat4View[3][2] = trans.z;

			this->m_bIsViewDirty = false;
		}
	}

	void VulkanCamera::UpdateProjectionMatrix()
	{
		PerspectiveLH(this->m_fFovY, this->m_fAspect, this->m_fNearZ, this->m_fFarZ);
	}

}; //LostPeter