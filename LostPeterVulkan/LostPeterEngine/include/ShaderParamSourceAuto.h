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

#ifndef _SHADER_PARAM_SOURCE_AUTO_H_
#define _SHADER_PARAM_SOURCE_AUTO_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport ShaderParamSourceAuto : public Base
    {
    public:
        ShaderParamSourceAuto(const String& strNameShaderParamSource);
        virtual ~ShaderParamSourceAuto();

    public: 
    protected:
    ////World
		mutable FMatrix4 m_aWorldMatrix[256];
		mutable uint32 m_nWorldMatrixCount;
		mutable const FMatrix4* m_pWorldMatrixArray;
		mutable float* m_pWorldFloat;
		mutable FMatrix4 m_WorldMatrix_Transpose;
		mutable FMatrix4 m_WorldMatrix_Inverse;
		mutable FMatrix4 m_WorldMatrix_InverseTranspose;
		mutable bool m_bWorldMatrixDirty;
		mutable bool m_bWorldMatrixTransposeDirty;
		mutable bool m_bWorldMatrixInverseDirty;
		mutable bool m_bWorldMatrixInverseTransposeDirty;
	////View
		mutable FMatrix4 m_ViewMatrix;
		mutable FMatrix4 m_ViewMatrix_Transpose;
		mutable FMatrix4 m_ViewMatrix_Inverse;
		mutable FMatrix4 m_ViewMatrix_InverseTranspose;
		mutable bool m_bViewMatrixDirty;
		mutable bool m_bViewMatrixTransposeDirty;
		mutable bool m_bViewMatrixInverseDirty;
		mutable bool m_bViewMatrixInverseTransposeDirty;
	////Projection
		mutable FMatrix4 m_ProjectionMatrix;
		mutable FMatrix4 m_ProjectionMatrix_Transpose;
		mutable FMatrix4 m_ProjectionMatrix_Inverse;
		mutable FMatrix4 m_ProjectionMatrix_InverseTranspose;
		mutable bool m_bProjectionMatrixDirty;
		mutable bool m_bProjectionMatrixTransposeDirty;
		mutable bool m_bProjectionMatrixInverseDirty;
		mutable bool m_bProjectionMatrixInverseTransposeDirty;
	////World * View	
		mutable FMatrix4 m_WorldViewMatrix;
		mutable FMatrix4 m_WorldViewMatrix_Transpose;
		mutable FMatrix4 m_WorldViewMatrix_Inverse;
		mutable FMatrix4 m_WorldViewMatrix_InverseTranspose;
		mutable bool m_bWorldViewMatrixDirty;
		mutable bool m_bWorldViewMatrixTransposeDirty;
		mutable bool m_bWorldViewMatrixInverseDirty;
		mutable bool m_bWorldViewMatrixInverseTransposeDirty;
	////View * Projection	
		mutable FMatrix4 m_ViewProjectionMatrix;
		mutable FMatrix4 m_ViewProjectionMatrix_Transpose;
		mutable FMatrix4 m_ViewProjectionMatrix_Inverse;
		mutable FMatrix4 m_ViewProjectionMatrix_InverseTranspose;
		mutable bool m_bViewProjectionMatrixDirty;
		mutable bool m_bViewProjectionMatrixTransposeDirty;
		mutable bool m_bViewProjectionMatrixInverseDirty;
		mutable bool m_bViewProjectionMatrixInverseTransposeDirty;
	////World * View * Projection	
		mutable FMatrix4 m_WorldViewProjectionMatrix;
		mutable FMatrix4 m_WorldViewProjectionMatrix_Transpose;
		mutable FMatrix4 m_WorldViewProjectionMatrix_Inverse;
		mutable FMatrix4 m_WorldViewProjectionMatrix_InverseTranspose;
		mutable bool m_bWorldViewProjectionMatrixDirty;
		mutable bool m_bWorldViewProjectionMatrixTransposeDirty;
		mutable bool m_bWorldViewProjectionMatrixInverseDirty;
		mutable bool m_bWorldViewProjectionMatrixInverseTransposeDirty;
	////Renderable,RenderTarget,ObjectCamera,Viewport,SceneManager
		const Renderable* m_pCurRenderable;
		const RenderTarget* m_pCurRenderTarget;
		const ObjectCamera*	m_pCurCamera;
		mutable FVector4 m_vCameraPosInObjectSpace;
		mutable FVector4 m_vCameraRightInObjectSpace;
		mutable FVector4 m_vCameraUpInObjectSpace;
		mutable FVector4 m_vCameraDirInObjectSpace;
		mutable FVector4 m_vCameraPos;
		mutable FVector4 m_vCameraRight;
		mutable FVector4 m_vCameraUp;
		mutable FVector4 m_vCameraDir;
		mutable FVector4 m_vCameraParam;
		mutable bool m_bCameraPosInObjectSpaceDirty;
		mutable bool m_bCameraRightInObjectSpaceDirty;
		mutable bool m_bCameraUpInObjectSpaceDirty;
		mutable bool m_bCameraDirInObjectSpaceDirty;
		mutable bool m_bCameraPosDirty;
		mutable bool m_bCameraRightDirty;
		mutable bool m_bCameraUpDirty;
		mutable bool m_bCameraDirDirty;
		mutable bool m_bCameraParamDirty;
		const Viewport*	m_pCurViewport;
		mutable FVector4 m_vViewportSize;
		mutable bool m_bViewportSizeDirty;
		const SceneManager* m_pCurSceneManager;
		const ObjectVisibleBoundsInfo* m_pMainCameraBoundsInfo;
		mutable FVector4 m_v4SceneDepthRange;
		mutable bool m_bSceneDepthRangeDirty;
	////Light
		const ObjectLightPtrVector* m_pCurLightArray;
		mutable bool m_bShadowCameraDepthRangesDirty;
		mutable FColor m_aGlobalAmbient[C_CONFIG_MAX_LIGHT_COUNT];
	////Material
		mutable FColor m_clAmbient;
		mutable FColor m_clDiffuse;
		mutable FColor m_clSpecular;
		mutable FColor m_clEmissive;
		mutable float m_fShininess;
	////Fog

	////Texture

	////Time
		//ControllerManager* m_pControllerManager;

    public:
    ////World
		F_FORCEINLINE const float* GetWorldFloat() const { return m_pWorldFloat; }
		void SetWorldFloat(float* pFloat, uint32 nCount);
		const FMatrix4* GetWorldMatrixArray() const;
		void SetWorldMatrixArray(const FMatrix4* pMatrix, uint32 nCount);
		uint32 GetWorldMatrixCount() const;
		const FMatrix4& GetWorldMatrix() const;
		void SetWorldMatrix(const FMatrix4& matWorld);
		const FMatrix4& GetWorldMatrix_Transpose() const;
		const FMatrix4& GetWorldMatrix_Inverse() const;
		const FMatrix4& GetWorldMatrix_InverseTranspose() const;
		F_FORCEINLINE void SetWorldMatrixDirty(bool bWorldMatrixDirty) { m_bWorldMatrixDirty = bWorldMatrixDirty; }
		F_FORCEINLINE void SetWorldMatrixTransposeDirty(bool bWorldMatrixTransposeDirty) { m_bWorldMatrixTransposeDirty = bWorldMatrixTransposeDirty; }
		F_FORCEINLINE void SetWorldMatrixInverseDirty(bool bWorldMatrixInverseDirty) { m_bWorldMatrixInverseDirty = bWorldMatrixInverseDirty; }
		F_FORCEINLINE void SetWorldMatrixInverseTransposeDirty(bool bWorldMatrixInverseTransposeDirty) { m_bWorldMatrixInverseTransposeDirty = bWorldMatrixInverseTransposeDirty; }
	////View
		const FMatrix4& GetViewMatrix() const;
		const FMatrix4& GetViewMatrix_Transpose() const;
		const FMatrix4& GetViewMatrix_Inverse() const;
		const FMatrix4& GetViewMatrix_InverseTranspose() const;
		F_FORCEINLINE void SetViewMatrixDirty(bool bViewMatrixDirty) { m_bViewMatrixDirty = bViewMatrixDirty; }
		F_FORCEINLINE void SetViewMatrixTransposeDirty(bool bViewMatrixTransposeDirty) { m_bViewMatrixTransposeDirty = bViewMatrixTransposeDirty; }
		F_FORCEINLINE void SetViewMatrixInverseDirty(bool bViewMatrixInverseDirty) { m_bViewMatrixInverseDirty = bViewMatrixInverseDirty; }
		F_FORCEINLINE void SetViewMatrixInverseTransposeDirty(bool bViewMatrixInverseTransposeDirty) { m_bViewMatrixInverseTransposeDirty = bViewMatrixInverseTransposeDirty; }
	////Projection
		const FMatrix4& GetProjectionMatrix() const;
		const FMatrix4& GetProjectionMatrix_Transpose() const;
		const FMatrix4& GetProjectionMatrix_Inverse() const;
		const FMatrix4& GetProjectionMatrix_InverseTranspose() const;
		F_FORCEINLINE void SetProjectionMatrixDirty(bool bProjectionMatrixDirty) { m_bProjectionMatrixDirty = bProjectionMatrixDirty; }
		F_FORCEINLINE void SetProjectionMatrixTransposeDirty(bool bProjectionMatrixTransposeDirty)	{ m_bProjectionMatrixTransposeDirty = bProjectionMatrixTransposeDirty; }
		F_FORCEINLINE void SetProjectionMatrixInverseDirty(bool bProjectionMatrixInverseDirty) { m_bProjectionMatrixInverseDirty = bProjectionMatrixInverseDirty; }
		F_FORCEINLINE void SetProjectionMatrixInverseTransposeDirty(bool bProjectionMatrixInverseTransposeDirty) { m_bProjectionMatrixInverseTransposeDirty = bProjectionMatrixInverseTransposeDirty; }
	////World * View	
		const FMatrix4& GetWorldViewMatrix() const;
		const FMatrix4& GetWorldViewMatrix_Transpose() const;
		const FMatrix4& GetWorldViewMatrix_Inverse() const;
		const FMatrix4& GetWorldViewMatrix_InverseTranspose() const;
		F_FORCEINLINE void SetWorldViewMatrixDirty(bool bWorldViewMatrixDirty) { m_bWorldViewMatrixDirty = bWorldViewMatrixDirty; }
		F_FORCEINLINE void SetWorldViewMatrixTransposeDirty(bool bWorldViewMatrixTransposeDirty) { m_bWorldViewMatrixTransposeDirty = bWorldViewMatrixTransposeDirty; }
		F_FORCEINLINE void SetWorldViewMatrixInverseDirty(bool bWorldViewMatrixInverseDirty) { m_bWorldViewMatrixInverseDirty = bWorldViewMatrixInverseDirty; }
		F_FORCEINLINE void SetWorldViewMatrixInverseTransposeDirty(bool bWorldViewMatrixInverseTransposeDirty) { m_bWorldViewMatrixInverseTransposeDirty = bWorldViewMatrixInverseTransposeDirty; }
	////View * Projection		
		const FMatrix4& GetViewProjectionMatrix() const;
		const FMatrix4& GetViewProjectionMatrix_Transpose() const;
		const FMatrix4& GetViewProjectionMatrix_Inverse() const;
		const FMatrix4& GetViewProjectionMatrix_InverseTranspose() const;
		F_FORCEINLINE void SetViewProjectionMatrixDirty(bool bViewProjectionMatrixDirty) { m_bViewProjectionMatrixDirty = bViewProjectionMatrixDirty; }
		F_FORCEINLINE void SetViewProjectionMatrixTransposeDirty(bool bViewProjectionMatrixTransposeDirty) { m_bViewProjectionMatrixTransposeDirty = bViewProjectionMatrixTransposeDirty; }
		F_FORCEINLINE void SetViewProjectionMatrixInverseDirty(bool bVal) { m_bViewProjectionMatrixInverseDirty = bVal; }
		F_FORCEINLINE void SetViewProjectionMatrixInverseTransposeDirty(bool bVal) { m_bViewProjectionMatrixInverseTransposeDirty = bVal; }
	////World * View * Projection			
		const FMatrix4& GetWorldViewProjectionMatrix() const;
		const FMatrix4& GetWorldViewProjectionMatrix_Transpose() const;
		const FMatrix4& GetWorldViewProjectionMatrix_Inverse() const;
		const FMatrix4& GetWorldViewProjectionMatrix_InverseTranspose() const;
		F_FORCEINLINE void  SetWorldViewProjectionMatrixDirty(bool bWorldViewProjectionMatrixDirty) { m_bWorldViewProjectionMatrixDirty = bWorldViewProjectionMatrixDirty; }
		F_FORCEINLINE void  SetWorldViewProjectionMatrixTransposeDirty(bool bWorldViewProjectionMatrixTransposeDirty) { m_bWorldViewProjectionMatrixTransposeDirty = bWorldViewProjectionMatrixTransposeDirty; }
		F_FORCEINLINE void  SetWorldViewProjectionMatrixInverseDirty(bool bWorldViewProjectionMatrixInverseDirty)	{ m_bWorldViewProjectionMatrixInverseDirty = bWorldViewProjectionMatrixInverseDirty; }
		F_FORCEINLINE void  SetWorldViewProjectionMatrixInverseTransposeDirty(bool bWorldViewProjectionMatrixInverseTransposeDirty) { m_bWorldViewProjectionMatrixInverseTransposeDirty = bWorldViewProjectionMatrixInverseTransposeDirty; }
	////Renderable,RenderTarget,ObjectCamera,Viewport,SceneManager
		void SetCurRenderable(const Renderable* pRenderable);
		void SetCurRenderableGrouped(const Renderable* pRenderable);
		F_FORCEINLINE const RenderTarget* GetCurRenderTarget() const { return m_pCurRenderTarget; }
		F_FORCEINLINE void SetCurRenderTarget(const RenderTarget* pRenderTarget) { m_pCurRenderTarget = pRenderTarget; }
		void SetCurCamera(const ObjectCamera* pCamera);
		const FVector4& GetCameraPosInObjectSpace() const;
		const FVector4& GetCameraRightInObjectSpace() const;
		const FVector4& GetCameraUpInObjectSpace() const;
		const FVector4& GetCameraDirInObjectSpace() const;
		const FVector4& GetCameraPos() const;
		const FVector4& GetCameraRight() const;
		const FVector4& GetCameraUp() const;
		const FVector4& GetCameraDirection() const;
		float GetCameraFOV() const;
		float GetCameraNearClipDistance() const;
		float GetCameraFarClipDistance() const;
		const FVector4& GetCameraParam() const;
		void SetCurViewport(const Viewport* pViewport);
		const FVector4& GetViewportSize() const;
		float GetViewportWidth() const;
		float GetViewportHeight() const;
		float GetViewportWidth_Inverse() const;
		float GetViewportHeight_Inverse() const;
		F_FORCEINLINE void SetCurSceneManager(const SceneManager* pSceneManager) { m_pCurSceneManager = pSceneManager; }
		void SetMainCamBoundsInfo(ObjectVisibleBoundsInfo* pInfo);
		const FVector4& GetSceneDepthRange() const;
	////Light
		void SetCurLightArray(const ObjectLightPtrVector* pLights);
		const ObjectLight* GetLight(uint32 nIndex) const;
		const FColor& GetGlobalAmbient(uint8 nIndex = 0) const;
		void	SetGlobalAmbient(const FColor& clAmbient,uint8 nIndex = 0);
	////Material
		F_FORCEINLINE const FColor& GetAmbient() const { return m_clAmbient; }
		F_FORCEINLINE const FColor& GetDiffuse() const { return m_clDiffuse; }
		F_FORCEINLINE const FColor& GetSpecular() const { return m_clSpecular; }
		F_FORCEINLINE const FColor& GetEmissive() const { return m_clEmissive; }
		F_FORCEINLINE float	 GetShininess() const { return m_fShininess; }
		F_FORCEINLINE void SetMaterialParam(const FColor& clAmbient,const FColor& clDiffuse,const FColor& clSpecular,
									const FColor& clEmissive,float fShininess)
		{
			m_clAmbient		= clAmbient;
			m_clDiffuse		= clDiffuse;
			m_clSpecular	= clSpecular;
			m_clEmissive	= clEmissive;
			m_fShininess	= fShininess;
		}
	////Fog

	////Texture
		const FVector4& GetTextureSize(uint32 nTexUnit) const;
		const FVector4& GetTextureSize_Inverse(uint32 nTexUnit) const;
		const FVector4& GetTextureSize_Packed(uint32 nTexUnit) const;

		const FVector4& GetTextureUVTranslate(uint32 nTexUnit) const;
		const FVector4& GetTextureUVRotate(uint32 nTexUnit) const;
		const FVector4& GetTextureUVScale(uint32 nTexUnit) const;
		const FMatrix4& GetTextureUVTransformMatrix(uint32 nTexUnit) const;

	////Time
		float	GetTimeElapsed() const;
		float	GetTimeElapsed_0_X(float x) const;
		float	GetTimeElapsed_Sin_0_X(float x) const;
		float	GetTimeElapsed_Cos_0_X(float x) const;
		float	GetTimeElapsed_Tan_0_X(float x) const;
		const FVector4& GetTimeElapsed_Packed_0_X(float x) const;
		float	GetTimeElapsed_0_1(float x) const;
		float	GetTimeElapsed_Sin_0_1(float x) const;
		float	GetTimeElapsed_Cos_0_1(float x) const;
		float	GetTimeElapsed_Tan_0_1(float x) const;
		const FVector4& GetTimeElapsed_Packed_0_1(float x) const;
		float	GetTimeElapsed_0_2PI(float x) const;
		float	GetTimeElapsed_Sin_0_2PI(float x) const;
		float	GetTimeElapsed_Cos_0_2PI(float x) const;
		float	GetTimeElapsed_Tan_0_2PI(float x) const;
		const FVector4& GetTimeElapsed_Packed_0_2PI(float x) const;
		float	GetFrameTime() const;
		float	GetFPS() const;
    };

}; //LostPeterEngine

#endif