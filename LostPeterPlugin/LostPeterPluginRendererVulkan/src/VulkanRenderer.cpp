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

#include "../include/VulkanRenderer.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanRenderer::VulkanRenderer()
        : Renderer("VulkanRenderer")
    {
        
    }

    VulkanRenderer::~VulkanRenderer()
    {
        
    }

    void VulkanRenderer::Destroy()
    {

    }

    RenderWindow* VulkanRenderer::Init(bool bAutoCreateWindow, const String& strWndTitle /*= "Render Window"*/)
    {
        return nullptr;
    }

    EVertexElementDataType VulkanRenderer::GetColorVertexElementType() const
    {
        return E_VertexElementData_Float1;
    }

    RenderWindow* VulkanRenderer::CreateRenderWindow(const String& strName, uint32 nWidth, uint32 nHeight, bool bFullScreen,
												     const String2StringMap* pParams /*= nullptr*/, bool bShowWindow /*= true*/)
    {
        return nullptr;
    }

    bool VulkanRenderer::IsDeviceLost()
    {
        return false;
    }

    bool VulkanRenderer::BeginFrame()
    {   
        
        return true;
    }
    bool VulkanRenderer::EndFrame()
    {
        return true;
    }

    bool VulkanRenderer::SetViewport(Viewport* pViewport)
    {
        return true;
    }
    bool VulkanRenderer::SetRenderTarget(RenderTarget* pRenderTarget)
    {
        return true;
    }
    bool VulkanRenderer::SetDepthStencil(void* pDepth)
    {
        return true;
    }

    bool VulkanRenderer::ClearFrameBuffer(uint32 nBuffers, const FColor& color /*= FMath::ms_clBlack*/, float fDepth /*= 1.0f*/, uint16 nStencil /*= 0*/)
    {
        return true;
    }
    bool VulkanRenderer::ClearFrameBuffer(const FRectI& rect, uint32 nBuffers, const FColor& color /*= FMath::ms_clBlack*/, float fDepth /*= 1.0f*/, uint16 nStencil /*= 0*/)
    {
        return true;
    }

    bool VulkanRenderer::SetWorldMatrix(const FMatrix4& matWorld)
    {
        return true;
    }
    bool VulkanRenderer::SetWorldMatrices(const FMatrix4* pMatWorld, int nCount)
    {
        return true;
    }
    bool VulkanRenderer::SetViewMatrix(const FMatrix4& matView)
    {
        return true;
    }
    bool VulkanRenderer::SetProjectionMatrix(const FMatrix4& matProj)
    {
        return true;
    }

}; //LostPeterPluginRendererVulkan