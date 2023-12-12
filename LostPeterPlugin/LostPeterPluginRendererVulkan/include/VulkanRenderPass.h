/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-12
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_RENDER_PASS_H_
#define _VULKAN_RENDER_PASS_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanRenderPass : public Base
    {
    public:
        VulkanRenderPass(const String& nameRenderPass,
                         VulkanDevice* pDevice,
                         bool bIsUseDefault,
                         bool bIsMultiView2);
        virtual ~VulkanRenderPass();

    public:
    protected:
        VulkanDevice* m_pDevice;

        bool m_bIsUseDefault;
        bool m_bIsMultiView2;

        VulkanFrameBufferAttachment* m_pFramebufferAttachmentColor;
        VulkanFrameBufferAttachment* m_pFramebufferAttachmentDepth;
        VkSampler m_vkSampler;
        VkDescriptorImageInfo m_vkDescriptorImageInfo;
        
        VkRenderPass m_vkRenderPass;
        VkFramebuffer m_vkFrameBuffer;

    public:
        F_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }
        F_FORCEINLINE bool IsUseDefault() const { return m_bIsUseDefault; }
        F_FORCEINLINE bool IsMultiView2() const { return m_bIsMultiView2; }

        F_FORCEINLINE VulkanFrameBufferAttachment* GetFramebufferAttachmentColor() const { return m_pFramebufferAttachmentColor; }
        F_FORCEINLINE VulkanFrameBufferAttachment* GetFramebufferAttachmentDepth() const { return m_pFramebufferAttachmentDepth; }

        F_FORCEINLINE VkSampler GetVkSampler() const { return m_vkSampler; }
        F_FORCEINLINE VkDescriptorImageInfo* GetVkDescriptorImageInfo() { return &m_vkDescriptorImageInfo; }
        F_FORCEINLINE VkRenderPass GetVkRenderPass() const { return m_vkRenderPass; }
        F_FORCEINLINE VkFramebuffer GetVkFramebuffer() const { return m_vkFrameBuffer; }

    public:
        void Destroy();

        virtual bool Init(uint32_t width, 
                          uint32_t height, 
                          VkSampleCountFlagBits numSamples,
                          VkFormat formatSwapChain,
                          VkFormat formatDepth,
                          VkRenderPass vkRenderPass);
    
    public:
        void CleanupSwapChain();
        void RecreateSwapChain();
    };

}; //LostPeterPluginRendererVulkan

#endif