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

#ifndef _VULKAN_FRAME_BUFFER_ATTACHMENT_H_
#define _VULKAN_FRAME_BUFFER_ATTACHMENT_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanFrameBufferAttachment : public Base
    {
    public:
        VulkanFrameBufferAttachment(const String& nameFrameBufferAttachment,
                                    VulkanDevice* pDevice, 
                                    uint32 nFrameBufferFlags);
        virtual ~VulkanFrameBufferAttachment();

    public:
    protected:
        VulkanDevice* m_pDevice;

        uint32 m_nFrameBufferFlags;
        bool m_bIsImageArray;
        VkSampleCountFlagBits m_numSamples;
        VkFormat m_formatSwapChain;
        VkFormat m_formatDepth;

        VkImage m_vkImage;
        VkDeviceMemory m_vkImageMemory;
        VkImageView m_vkImageView;

    public:
        F_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }
        F_FORCEINLINE uint32 GetFrameBuffer() const { return m_nFrameBufferFlags; }
        F_FORCEINLINE bool IsColor() const { return F_HasFrameBufferColor(m_nFrameBufferFlags); }
        F_FORCEINLINE bool IsDepth() const { return F_HasFrameBufferDepth(m_nFrameBufferFlags); }
        F_FORCEINLINE bool IsDepthStencil() const { return F_HasFrameBufferDepthStencil(m_nFrameBufferFlags); }
        F_FORCEINLINE bool IsImageArray() const { return m_bIsImageArray; }
        F_FORCEINLINE VkSampleCountFlagBits GetNumSamples() const { return m_numSamples; }
        F_FORCEINLINE VkFormat GetFormatSwapChain() const { return m_formatSwapChain; }
        F_FORCEINLINE VkFormat GetFormatDepth() const { return m_formatDepth; }

        F_FORCEINLINE const VkImage& GetVkImage() const { return m_vkImage; }
        F_FORCEINLINE VkImage GetVkImage() { return m_vkImage; }
        F_FORCEINLINE const VkDeviceMemory& GetVkImageMemory() const { return m_vkImageMemory; }
        F_FORCEINLINE VkDeviceMemory GetVkImageMemory() { return m_vkImageMemory; }
        F_FORCEINLINE const VkImageView& GetVkImageView() const { return m_vkImageView; }
        F_FORCEINLINE VkImageView GetVkImageView() { return m_vkImageView; }

    public:
        void Destroy();

        virtual bool Init(uint32_t nWidth, 
                          uint32_t nHeight, 
                          bool bIsImageArray,
                          VkSampleCountFlagBits typeSamplesCountFlagBits,
                          VkFormat formatSwapChain,
                          VkFormat formatDepth);
    };

}; //LostPeterPluginRendererVulkan

#endif