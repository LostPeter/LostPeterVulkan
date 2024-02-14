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

namespace LostPeterPluginRendererVulkan
{
    class VulkanRenderer : public Renderer
    {
    public:
        VulkanRenderer();
        ~VulkanRenderer();

    public:
    protected:
        VulkanInstance* m_pVulkanInstance;


    public:
        virtual void Destroy();
        virtual RenderWindow* Init(bool bAutoCreateWindow, const String& strWndTitle = "Render Window");
            
    protected:
		virtual bool initRenderCapabilities();
		virtual void initFromRenderCapabilities(RenderTarget* pPrimary);
        virtual bool initStreamManager();
        virtual bool initShaderProgramManager();
        virtual bool initTextureManager();

    public:
        F_FORCEINLINE VulkanInstance* GetVulkanInstance() const { return m_pVulkanInstance; }
        VulkanDevice* GetDevice();
    
    public:
        virtual FVertexElementDataType GetColorVertexElementType() const;
        virtual void ConvertProjectionMatrix(const FMatrix4& matIn, FMatrix4& matOut, bool bForShaderProgram = false);
        
        virtual RenderWindow* CreateRenderWindow(const String& strName, 
                                                 uint32 nWidth, 
                                                 uint32 nHeight, 
												 const String2StringMap* pParams = nullptr);

    public:
        virtual bool BeginFrame();
            
		virtual bool EndFrame();

        virtual bool SetViewport(Viewport* pViewport);
		virtual bool SetRenderTarget(RenderTarget* pRenderTarget);
		virtual bool SetDepthStencil(void* pDepth);

		virtual bool ClearFrameBuffer(uint32 nBuffers, 
                                      const FColor& color = FMath::ms_clBlack, 
                                      float fDepth = 1.0f, 
                                      uint16 nStencil = 0);
		virtual bool ClearFrameBuffer(const FRectI& rect, 
                                      uint32 nBuffers, 
                                      const FColor& color = FMath::ms_clBlack, 
                                      float fDepth = 1.0f, 
                                      uint16 nStencil = 0);
		
        
		virtual bool SetWorldMatrix(const FMatrix4& matWorld);
		virtual bool SetWorldMatrices(const FMatrix4* pMatWorld, int nCount);
		virtual bool SetViewMatrix(const FMatrix4& matView);
		virtual bool SetProjectionMatrix(const FMatrix4& matProj);
        
    public:
        
    };

}; //LostPeterPluginRendererVulkan

#endif