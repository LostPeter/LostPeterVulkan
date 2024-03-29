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

#include "../include/FCamera.h"
#include "../include/FMath.h"
#include "../include/FRay.h"

namespace LostPeterFoundation
{
    FCamera::FCamera()
		: m_typeCamera(F_Camera_Perspective)
		, m_vPos(FMath::ms_v3Zero)
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

	FCamera::~FCamera()
	{

	}	

	void FCamera::SetOrientation(const FQuaternion& qRot)
	{
		this->m_qRot = FMath::Normalize(qRot);
		this->m_bIsViewDirty = true;
	}

	FVector3 FCamera::GetEulerAngles() const
	{
		return FMath::ToEulerAngles(m_qRot);
	}
	void FCamera::SetEulerAngles(const FVector3& vEulerAngles)
	{
		SetOrientation(FMath::ToQuaternion(vEulerAngles));
	}

	FVector3 FCamera::GetRight() const 
	{ 
		return m_qRot * FMath::ms_v3UnitX; 
	}
	FVector3 FCamera::GetUp() const 
	{ 
		return m_qRot * FMath::ms_v3UnitY; 
	}
	FVector3 FCamera::GetDir() const 
	{ 
		return m_qRot * FMath::ms_v3UnitZ; 
	}
	void FCamera::SetDir(const FVector3& vDir)
	{
		if (vDir == FMath::ms_v3Zero) 
			return;

		FVector3 zAdjustVec = FMath::Normalize(vDir);
		FVector3 axes[3];
		UpdateViewMatrix();
		FMath::ToAxes(this->m_qRot, axes[0], axes[1], axes[2]);
		FQuaternion rotQuat = FMath::ms_qUnit;
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

	FMatrix4 FCamera::GetMatrix4World() const
	{
		return FMath::Translate(this->m_vPos);
	}

	FMatrix4 FCamera::GetMatrix4ViewProjection() const
	{
		return GetMatrix4Projection() * GetMatrix4View();
	}

	FMatrix4 FCamera::GetMatrix4ViewProjectionInverse() const
	{
		return FMath::InverseMatrix4(GetMatrix4ViewProjection());
	}
	
	bool FCamera::ConvertScreenPosToWorldPos3(const FVector3& vPosScreen, const FMatrix4& mat4Clip2World, const FVector4& vViewport, FVector3& vPosWorld) const
	{
		//1> Pixels in [-1,1]
		FVector3 vIn;
		vIn.x = (vPosScreen.x - vViewport.x) * 2.0f / vViewport.z - 1.0f;
		vIn.y = 1.0f - (vPosScreen.y - vViewport.y) * 2.0f / vViewport.w;
		vIn.z = 0.95f;

		//2> Point on Plane
		FVector3 vOnPlane;
		if (FMath::TransformPerspective(mat4Clip2World, vIn, vOnPlane))
		{
			const FVector3& vCameraPos = GetPos();
        	FVector3 vDir = vOnPlane - vCameraPos;
			FVector3 vForward = GetDir();
			float fDistToPlane = FMath::Dot(vDir, vForward);
			if (FMath::Abs(fDistToPlane) >= 1.0e-6f)
			{
				if (IsPerspective())
				{
					vDir *= vPosScreen.z / fDistToPlane;
					vPosWorld = vCameraPos + vDir;
				}
				else
				{
					vPosWorld = vOnPlane - vForward * (fDistToPlane - vPosScreen.z);
				}
				return true;
			}
		}

		vPosWorld = FMath::ms_v3Zero;
    	return false;
	}

	bool FCamera::ConvertScreenPosToWorldPos3(const FVector3& vPosScreen, const FVector4& vViewport, FVector3& vPosWorld) const
	{
		return ConvertScreenPosToWorldPos3(vPosScreen, GetMatrix4ViewProjectionInverse(), vViewport, vPosWorld);
	}

	bool FCamera::ConvertScreenPos2ToWorldRay(const FVector2& vPosSceen2, const FVector4& vViewport, FRay* pOutRay) const
	{
		return ConvertScreenPos2ToWorldRay(vPosSceen2.x, vPosSceen2.y, vViewport, pOutRay);
	}

	bool FCamera::ConvertScreenPos2ToWorldRay(float screenX, float screenY, const FVector4& vViewport, FRay* pOutRay) const
	{
		float fNear = GetNearZ();
		FVector3 vScreenNear(screenX, screenY, fNear);
		FVector3 vScreenFar(screenX, screenY, fNear + 1000.0f);
		FMatrix4 mat4Clip2World = GetMatrix4ViewProjectionInverse();

		//RayOrigin
		FVector3 vRayOrigin;
		if (!ConvertScreenPosToWorldPos3(vScreenNear, mat4Clip2World, vViewport, vRayOrigin))
		{
			pOutRay->SetOrigin(GetPos());
			pOutRay->SetDirection(FMath::ms_v3UnitZ);
			return false;
		}
		
		//RayTarget
		FVector3 vRayTarget;
		if (!ConvertScreenPosToWorldPos3(vScreenFar, mat4Clip2World, vViewport, vRayTarget))
		{
			pOutRay->SetOrigin(GetPos());
			pOutRay->SetDirection(FMath::ms_v3UnitZ);
			return false;
		}

		pOutRay->SetOrigin(vRayOrigin);
		pOutRay->SetDirection(FMath::Normalize(vRayTarget - vRayOrigin));
		return true;
	}

	void FCamera::LookAtLH(const FVector3& pos, const FVector3& target, const FVector3& worldUp)
	{
		this->m_vPos = pos;
		FMatrix4 mat = glm::lookAtLH(pos, target, worldUp);
		FQuaternion qRot = FMath::ToQuaternion(FMath::TransposeMatrix3(mat));

		SetOrientation(qRot);
	}

	void FCamera::PerspectiveLH(float fFovY, float fAspect, float fNearZ, float fFarZ)
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

	void FCamera::Strafe(float dis)
	{
		FVector3 vPos = this->m_vPos + dis * GetRight();
		SetPos(vPos);
	}

	void FCamera::Walk(float dis)
	{
		FVector3 vPos = this->m_vPos + dis * GetDir();
		SetPos(vPos);
	}

	void FCamera::Pitch(float angle)
	{
		FVector3 xAxis = m_qRot * FMath::ms_v3UnitX;
		Rotate(xAxis, angle);
	}
	void FCamera::Yaw(float angle)
	{
		FVector3 yAxis = m_qRot * FMath::ms_v3UnitY;
		Rotate(yAxis, angle);
	}
	void FCamera::Roll(float angle)
	{
		FVector3 zAxis = m_qRot * FMath::ms_v3UnitZ;
		Rotate(zAxis, angle);
	}

	void FCamera::Rotate(const FVector3& axis, float angle)
	{
		FQuaternion qRot = FMath::ToQuaternionFromAngleAxis(angle, axis);
		Rotate(qRot);
	}
	void FCamera::Rotate(const FQuaternion& qRot)
	{
		FQuaternion qNorm = FMath::Normalize(qRot);
		SetOrientation(qNorm * m_qRot);
	}

	void FCamera::UpdateViewMatrix()
	{
		if (this->m_bIsViewDirty)
		{
			// View matrix is: Use Left-Hand Coordinate, HLSL (matrix4 * vector4)
			//  [ Px  Py  Pz   0  ]
			//  [ Qx  Qy  Qz   0  ]
			//  [ Rx  Ry  Rz   0  ]
			//  [ Tx  Ty  Tz   1  ]
			// T = -Transpose(Matrix3(Rot)) * Pos

			FMatrix3 mat3Rot = FMath::TransposeMatrix3(FMath::ToMatrix3(m_qRot));
        	FVector3 trans = -mat3Rot * this->m_vPos;

			this->m_mat4View = FMatrix4(mat3Rot);
			this->m_mat4View[3][0] = trans.x;
			this->m_mat4View[3][1] = trans.y;
			this->m_mat4View[3][2] = trans.z;

			this->m_bIsViewDirty = false;
		}
	}

	void FCamera::UpdateProjectionMatrix()
	{
		PerspectiveLH(this->m_fFovY, this->m_fAspect, this->m_fNearZ, this->m_fFarZ);
	}
    
}; //LostPeterFoundation