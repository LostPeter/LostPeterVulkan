/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-27
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_RENDER_FRAME_BUFFER_DESCRIPTOR_H_
#define _VULKAN_RENDER_FRAME_BUFFER_DESCRIPTOR_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanRenderFrameBufferDescriptor : public RenderFrameBufferDescriptor
    {
    public:
        VulkanRenderFrameBufferDescriptor(const String& nameRenderFrameBufferDescriptor,
                                          VulkanDevice* pDevice,
                                          VulkanSwapChain* pSwapChain);
        virtual ~VulkanRenderFrameBufferDescriptor();

    public:
    protected:
        VulkanDevice* m_pDevice;
        VulkanSwapChain* m_pSwapChain;
        VulkanRenderPass* m_pRenderPass;    
        VulkanFrameBuffer* m_pFrameBuffer;

    public:
        F_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }
        F_FORCEINLINE VulkanRenderPass* GetRenderPass() const { return m_pRenderPass; }
        F_FORCEINLINE VulkanFrameBuffer* GetFrameBuffer() const { return m_pFrameBuffer; }

    public:
        virtual void Destroy();
        virtual bool Init(FTextureType eTexture,
                          FPixelFormatType ePixelFormatColor,
                          FPixelFormatType ePixelFormatDepth,
                          FMSAASampleCountType eMSAASampleCount,
                          bool bHasImGUI);

    public:
    protected:
        void destroyTextures(TexturePtrVector& aTexture);

        virtual bool createTextureColor(FTextureType eTexture,
                                        FPixelFormatType ePixelFormatColor, 
                                        FMSAASampleCountType eMSAASampleCount);
        virtual bool createTextureDepth(FTextureType eTexture,
                                        FPixelFormatType ePixelFormatDepth, 
                                        FMSAASampleCountType eMSAASampleCount);
        virtual bool createTextureSwapChain();

        virtual bool createRenderPass();
        virtual bool createRenderFrameBuffer();

    };

}; //LostPeterPluginRendererVulkan

#endif