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

#ifndef _F_CAMERA_H_
#define _F_CAMERA_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class foundationExport FCamera
	{
	public:
		FCamera();
		virtual ~FCamera();

    protected:
		FCameraType m_typeCamera;

		FVector3 m_vPos;
		FQuaternion m_qRot;
		float m_fNearZ;
		float m_fFarZ;
		float m_fAspect;
		float m_fFovY;
		float m_fNearWindowHeight;
		float m_fFarWindowHeight;

		bool m_bIsViewDirty;

		FMatrix4 m_mat4View;
		FMatrix4 m_mat4Projection;

    public:
		F_FORCEINLINE FCameraType GetCameraType() const  { return this->m_typeCamera; }
		F_FORCEINLINE bool IsPerspective() const { return this->m_typeCamera == F_Camera_Perspective; }
		F_FORCEINLINE bool IsOrthogonal() const { return this->m_typeCamera == F_Camera_Orthogonal; }

		F_FORCEINLINE const FVector3& GetPos() const  { return this->m_vPos; }
		F_FORCEINLINE void SetPos(float x, float y, float z) { SetPos(FVector3(x, y, z)); }
		F_FORCEINLINE void SetPos(const FVector3& v)
		{
			this->m_vPos = v;
			this->m_bIsViewDirty = true;
		}
		F_FORCEINLINE const FQuaternion& GetOrientation() const	{ return m_qRot; }
		F_FORCEINLINE void SetOrientation(const FQuaternion& qRot);

		FVector3 GetEulerAngles() const;
		void SetEulerAngles(const FVector3& vEulerAngles);

		FVector3 GetRight() const;
		FVector3 GetUp() const;
		FVector3 GetDir() const;
		void SetDir(const FVector3& vDir);

		F_FORCEINLINE float GetNearZ() const { return this->m_fNearZ; }
		F_FORCEINLINE void SetNearZ(float fNearZ) { this->m_fNearZ = fNearZ; }
		F_FORCEINLINE float GetFarZ() const { return this->m_fFarZ; }
		F_FORCEINLINE void SetFarZ(float fFarZ) { this->m_fFarZ = fFarZ; }
		F_FORCEINLINE float GetAspect() const { return this->m_fAspect; }
		F_FORCEINLINE void SetAspect(float fAspect) { this->m_fAspect = fAspect; }
		F_FORCEINLINE float GetFovY()const { return this->m_fFovY; }
		F_FORCEINLINE void SetFovY(float fFovY) { this->m_fFovY = fFovY; }
		F_FORCEINLINE float GetFovX() const
		{
			float halfWidth = 0.5f * GetNearWindowWidth();
			return 2.0f * atan(halfWidth / this->m_fNearZ);
		}

		F_FORCEINLINE float GetNearWindowWidth() const { return this->m_fAspect * this->m_fNearWindowHeight; }
		F_FORCEINLINE float GetNearWindowHeight() const { return this->m_fNearWindowHeight; }
		F_FORCEINLINE float GetFarWindowWidth() const { return this->m_fAspect * this->m_fFarWindowHeight; }
		F_FORCEINLINE float GetFarWindowHeight() const { return this->m_fFarWindowHeight; }

		FMatrix4 GetMatrix4World() const;
		F_FORCEINLINE const FMatrix4& GetMatrix4View() const { return this->m_mat4View; }
		F_FORCEINLINE const FMatrix4& GetMatrix4Projection() const { return this->m_mat4Projection; }
		F_FORCEINLINE FMatrix4 GetMatrix4ViewProjection() const;
		F_FORCEINLINE FMatrix4 GetMatrix4ViewProjectionInverse() const;

		bool ConvertScreenPosToWorldPos3(const FVector3& vPosScreen, const FMatrix4& mat4Clip2World, const FVector4& vViewport, FVector3& vPosWorld) const;
		bool ConvertScreenPosToWorldPos3(const FVector3& vPosScreen, const FVector4& vViewport, FVector3& vPosWorld) const;

		bool ConvertScreenPos2ToWorldRay(const FVector2& vPosSceen2, const FVector4& vViewport, FRay* pOutRay) const;
		bool ConvertScreenPos2ToWorldRay(float screenX, float screenY, const FVector4& vViewport, FRay* pOutRay) const;
		

	public:
		virtual void LookAtLH(const FVector3& pos, const FVector3& target, const FVector3& vUp);
		virtual void PerspectiveLH(float fFovY, float fAspect, float fNearZ, float fFarZ);

		virtual void Strafe(float dis);
		virtual void Walk(float dis);

		virtual void Pitch(float angle); //Rotate with X Axis 
		virtual void Yaw(float angle); //Rotate with Y Axis
		virtual void Roll(float angle);	//Rotate with Z Axis
		
		virtual void Rotate(const FVector3& axis, float angle);
		virtual void Rotate(const FQuaternion& qRot);
		
		virtual void UpdateViewMatrix();
		virtual void UpdateProjectionMatrix();
    };

}; //LostPeterFoundation

#endif