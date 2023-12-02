/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-23
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_RENDERER_H_
#define _VULKAN_RENDERER_H_

#include "VulkanPreDefine.h"

namespace LostPeter
{
    class VulkanRenderer : public Renderer
    {
    public:
        VulkanRenderer();
        ~VulkanRenderer();

    public:
    protected:
       

    public:
        virtual void Destroy();
        virtual RenderWindow* Init(bool bAutoCreateWindow, const String& strWndTitle = "Render Window");

    
    public:
        virtual RenderWindow* CreateRenderWindow(const String& strName, uint32 nWidth, uint32 nHeight, bool bFullScreen,
												 const String2StringMap* pParams = nullptr, bool bShowWindow = true);

        virtual bool IsDeviceLost();

    public:
        virtual bool BeginFrame();
		virtual bool EndFrame();

        virtual bool SetViewport(Viewport* pViewport);
		virtual bool SetRenderTarget(RenderTarget* pRenderTarget);
		virtual bool SetDepthStencil(void* pDepth);

		virtual bool ClearFrameBuffer(uint32 nBuffers, const FColor& color = FMath::ms_clBlack, float fDepth = 1.0f, uint16 nStencil = 0);
		virtual bool ClearFrameBuffer(const FRectI& rect, uint32 nBuffers, const FColor& color = FMath::ms_clBlack, float fDepth = 1.0f, uint16 nStencil = 0);
		
        
		virtual bool SetWorldMatrix(const FMatrix4& matWorld);
		virtual bool SetWorldMatrices(const FMatrix4* pMatWorld, int nCount);
		virtual bool SetViewMatrix(const FMatrix4& matView);
		virtual bool SetProjectionMatrix(const FMatrix4& matProj);
        
    };

}; //LostPeter

#endif