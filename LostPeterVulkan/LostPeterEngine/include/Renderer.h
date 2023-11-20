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
    class utilExport Renderer : public Base
                              , public FNonCopyable
    {
    public:
        Renderer(const String& nameRenderer);
        virtual ~Renderer();

    public:
    protected:
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
        LP_FORCEINLINE RenderWindow* GetRenderWindow() const { return m_pRenderWindow; }
        LP_FORCEINLINE void SetRenderWindow(RenderWindow* pRenderWindow) { m_pRenderWindow = pRenderWindow; }
        LP_FORCEINLINE RenderTarget* GetActiveRenderTarget() const { return m_pActiveRenderTarget; }
        LP_FORCEINLINE void SetActiveRenderTarget(RenderTarget* pRenderTarget) { m_pActiveRenderTarget = pRenderTarget; }
		LP_FORCEINLINE Viewport* GetActiveViewport() const { return m_pActiveViewport; }
		LP_FORCEINLINE ObjectCamera* GetActiveCamera() const { return m_pActiveObjectCamera; }
		
		LP_FORCEINLINE bool	IsRendererInit() const { return m_bRendererIsInit; }

        LP_FORCEINLINE bool	IsFullScreen() const { return m_bFullScreen; }
		LP_FORCEINLINE bool	IsFakeFullScreen() const { return m_bFakeFullScreen; }
		LP_FORCEINLINE bool	IsUseNvPerfHud() const { return m_bUseNvPerfHud; }
		LP_FORCEINLINE bool	IsVSync() const { return m_bVSync; }
		LP_FORCEINLINE bool	GetWBufferEnable() const { return m_bWBuffer; }
		LP_FORCEINLINE void	SetWBufferEnable(bool b) { m_bWBuffer = b;}
		LP_FORCEINLINE bool	IsEmptyGpuBuffer() const { return m_bEmptyGpuBuffer; }

    public:
        virtual void Destroy();
        virtual RenderWindow* Init(bool bAutoCreateWindow, const String& strWndTitle = "Render Window") = 0;
    
		virtual void UpdateRenderTargetPriority(uint8 nPriority, RenderTarget* pRenderTarget);
		virtual void UpdateRenderTargetAll();
		virtual void NotifyObjectCameraRemoved(const ObjectCamera* pObjectCamera);

        virtual void AddRendererListener(RendererListener* pRendererListener);
		virtual void RemoveRendererListener(RendererListener* pRendererListener);
        
    public:
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

		virtual bool ClearFrameBuffer(uint32 nBuffers, const FColor& color = FMath::ms_clBlack, float fDepth = 1.0f, uint16 nStencil = 0) = 0;
		virtual bool ClearFrameBuffer(const FRectI& rect, uint32 nBuffers, const FColor& color = FMath::ms_clBlack, float fDepth = 1.0f, uint16 nStencil = 0) = 0;
		
        
		virtual bool SetWorldMatrix(const FMatrix4& matWorld) = 0;
		virtual bool SetWorldMatrices(const FMatrix4* pMatWorld, int nCount) { return false; }
		virtual bool SetViewMatrix(const FMatrix4& matView) = 0;
		virtual bool SetProjectionMatrix(const FMatrix4& matProj) = 0;
		virtual bool SetViewProjectionMatrix(ObjectCamera* pObjectCamera);


    };

}; //LostPeterEngine

#endif