/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-28
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/ShaderParamSourceAuto.h"
#include "../include/RenderEngine.h"
#include "../include/Renderer.h"
#include "../include/Renderable.h"
#include "../include/RenderTarget.h"
#include "../include/ObjectCamera.h"
#include "../include/ObjectVisibleBoundsInfo.h"
#include "../include/ObjectLight.h"
#include "../include/Viewport.h"
#include "../include/MaterialInstance.h"
#include "../include/RenderState.h"

namespace LostPeterEngine
{
    ShaderParamSourceAuto::ShaderParamSourceAuto(const String& strNameShaderParamSource)
        : Base(strNameShaderParamSource)
   ////World
		, m_nWorldMatrixCount(0)
		, m_pWorldMatrixArray(nullptr)
		, m_pWorldFloat(nullptr)
		, m_bWorldMatrixDirty(true)
		, m_bWorldMatrixTransposeDirty(true)
		, m_bWorldMatrixInverseDirty(true)
		, m_bWorldMatrixInverseTransposeDirty(true)
	////View	
		, m_bViewMatrixDirty(true)
		, m_bViewMatrixTransposeDirty(true)
		, m_bViewMatrixInverseDirty(true)
		, m_bViewMatrixInverseTransposeDirty(true)
	////Projection
		, m_bProjectionMatrixDirty(true)
		, m_bProjectionMatrixTransposeDirty(true)
		, m_bProjectionMatrixInverseDirty(true)
		, m_bProjectionMatrixInverseTransposeDirty(true)
	////World * View	
		, m_bWorldViewMatrixDirty(true)
		, m_bWorldViewMatrixTransposeDirty(true)
		, m_bWorldViewMatrixInverseDirty(true)
		, m_bWorldViewMatrixInverseTransposeDirty(true)
	////View * Projection
		, m_bViewProjectionMatrixDirty(true)
		, m_bViewProjectionMatrixTransposeDirty(true)
		, m_bViewProjectionMatrixInverseDirty(true)
		, m_bViewProjectionMatrixInverseTransposeDirty(true)
	////World * View * Projection
		, m_bWorldViewProjectionMatrixDirty(true)
		, m_bWorldViewProjectionMatrixTransposeDirty(true)
		, m_bWorldViewProjectionMatrixInverseDirty(true)
		, m_bWorldViewProjectionMatrixInverseTransposeDirty(true)
	////Renderable,RenderTarget,ObjectCamera,Viewport,SceneManager
		, m_pCurRenderable(nullptr)
		, m_pCurRenderTarget(nullptr)
		, m_pCurCamera(nullptr)
		, m_bCameraPosInObjectSpaceDirty(true)
		, m_bCameraRightInObjectSpaceDirty(true)
		, m_bCameraUpInObjectSpaceDirty(true)
		, m_bCameraDirInObjectSpaceDirty(true)
		, m_bCameraPosDirty(true)
		, m_bCameraRightDirty(true)
		, m_bCameraUpDirty(true)
		, m_bCameraDirDirty(true)
		, m_bCameraParamDirty(true)
		, m_pCurViewport(nullptr)
		, m_bViewportSizeDirty(true)
		, m_pCurSceneManager(nullptr)
		, m_pMainCameraBoundsInfo(nullptr)
		, m_v4SceneDepthRange(FMath::ms_v4Zero)
		, m_bSceneDepthRangeDirty(true)
	////Light
		, m_pCurLightArray(nullptr)
		, m_bShadowCameraDepthRangesDirty(true)
	////Material
		, m_clAmbient(FMath::ms_clWhite)
		, m_clDiffuse(FMath::ms_clWhite)
		, m_clSpecular(FMath::ms_clBlack)
		, m_clEmissive(FMath::ms_clBlack)
		, m_fShininess(1.0f)
	////Fog

	////Texture

	////Time
	{
		for (int i = 0; i < C_CONFIG_MAX_LIGHT_COUNT; i++)
			m_aGlobalAmbient[i] = FMath::ms_clBlack;
		//m_pControllerManager = ControllerManager::GetSingletonPtr();
		//F_Assert(m_pControllerManager && "ShaderParamSourceAuto::ShaderParamSourceAuto")
	}

	ShaderParamSourceAuto::~ShaderParamSourceAuto()
	{

	}
	
	void ShaderParamSourceAuto::SetWorldFloat(float* pFloat, uint32 nCount)
	{
		m_pWorldFloat = pFloat;
		m_nWorldMatrixCount = nCount;
		m_pWorldMatrixArray = nullptr;

		m_bWorldMatrixDirty	= true;
		m_bWorldMatrixTransposeDirty = true;
		m_bWorldMatrixInverseDirty = true;
		m_bWorldMatrixInverseTransposeDirty = true;

		m_bWorldViewMatrixDirty	= true;
		m_bWorldViewMatrixTransposeDirty = true;
		m_bWorldViewMatrixInverseDirty = true;
		m_bWorldViewMatrixInverseTransposeDirty = true;
			
		m_bWorldViewProjectionMatrixDirty = true;
		m_bWorldViewProjectionMatrixTransposeDirty = true;
		m_bWorldViewProjectionMatrixInverseDirty = true;
		m_bWorldViewProjectionMatrixInverseTransposeDirty = true;
	}

	const FMatrix4* ShaderParamSourceAuto::GetWorldMatrixArray() const
	{
		if (m_bWorldMatrixDirty)
		{
			m_pWorldMatrixArray = m_aWorldMatrix;
			m_pCurRenderable->GetWorldTransforms(m_aWorldMatrix, m_nWorldMatrixCount);
			m_bWorldMatrixDirty = false;
		}
		return m_pWorldMatrixArray;
	}

	void ShaderParamSourceAuto::SetWorldMatrixArray(const FMatrix4* pMatrix, uint32 nCount)
	{
		m_pWorldMatrixArray = pMatrix;
		m_nWorldMatrixCount = nCount;
		m_bWorldMatrixDirty = false;
	}

	uint32 ShaderParamSourceAuto::GetWorldMatrixCount() const
	{
		if (m_bWorldMatrixDirty)
		{
			m_pWorldMatrixArray = m_aWorldMatrix;
			m_pCurRenderable->GetWorldTransforms(m_aWorldMatrix, m_nWorldMatrixCount);
			m_bWorldMatrixDirty = false;
		}
		return m_nWorldMatrixCount;
	}

	const FMatrix4& ShaderParamSourceAuto::GetWorldMatrix() const
	{
		if (m_bWorldMatrixDirty)
		{
			m_pWorldMatrixArray = m_aWorldMatrix;
			m_pCurRenderable->GetWorldTransforms(m_aWorldMatrix, m_nWorldMatrixCount);
			m_bWorldMatrixDirty = false;
		}
		return m_pWorldMatrixArray[0];
	}

	void ShaderParamSourceAuto::SetWorldMatrix(const FMatrix4& matWorld)
	{
		m_pWorldMatrixArray = m_aWorldMatrix;
		m_aWorldMatrix[0] = matWorld;
		m_nWorldMatrixCount = 1;
		m_bWorldMatrixDirty = false;
	}

	const FMatrix4& ShaderParamSourceAuto::GetWorldMatrix_Transpose() const
	{
		if (m_bWorldMatrixTransposeDirty)
		{
			m_WorldMatrix_Transpose = FMath::TransposeMatrix4(GetWorldMatrix());
			m_bWorldMatrixTransposeDirty = false;
		}
		return m_WorldMatrix_Transpose;
	}

	const FMatrix4& ShaderParamSourceAuto::GetWorldMatrix_Inverse() const
	{
		if (m_bWorldMatrixInverseDirty)
		{
			m_WorldMatrix_Inverse = FMath::InverseMatrix4(GetWorldMatrix());
			m_bWorldMatrixInverseDirty = false;
		}
		return m_WorldMatrix_Inverse;
	}

	const FMatrix4& ShaderParamSourceAuto::GetWorldMatrix_InverseTranspose() const
	{
		if (m_bWorldMatrixInverseTransposeDirty)
		{
			m_WorldMatrix_InverseTranspose = FMath::InverseMatrix4(FMath::TransposeMatrix4(GetWorldMatrix()));
			m_bWorldMatrixInverseTransposeDirty = false;
		}
		return m_WorldMatrix_InverseTranspose;
	}

	const FMatrix4& ShaderParamSourceAuto::GetViewMatrix() const
	{
		if (m_bViewMatrixDirty)
		{
			if (m_pCurRenderable && m_pCurRenderable->GetIsUseIdentityView())
				m_ViewMatrix = FMath::ms_mat4Unit;
			else
				m_ViewMatrix = m_pCurCamera->GetViewMatrix(true);
			m_bViewMatrixDirty = false;
		}
		return m_ViewMatrix;
	}

	const FMatrix4& ShaderParamSourceAuto::GetViewMatrix_Transpose() const
	{
		if (m_bViewMatrixTransposeDirty)
		{
			m_ViewMatrix_Transpose = FMath::TransposeMatrix4(GetViewMatrix());
			m_bViewMatrixTransposeDirty = false;
		}
		return m_ViewMatrix_Transpose;
	}

	const FMatrix4& ShaderParamSourceAuto::GetViewMatrix_Inverse() const
	{
		if (m_bViewMatrixInverseDirty)
		{
			m_ViewMatrix_Inverse = FMath::InverseMatrix4(GetViewMatrix());
			m_bViewMatrixInverseDirty = false;
		}
		return m_ViewMatrix_Inverse;
	}

	const FMatrix4& ShaderParamSourceAuto::GetViewMatrix_InverseTranspose() const
	{
		if (m_bViewMatrixInverseTransposeDirty)
		{
			m_ViewMatrix_InverseTranspose = FMath::InverseMatrix4(FMath::TransposeMatrix4(GetViewMatrix()));
			m_bViewMatrixInverseTransposeDirty = false;
		}
		return m_ViewMatrix_InverseTranspose;
	}

	const FMatrix4& ShaderParamSourceAuto::GetProjectionMatrix() const
	{
		if (m_bProjectionMatrixDirty)
		{
			if (m_pCurRenderable && m_pCurRenderable->GetIsUseIdentityProjection())
			{
				Renderer* pRenderer = RenderEngine::GetSingleton().GetRendererCurrent();
				pRenderer->ConvertProjectionMatrix(FMath::ms_mat4Unit, m_ProjectionMatrix, true);
			}
			else
			{
				m_ProjectionMatrix = m_pCurCamera->GetProjectionMatrixWithRSDepth();
			}
			if (m_pCurRenderTarget && m_pCurRenderTarget->RequiresTextureFlipping())
			{
				m_ProjectionMatrix[1][0] = -m_ProjectionMatrix[1][0];
				m_ProjectionMatrix[1][1] = -m_ProjectionMatrix[1][1];
				m_ProjectionMatrix[1][2] = -m_ProjectionMatrix[1][2];
				m_ProjectionMatrix[1][3] = -m_ProjectionMatrix[1][3];
			}
			m_bProjectionMatrixDirty = false;
		}
		return m_ProjectionMatrix;
	}

	const FMatrix4& ShaderParamSourceAuto::GetProjectionMatrix_Transpose() const
	{
		if (m_bProjectionMatrixTransposeDirty)
		{
			m_ProjectionMatrix_Transpose = FMath::TransposeMatrix4(GetProjectionMatrix());
			m_bProjectionMatrixTransposeDirty = false;
		}
		return m_ProjectionMatrix_Transpose;
	}

	const FMatrix4& ShaderParamSourceAuto::GetProjectionMatrix_Inverse() const
	{
		if (m_bProjectionMatrixInverseDirty)
		{
			m_ProjectionMatrix_Inverse = FMath::InverseMatrix4(GetProjectionMatrix());
			m_bProjectionMatrixInverseDirty = false;
		}
		return m_ProjectionMatrix_Inverse;
	}

	const FMatrix4& ShaderParamSourceAuto::GetProjectionMatrix_InverseTranspose() const
	{
		if (m_bProjectionMatrixInverseTransposeDirty)
		{
			m_ProjectionMatrix_InverseTranspose = FMath::InverseMatrix4(FMath::TransposeMatrix4(GetProjectionMatrix()));
			m_bProjectionMatrixInverseTransposeDirty = false;
		}
		return m_ProjectionMatrix_InverseTranspose;
	}

	const FMatrix4& ShaderParamSourceAuto::GetWorldViewMatrix() const
	{
		if (m_bWorldViewMatrixDirty)
		{
			m_WorldViewMatrix = GetViewMatrix() * GetWorldMatrix();
			m_bWorldViewMatrixDirty = false;
		}
		return m_WorldViewMatrix;
	}

	const FMatrix4& ShaderParamSourceAuto::GetWorldViewMatrix_Transpose() const
	{	
		if (m_bWorldViewMatrixTransposeDirty)
		{
			m_WorldViewMatrix_Transpose = FMath::TransposeMatrix4(GetWorldViewMatrix());
			m_bWorldViewMatrixTransposeDirty = false;
		}
		return m_WorldViewMatrix_Transpose;
	}

	const FMatrix4& ShaderParamSourceAuto::GetWorldViewMatrix_Inverse() const
	{
		if (m_bWorldViewMatrixInverseDirty)
		{
			m_WorldViewMatrix_Inverse = FMath::InverseMatrix4(GetWorldViewMatrix());
			m_bWorldViewMatrixInverseDirty = false;
		}
		return m_ProjectionMatrix_Transpose;
	}

	const FMatrix4& ShaderParamSourceAuto::GetWorldViewMatrix_InverseTranspose() const
	{
		if (m_bWorldViewMatrixInverseTransposeDirty)
		{
			m_WorldViewMatrix_InverseTranspose = FMath::InverseMatrix4(FMath::TransposeMatrix4(GetWorldViewMatrix()));
			m_bWorldViewMatrixInverseTransposeDirty = false;
		}
		return m_WorldViewMatrix_InverseTranspose;
	}

	const FMatrix4& ShaderParamSourceAuto::GetViewProjectionMatrix() const
	{
		if (m_bViewProjectionMatrixDirty)
		{
			m_ViewProjectionMatrix = GetProjectionMatrix() * GetViewMatrix();
			m_bViewProjectionMatrixDirty = false;
		}
		return m_ViewProjectionMatrix;
	}

	const FMatrix4& ShaderParamSourceAuto::GetViewProjectionMatrix_Transpose() const
	{
		if (m_bViewProjectionMatrixTransposeDirty)
		{
			m_ViewProjectionMatrix_Transpose = FMath::TransposeMatrix4(GetViewProjectionMatrix());
			m_bViewProjectionMatrixTransposeDirty = false;
		}
		return m_ViewProjectionMatrix_Transpose;
	}

	const FMatrix4& ShaderParamSourceAuto::GetViewProjectionMatrix_Inverse() const
	{
		if (m_bViewProjectionMatrixInverseDirty)
		{
			m_ViewProjectionMatrix_Inverse = FMath::InverseMatrix4(GetViewProjectionMatrix());
			m_bViewProjectionMatrixInverseDirty = false;
		}
		return m_ViewProjectionMatrix_Inverse;
	}

	const FMatrix4& ShaderParamSourceAuto::GetViewProjectionMatrix_InverseTranspose() const
	{
		if (m_bViewProjectionMatrixInverseTransposeDirty)
		{
			m_ViewProjectionMatrix_InverseTranspose = FMath::InverseMatrix4(FMath::TransposeMatrix4(GetViewProjectionMatrix()));
			m_bViewProjectionMatrixInverseTransposeDirty = false;
		}
		return m_ViewProjectionMatrix_InverseTranspose;
	}

	const FMatrix4& ShaderParamSourceAuto::GetWorldViewProjectionMatrix() const
	{
		if (m_bWorldViewProjectionMatrixDirty)
		{
			m_WorldViewProjectionMatrix = GetProjectionMatrix() * GetWorldViewMatrix();
			m_bWorldViewProjectionMatrixDirty = false;
		}
		return m_WorldViewProjectionMatrix;
	}

	const FMatrix4& ShaderParamSourceAuto::GetWorldViewProjectionMatrix_Transpose() const
	{
		if (m_bWorldViewProjectionMatrixTransposeDirty)
		{
			m_WorldViewProjectionMatrix_Transpose = FMath::TransposeMatrix4(GetWorldViewProjectionMatrix());
			m_bWorldViewProjectionMatrixTransposeDirty = false;
		}
		return m_WorldViewProjectionMatrix_Transpose;
	}

	const FMatrix4& ShaderParamSourceAuto::GetWorldViewProjectionMatrix_Inverse() const
	{
		if (m_bWorldViewProjectionMatrixInverseDirty)
		{
			m_WorldViewProjectionMatrix_Inverse = FMath::InverseMatrix4(GetWorldViewProjectionMatrix());
			m_bWorldViewProjectionMatrixInverseDirty = false;
		}
		return m_WorldViewProjectionMatrix_Inverse;
	}

	const FMatrix4& ShaderParamSourceAuto::GetWorldViewProjectionMatrix_InverseTranspose() const
	{
		if (m_bWorldViewProjectionMatrixInverseTransposeDirty)
		{
			m_WorldViewProjectionMatrix_InverseTranspose = FMath::InverseMatrix4(FMath::TransposeMatrix4(GetWorldViewProjectionMatrix()));
			m_bWorldViewProjectionMatrixInverseTransposeDirty = false;
		}
		return m_WorldViewProjectionMatrix_InverseTranspose;
	}	

	void ShaderParamSourceAuto::SetCurRenderable(const Renderable* pRenderable)
	{
		m_pCurRenderable = pRenderable;
		bool bViewDirty	= m_pCurRenderable->GetIsUseIdentityView();
		bool bProjectionDirty = m_pCurRenderable->GetIsUseIdentityProjection();

	////World
		m_bWorldMatrixDirty	= true;
		m_bWorldMatrixTransposeDirty = true;
		m_bWorldMatrixInverseDirty = true;
		m_bWorldMatrixInverseTransposeDirty	= true;
	
		if (bViewDirty)
		{
		////View
			m_bViewMatrixDirty = true;
			m_bViewMatrixTransposeDirty	= true;
			m_bViewMatrixInverseDirty = true;
			m_bViewMatrixInverseTransposeDirty = true;
		////World * View	
			m_bWorldViewMatrixDirty	= true;
			m_bWorldViewMatrixTransposeDirty = true;
			m_bWorldViewMatrixInverseDirty = true;
			m_bWorldViewMatrixInverseTransposeDirty	= true;
		////View * Projection	
			m_bViewProjectionMatrixDirty = true;
			m_bViewProjectionMatrixTransposeDirty = true;
			m_bViewProjectionMatrixInverseDirty	= true;
			m_bViewProjectionMatrixInverseTransposeDirty = true;
		////World * View * Projection		
			m_bWorldViewProjectionMatrixDirty = true;
			m_bWorldViewProjectionMatrixTransposeDirty = true;
			m_bWorldViewProjectionMatrixInverseDirty = true;
			m_bWorldViewProjectionMatrixInverseTransposeDirty = true;
		}

		if (bProjectionDirty)
		{
		////Projection
			m_bProjectionMatrixDirty = true;
			m_bProjectionMatrixTransposeDirty = true;
			m_bProjectionMatrixInverseDirty	= true;
			m_bProjectionMatrixInverseTransposeDirty = true;
		////View * Projection
			m_bViewProjectionMatrixDirty = true;
			m_bViewProjectionMatrixTransposeDirty = true;
			m_bViewProjectionMatrixInverseDirty	= true;
			m_bViewProjectionMatrixInverseTransposeDirty = true;
		////World * View * Projection		
			m_bWorldViewProjectionMatrixDirty = true;
			m_bWorldViewProjectionMatrixTransposeDirty = true;
			m_bWorldViewProjectionMatrixInverseDirty = true;
			m_bWorldViewProjectionMatrixInverseTransposeDirty = true;
		}
		
		m_bCameraPosInObjectSpaceDirty = true;
		m_bCameraRightInObjectSpaceDirty = true;
		m_bCameraUpInObjectSpaceDirty = true;
		m_bCameraDirInObjectSpaceDirty = true;
	}

	void ShaderParamSourceAuto::SetCurRenderableGrouped(const Renderable* pRenderable)
	{

	}
	
	void ShaderParamSourceAuto::SetCurCamera(const ObjectCamera* pCamera)
	{
		m_pCurCamera = pCamera;
	////View
		m_bViewMatrixDirty = true;
		m_bViewMatrixTransposeDirty	= true;
		m_bViewMatrixInverseDirty = true;
		m_bViewMatrixInverseTransposeDirty = true;
	////Projection
		m_bProjectionMatrixDirty = true;
		m_bProjectionMatrixTransposeDirty = true;
		m_bProjectionMatrixInverseDirty	= true;
		m_bProjectionMatrixInverseTransposeDirty = true;
	////World * View
		m_bWorldViewMatrixDirty	= true;
		m_bWorldViewMatrixTransposeDirty = true;
		m_bWorldViewMatrixInverseDirty = true;
		m_bWorldViewMatrixInverseTransposeDirty	= true;
	////View * Projection
		m_bViewProjectionMatrixDirty = true;
		m_bViewProjectionMatrixTransposeDirty = true;
		m_bViewProjectionMatrixInverseDirty	= true;
		m_bViewProjectionMatrixInverseTransposeDirty = true;
	////World * View * Projection
		m_bWorldViewProjectionMatrixDirty = true;
		m_bWorldViewProjectionMatrixTransposeDirty = true;
		m_bWorldViewProjectionMatrixInverseDirty = true;
		m_bWorldViewProjectionMatrixInverseTransposeDirty = true;
	
	////Camera
		m_bCameraPosInObjectSpaceDirty = true;
		m_bCameraRightInObjectSpaceDirty = true;
		m_bCameraUpInObjectSpaceDirty = true;
		m_bCameraDirInObjectSpaceDirty = true;
		m_bCameraPosDirty = true;
		m_bCameraRightDirty	= true;
		m_bCameraUpDirty = true; 
		m_bCameraDirDirty = true;
		m_bCameraParamDirty	= true;
	}
	
	const FVector4& ShaderParamSourceAuto::GetCameraPosInObjectSpace() const
	{
		if (m_bCameraPosInObjectSpaceDirty)
		{
            FVector3 v3 = FMath::TransformAffine(GetWorldMatrix_Inverse(), m_pCurCamera->GetPositionReal());
			m_vCameraPosInObjectSpace = FVector4(v3.x, v3.y, v3.z, 0.0f);
			m_bCameraPosInObjectSpaceDirty = false;
		}
		return m_vCameraPosInObjectSpace;
	}

	const FVector4& ShaderParamSourceAuto::GetCameraRightInObjectSpace() const
	{
		if(m_bCameraRightInObjectSpaceDirty)
		{
            FVector3 v3 = FMath::TransformAffine(GetWorldMatrix_Inverse(), m_pCurCamera->GetRightReal());
			m_vCameraRightInObjectSpace = FVector4(v3.x, v3.y, v3.z, 0.0f);
			m_bCameraRightInObjectSpaceDirty = false;
		}
		return m_vCameraRightInObjectSpace;
	}

	const FVector4& ShaderParamSourceAuto::GetCameraUpInObjectSpace() const
	{
		if(m_bCameraUpInObjectSpaceDirty)
		{
            FVector3 v3 = FMath::TransformAffine(GetWorldMatrix_Inverse(), m_pCurCamera->GetUpReal());
			m_vCameraUpInObjectSpace = FVector4(v3.x, v3.y, v3.z, 0.0f);
			m_bCameraUpInObjectSpaceDirty = false;
		}
		return m_vCameraUpInObjectSpace;
	}

	const FVector4& ShaderParamSourceAuto::GetCameraDirInObjectSpace() const
	{
		if(m_bCameraDirInObjectSpaceDirty)
		{
            FVector3 v3 = FMath::TransformAffine(GetWorldMatrix_Inverse(), m_pCurCamera->GetDirectionReal());
			m_vCameraDirInObjectSpace = FVector4(v3.x, v3.y, v3.z, 0.0f);
			m_bCameraDirInObjectSpaceDirty = false;
		}
		return m_vCameraDirInObjectSpace;
	}

	const FVector4& ShaderParamSourceAuto::GetCameraPos() const
	{
		if (m_bCameraPosDirty)
		{
			FVector3 vec3 = m_pCurCamera->GetPositionWorld();
			m_vCameraPos[0] = vec3[0];
			m_vCameraPos[1] = vec3[1];
			m_vCameraPos[2] = vec3[2];
			m_vCameraPos[3] = 1.0;
			m_bCameraPosDirty = false;
		}
		return m_vCameraPos;
	}

	const FVector4& ShaderParamSourceAuto::GetCameraRight() const
	{
		if (m_bCameraRightDirty)
		{
			FVector3 vec3 = m_pCurCamera->GetRightReal();
			m_vCameraRight[0] = vec3[0];
			m_vCameraRight[1] = vec3[1];
			m_vCameraRight[2] = vec3[2];
			m_vCameraRight[3] = 1.0;
			m_bCameraRightDirty = false;
		}
		return m_vCameraRight;
	}

	const FVector4& ShaderParamSourceAuto::GetCameraUp() const
	{
		if (m_bCameraUpDirty)
		{
			FVector3 vec3 = m_pCurCamera->GetUpReal();
			m_vCameraUp[0] = vec3[0];
			m_vCameraUp[1] = vec3[1];
			m_vCameraUp[2] = vec3[2];
			m_vCameraUp[3] = 1.0;
			m_bCameraUpDirty = false;
		}
		return m_vCameraUp;
	}

	const FVector4& ShaderParamSourceAuto::GetCameraDirection() const
	{
		if (m_bCameraDirDirty)
		{
			FVector3 vec3 = m_pCurCamera->GetDirectionReal();
			m_vCameraDir[0] = vec3[0];
			m_vCameraDir[1] = vec3[1];
			m_vCameraDir[2] = vec3[2];
			m_vCameraDir[3] = 1.0;
			m_bCameraDirDirty = false;
		}
		return m_vCameraDir;
	}

	float ShaderParamSourceAuto::GetCameraFOV() const
	{
		if (m_bCameraParamDirty)
		{
			m_vCameraParam.x = m_pCurCamera->GetFovY();
			m_vCameraParam.y = m_pCurCamera->GetNearClipDistance();
			m_vCameraParam.z = m_pCurCamera->GetFarClipDistance();
			m_vCameraParam.w = 0.0f; 
			m_bCameraParamDirty = false;
		}
		return m_vCameraParam.x;
	}

	float ShaderParamSourceAuto::GetCameraNearClipDistance() const
	{
		if (m_bCameraParamDirty)
		{
			m_vCameraParam.x = m_pCurCamera->GetFovY();
			m_vCameraParam.y = m_pCurCamera->GetNearClipDistance();
			m_vCameraParam.z = m_pCurCamera->GetFarClipDistance();
			m_vCameraParam.w = 0.0f; 
			m_bCameraParamDirty = false;
		}
		return m_vCameraParam.y;
	}

	float ShaderParamSourceAuto::GetCameraFarClipDistance() const
	{
		if(m_bCameraParamDirty)
		{
			m_vCameraParam.x = m_pCurCamera->GetFovY();
			m_vCameraParam.y = m_pCurCamera->GetNearClipDistance();
			m_vCameraParam.z = m_pCurCamera->GetFarClipDistance();
			m_vCameraParam.w = 0.0f; 
			m_bCameraParamDirty = false;
		}
		return m_vCameraParam.z;
	}

	const FVector4& ShaderParamSourceAuto::GetCameraParam() const
	{
		if (m_bCameraParamDirty)
		{
			m_vCameraParam.x = m_pCurCamera->GetFovY();
			m_vCameraParam.y = m_pCurCamera->GetNearClipDistance();
			m_vCameraParam.z = m_pCurCamera->GetFarClipDistance();
			m_vCameraParam.w = 0.0f; 
			m_bCameraParamDirty = false;
		}
		return m_vCameraParam;
	}

	void ShaderParamSourceAuto::SetCurViewport(const Viewport* pViewport)
	{
		m_pCurViewport = pViewport;
		m_bViewportSizeDirty = true;
	}
	
	const FVector4& ShaderParamSourceAuto::GetViewportSize() const
	{
		if (m_bViewportSizeDirty)
		{
			m_vViewportSize.x = (float)m_pCurViewport->GetActualWidth();
			m_vViewportSize.y = (float)m_pCurViewport->GetActualHeight(); 
			m_vViewportSize.z = 1.0f / m_vViewportSize.x;
			m_vViewportSize.w = 1.0f / m_vViewportSize.y;
			m_bViewportSizeDirty = false;
		}	
		return m_vViewportSize;
	}

	float ShaderParamSourceAuto::GetViewportWidth() const
	{
		if (m_bViewportSizeDirty)
		{
			m_vViewportSize.x = (float)m_pCurViewport->GetActualWidth();
			m_vViewportSize.y = (float)m_pCurViewport->GetActualHeight(); 
			m_vViewportSize.z = 1.0f / m_vViewportSize.x;
			m_vViewportSize.w = 1.0f / m_vViewportSize.y;
			m_bViewportSizeDirty = false;
		}	
		return m_vViewportSize.x;
	}

	float ShaderParamSourceAuto::GetViewportHeight() const
	{
		if (m_bViewportSizeDirty)
		{
			m_vViewportSize.x = (float)m_pCurViewport->GetActualWidth();
			m_vViewportSize.y = (float)m_pCurViewport->GetActualHeight(); 
			m_vViewportSize.z = 1.0f / m_vViewportSize.x;
			m_vViewportSize.w = 1.0f / m_vViewportSize.y;
			m_bViewportSizeDirty = false;
		}	
		return m_vViewportSize.y;
	}

	float ShaderParamSourceAuto::GetViewportWidth_Inverse() const
	{
		if (m_bViewportSizeDirty)
		{
			m_vViewportSize.x = (float)m_pCurViewport->GetActualWidth();
			m_vViewportSize.y = (float)m_pCurViewport->GetActualHeight(); 
			m_vViewportSize.z = 1.0f / m_vViewportSize.x;
			m_vViewportSize.w = 1.0f / m_vViewportSize.y;
			m_bViewportSizeDirty = false;
		}	
		return m_vViewportSize.z;
	}

	float ShaderParamSourceAuto::GetViewportHeight_Inverse() const
	{
		if (m_bViewportSizeDirty)
		{
			m_vViewportSize.x = (float)m_pCurViewport->GetActualWidth();
			m_vViewportSize.y = (float)m_pCurViewport->GetActualHeight(); 
			m_vViewportSize.z = 1.0f / m_vViewportSize.x;
			m_vViewportSize.w = 1.0f / m_vViewportSize.y;
			m_bViewportSizeDirty = false;
		}	
		return m_vViewportSize.w;
	}

	void ShaderParamSourceAuto::SetMainCamBoundsInfo(ObjectVisibleBoundsInfo* pInfo)
	{
		m_pMainCameraBoundsInfo	= pInfo;
		m_bSceneDepthRangeDirty = true;
	}

	const FVector4& ShaderParamSourceAuto::GetSceneDepthRange() const
	{
		if (m_bSceneDepthRangeDirty)
		{
			m_v4SceneDepthRange.x = m_pMainCameraBoundsInfo->m_fMinDisToCamera;
			m_v4SceneDepthRange.y = m_pMainCameraBoundsInfo->m_fMaxDisToCamera;
			m_v4SceneDepthRange.z = m_pMainCameraBoundsInfo->m_fMaxDisToCamera - m_pMainCameraBoundsInfo->m_fMinDisToCamera;
			m_v4SceneDepthRange.w = 1.0f / m_v4SceneDepthRange.z;
			m_bSceneDepthRangeDirty = false;
		}
		return m_v4SceneDepthRange;
	}

	void ShaderParamSourceAuto::SetCurLightArray(const ObjectLightPtrVector* pLights)
	{
		m_pCurLightArray = pLights;
		m_bShadowCameraDepthRangesDirty = true;
	}

	const ObjectLight* ShaderParamSourceAuto::GetLight(uint32 nIndex) const
	{	
		F_Assert(m_pCurLightArray && nIndex >= 0 && nIndex < (uint32)m_pCurLightArray->size() && "ShaderParamSourceAuto::GetLight")
		return (*m_pCurLightArray)[nIndex];
	}

	const FColor& ShaderParamSourceAuto::GetGlobalAmbient(uint8 nIndex /*= 0*/) const
	{
		return m_aGlobalAmbient[nIndex];
	}

	void ShaderParamSourceAuto::SetGlobalAmbient(const FColor& clAmbient, uint8 nIndex /*= 0*/)
	{
		m_aGlobalAmbient[nIndex] = clAmbient;
	}

	const FVector4& ShaderParamSourceAuto::GetTextureSize(uint32 nTexUnit) const
	{
		static FVector4 v4 = FMath::ms_v4Zero;
		F_Assert(m_pCurRenderable && "ShaderParamSourceAuto::GetTextureSize")


		return v4;
	}

	const FVector4& ShaderParamSourceAuto::GetTextureSize_Inverse(uint32 nTexUnit) const
	{
		static FVector4 v4 = FMath::ms_v4Zero;
		F_Assert(m_pCurRenderable && "ShaderParamSourceAuto::GetTextureSize_Inverse")


		return v4;
	}

	const FVector4& ShaderParamSourceAuto::GetTextureSize_Packed(uint32 nTexUnit) const
	{
		static FVector4 v4 = FMath::ms_v4Zero;
		F_Assert(m_pCurRenderable && "ShaderParamSourceAuto::GetTextureSize_Packed")
		

		return v4;
	}

	const FVector4& ShaderParamSourceAuto::GetTextureUVTranslate(uint32 nTexUnit) const
	{
		static FVector4 v4 = FMath::ms_v4Zero;
		F_Assert(m_pCurRenderable && "ShaderParamSourceAuto::GetTextureUVTranslate")


		return v4;
	}

	const FVector4& ShaderParamSourceAuto::GetTextureUVRotate(uint32 nTexUnit) const
	{
		static FVector4 v4 = FMath::ms_v4Zero;
		F_Assert(m_pCurRenderable && "ShaderParamSourceAuto::GetTextureUVRotate")


		return v4;
	}

	const FVector4& ShaderParamSourceAuto::GetTextureUVScale(uint32 nTexUnit) const
	{
		static FVector4 v4 = FMath::ms_v4Zero;
		F_Assert(m_pCurRenderable && "ShaderParamSourceAuto::GetTextureUVScale")


		return v4;
	}

	const FMatrix4& ShaderParamSourceAuto::GetTextureUVTransformMatrix(uint32 nTexUnit) const
	{
		static FMatrix4 m4 = FMath::ms_mat4Unit;
		F_Assert(m_pCurRenderable && "ShaderParamSourceAuto::GetTextureUVTransformMatrix")

		
		return m4;
	}

	float ShaderParamSourceAuto::GetTimeElapsed() const
	{
		return 0.0f;
	}

	float ShaderParamSourceAuto::GetTimeElapsed_0_X(float x) const
	{
		return fmod(this->GetTimeElapsed(), x);
	}

	float ShaderParamSourceAuto::GetTimeElapsed_Sin_0_X(float x) const
	{
		return FMath::Sin(this->GetTimeElapsed_0_X(x), true);
	}

	float ShaderParamSourceAuto::GetTimeElapsed_Cos_0_X(float x) const
	{
		return FMath::Cos(this->GetTimeElapsed_0_X(x), true);
	}

	float ShaderParamSourceAuto::GetTimeElapsed_Tan_0_X(float x) const
	{
		return tan(this->GetTimeElapsed_0_X(x));
	}

	const FVector4& ShaderParamSourceAuto::GetTimeElapsed_Packed_0_X(float x) const
	{
		static FVector4 v4 = FMath::ms_v4Zero;
		float t = this->GetTimeElapsed_0_X(x);
		v4.x = t;
		v4.y = FMath::Sin(t, true);
		v4.z = FMath::Cos(t, true);
		v4.w = tan(t);
		return v4;
	}

	float ShaderParamSourceAuto::GetTimeElapsed_0_1(float x) const
	{
		return this->GetTimeElapsed_0_X(x) / x;
	}

	float ShaderParamSourceAuto::GetTimeElapsed_Sin_0_1(float x) const
	{
		return FMath::Sin(this->GetTimeElapsed_0_1(x), true);
	}

	float ShaderParamSourceAuto::GetTimeElapsed_Cos_0_1(float x) const
	{
		return FMath::Cos(this->GetTimeElapsed_0_1(x), true);
	}

	float ShaderParamSourceAuto::GetTimeElapsed_Tan_0_1(float x) const
	{	
		return tan(this->GetTimeElapsed_0_1(x));
	}

	const FVector4& ShaderParamSourceAuto::GetTimeElapsed_Packed_0_1(float x) const
	{
		static FVector4 v4 = FMath::ms_v4Zero;
		float t = this->GetTimeElapsed_0_1(x);
		v4.x = t;
		v4.y = FMath::Sin(t, true);
		v4.z = FMath::Cos(t, true);
		v4.w = tan(t);
		return v4;
	}

	float ShaderParamSourceAuto::GetTimeElapsed_0_2PI(float x) const
	{
		return this->GetTimeElapsed_0_1(x) * FMath::ms_fPI_Two;
	}	

	float ShaderParamSourceAuto::GetTimeElapsed_Sin_0_2PI(float x) const
	{
		return FMath::Sin(this->GetTimeElapsed_0_2PI(x), true);
	}

	float ShaderParamSourceAuto::GetTimeElapsed_Cos_0_2PI(float x) const
	{
		return FMath::Cos(this->GetTimeElapsed_0_2PI(x), true);
	}

	float ShaderParamSourceAuto::GetTimeElapsed_Tan_0_2PI(float x) const
	{
		return tan(this->GetTimeElapsed_0_2PI(x));
	}

	const FVector4& ShaderParamSourceAuto::GetTimeElapsed_Packed_0_2PI(float x) const
	{
		static FVector4 v4 = FMath::ms_v4Zero;
		float t = this->GetTimeElapsed_0_2PI(x);
		v4.x = t;
		v4.y = FMath::Sin(t, true);
		v4.z = FMath::Cos(t, true);
		v4.w = tan(t);
		return v4;
	}

	float ShaderParamSourceAuto::GetFrameTime() const
	{
        return 0.0f;
	}

	float ShaderParamSourceAuto::GetFPS() const
	{
        return 0.0f;
	}

}; //LostPeterEngine