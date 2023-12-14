/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-18
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport Renderer : public Base
                                , public FNonCopyable
    {
    public:
        Renderer(const String& nameRenderer);
        virtual ~Renderer();

    public:
    protected:
		RenderCapabilities* m_pRenderCapabilities;
		VertexDeclarationManager* m_pVertexDeclarationManager;
		StreamVertexBindingManager* m_pVertexStreamBindingManager;
		StreamManager* m_pStreamManager;
		TextureManager* m_pTextureManager;

        RendererListenerPtrList m_listRendererListeners;
        RenderTargetManager* m_pRenderTargetManager;
        RenderWindow* m_pRenderWindow;
		RenderTarget* m_pActiveRenderTarget;
		Viewport* m_pActiveViewport;
		ObjectCamera* m_pActiveObjectCamera;

        bool m_bRendererIsInit;

        bool m_bFullScreen;
		bool m_bFakeFullScreen;
		bool m_bUseNvPerfHud;
		bool m_bVSync;
		bool m_bWBuffer;
		bool m_bEmptyGpuBuffer;

        uint32 m_nCurWidth;
		uint32 m_nCurHeight;

    public:
        F_FORCEINLINE RenderWindow* GetRenderWindow() const { return m_pRenderWindow; }
        F_FORCEINLINE void SetRenderWindow(RenderWindow* pRenderWindow) { m_pRenderWindow = pRenderWindow; }
        F_FORCEINLINE RenderTarget* GetActiveRenderTarget() const { return m_pActiveRenderTarget; }
        F_FORCEINLINE void SetActiveRenderTarget(RenderTarget* pRenderTarget) { m_pActiveRenderTarget = pRenderTarget; }
		F_FORCEINLINE Viewport* GetActiveViewport() const { return m_pActiveViewport; }
		F_FORCEINLINE ObjectCamera* GetActiveCamera() const { return m_pActiveObjectCamera; }
		
		F_FORCEINLINE bool	IsRendererInit() const { return m_bRendererIsInit; }

        F_FORCEINLINE bool	IsFullScreen() const { return m_bFullScreen; }
		F_FORCEINLINE bool	IsFakeFullScreen() const { return m_bFakeFullScreen; }
		F_FORCEINLINE bool	IsUseNvPerfHud() const { return m_bUseNvPerfHud; }
		F_FORCEINLINE bool	IsVSync() const { return m_bVSync; }
		F_FORCEINLINE bool	GetWBufferEnable() const { return m_bWBuffer; }
		F_FORCEINLINE void	SetWBufferEnable(bool b) { m_bWBuffer = b;}
		F_FORCEINLINE bool	IsEmptyGpuBuffer() const { return m_bEmptyGpuBuffer; }

    public:
        virtual void Destroy();
        virtual RenderWindow* Init(bool bAutoCreateWindow, const String& strWndTitle = "Render Window") = 0;

		virtual RenderTarget* GetRenderTarget(const String& strName);
		virtual void AttachRenderTarget(RenderTarget* pRenderTarget);
		virtual RenderTarget* DetachRenderTarget(const String& strName);
		virtual void DestroyRenderTarget(const String& strName);
		virtual void DestroyRenderWindow(const String& strName);
		virtual void DestroyRenderTexture(const String& strName);
		virtual void UpdateRenderTargetPriority(uint8 nPriority, RenderTarget* pRenderTarget);
		virtual void UpdateRenderTargetAll();
		virtual void NotifyObjectCameraRemoved(const ObjectCamera* pObjectCamera);

        virtual void AddRendererListener(RendererListener* pRendererListener);
		virtual void RemoveRendererListener(RendererListener* pRendererListener);

	protected:
		virtual bool initRenderCapabilities() = 0;
		virtual void initFromRenderCapabilities(RenderTarget* pPrimary) = 0;
        
    public:
		virtual void ConvertColorValue(const FColor& color, uint32* pDest);
		virtual EVertexElementDataType GetColorVertexElementType() const = 0;

		virtual RenderWindow* CreateRenderWindow(const String& strName, 
												 uint32 nWidth, 
												 uint32 nHeight, 
												 bool bFullScreen,
												 const String2StringMap* pParams = nullptr, 
												 bool bShowWindow = true) = 0;

		virtual bool IsDeviceLost() = 0;

        virtual void RenderEvent_Begin(const char* szEventName)	{ }
		virtual void RenderEvent_End() { }

		virtual void Present();
		virtual void UpdateOtherRenderWindows() { }

    public:
		virtual bool BeginFrame() = 0;
		virtual bool EndFrame() = 0;

        virtual bool SetViewport(Viewport* pViewport) = 0;
		virtual bool SetRenderTarget(RenderTarget* pRenderTarget) = 0;
		virtual bool SetDepthStencil(void* pDepth) = 0;

		virtual bool ClearFrameBuffer(uint32 nBuffers, 
									  const FColor& color = FMath::ms_clBlack, 
									  float fDepth = 1.0f, 
									  uint16 nStencil = 0) = 0;
		virtual bool ClearFrameBuffer(const FRectI& rect, 
									  uint32 nBuffers, 
									  const FColor& color = FMath::ms_clBlack, 
									  float fDepth = 1.0f, 
									  uint16 nStencil = 0) = 0;
		
        
		virtual bool SetWorldMatrix(const FMatrix4& matWorld) = 0;
		virtual bool SetWorldMatrices(const FMatrix4* pMatWorld, int nCount) { return false; }
		virtual bool SetViewMatrix(const FMatrix4& matView) = 0;
		virtual bool SetProjectionMatrix(const FMatrix4& matProj) = 0;
		virtual bool SetViewProjectionMatrix(ObjectCamera* pObjectCamera);
		
    };

}; //LostPeterEngine

#endif