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
                                          VulkanRenderWindow* pRenderWindow);
        virtual ~VulkanRenderFrameBufferDescriptor();

    public:
    protected:
        VulkanDevice* m_pDevice;
        VulkanRenderWindow* m_pRenderWindow;
        VulkanRenderPass* m_pRenderPass;    
        VulkanFrameBufferPtrVector m_aFrameBuffer;

        VkFormat m_eVkFormatColor;
        VkFormat m_eVkFormatDepth;
        VkFormat m_eVkFormatSwapChian;
        VkSampleCountFlagBits m_eVkMSAASampleCount;
        

    public:
        F_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }
        F_FORCEINLINE VulkanRenderWindow* GetRenderWindow() const { return m_pRenderWindow; }
        F_FORCEINLINE VulkanRenderPass* GetRenderPass() const { return m_pRenderPass; }
        F_FORCEINLINE const VulkanFrameBufferPtrVector& GetFrameBuffers() const { return m_aFrameBuffer; }
        F_FORCEINLINE VulkanFrameBufferPtrVector& GetFrameBuffers() { return m_aFrameBuffer; }
        F_FORCEINLINE int GetFrameBufferCount() const { return (int)m_aFrameBuffer.size(); }
        F_FORCEINLINE VulkanFrameBuffer* GetFrameBuffer(int index) 
        {
            F_Assert(index >= 0 && index < (int)m_aFrameBuffer.size() && "VulkanRenderFrameBufferDescriptor::GetFrameBuffer") 
            return m_aFrameBuffer[index]; 
        }

        F_FORCEINLINE VkFormat GetVkFormatColor() const { return m_eVkFormatColor; }
        F_FORCEINLINE VkFormat GetVkFormatDepth() const { return m_eVkFormatDepth; }
        F_FORCEINLINE VkFormat GetVkFormatSwapChian() const { return m_eVkFormatSwapChian; }
        F_FORCEINLINE VkSampleCountFlagBits GetVkMSAASampleCount() const { return m_eVkMSAASampleCount; }


    public:
        virtual void Destroy();
        virtual bool Init(FTextureType eTexture,
                          FPixelFormatType ePixelFormatColor,
                          FPixelFormatType ePixelFormatDepth,
                          FPixelFormatType ePixelFormatSwapChian,
                          FMSAASampleCountType eMSAASampleCount,
                          bool bIsUseImGUI);

    public:
    protected:
        void destroyRenderFrameBuffer();
        void destroyRenderPass();
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