/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2022-10-30
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_CAMERA_H_
#define _VULKAN_CAMERA_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport VulkanCamera
	{
	public:
		VulkanCamera();
		~VulkanCamera();

    private:
		glm::vec3 m_vPos;
		glm::quat m_qRot;

		float m_fNearZ;
		float m_fFarZ;
		float m_fAspect;
		float m_fFovY;
		float m_fNearWindowHeight;
		float m_fFarWindowHeight;

		bool m_bIsViewDirty;

		glm::mat4 m_mat4View;
		glm::mat4 m_mat4Projection;

    public:
		const glm::vec3& GetPos() const  { return this->m_vPos; }
		void SetPos(float x, float y, float z) { SetPos(glm::vec3(x, y, z)); }
		void SetPos(const glm::vec3& v)
		{
			this->m_vPos = v;
			this->m_bIsViewDirty = true;
		}
		const glm::quat& GetOrientation() const	{ return m_qRot; }
		void SetOrientation(const glm::quat& qRot);

		glm::vec3 GetEulerAngles() const;
		void SetEulerAngles(const glm::vec3& vEulerAngles);

		glm::vec3 GetRight() const;
		glm::vec3 GetUp() const;
		glm::vec3 GetDir() const;
		void SetDir(const glm::vec3& vDir);

		float GetNearZ() const { return this->m_fNearZ; }
		void SetNearZ(float fNearZ) { this->m_fNearZ = fNearZ; }
		float GetFarZ() const { return this->m_fFarZ; }
		void SetFarZ(float fFarZ) { this->m_fFarZ = fFarZ; }
		float GetAspect() const { return this->m_fAspect; }
		void SetAspect(float fAspect) { this->m_fAspect = fAspect; }
		float GetFovY()const { return this->m_fFovY; }
		void SetFovY(float fFovY) { this->m_fFovY = fFovY; }
		float GetFovX() const
		{
			float halfWidth = 0.5f * GetNearWindowWidth();
			return 2.0f * atan(halfWidth / this->m_fNearZ);
		}

		float GetNearWindowWidth() const { return this->m_fAspect * this->m_fNearWindowHeight; }
		float GetNearWindowHeight() const { return this->m_fNearWindowHeight; }
		float GetFarWindowWidth() const { return this->m_fAspect * this->m_fFarWindowHeight; }
		float GetFarWindowHeight() const { return this->m_fFarWindowHeight; }

		glm::mat4 GetMatrix4World() const;
		const glm::mat4& GetMatrix4View() const { return this->m_mat4View; }
		const glm::mat4& GetMatrix4Projection() const { return this->m_mat4Projection; }

	public:
		void LookAtLH(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& vUp);
		void PerspectiveLH(float fFovY, float fAspect, float fNearZ, float fFarZ);

		void Strafe(float dis);
		void Walk(float dis);

		void Pitch(float angle); //Rotate with X Axis 
		void Yaw(float angle); //Rotate with Y Axis
		void Roll(float angle);	//Rotate with Z Axis
		
		void Rotate(const glm::vec3& axis, float angle);
		void Rotate(const glm::quat& qRot);
		
		void UpdateViewMatrix();
		void UpdateProjectionMatrix();
    };

}; //LostPeter

#endif