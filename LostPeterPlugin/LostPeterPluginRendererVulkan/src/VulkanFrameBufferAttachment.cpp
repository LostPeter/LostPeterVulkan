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

#include "../include/VulkanFrameBufferAttachment.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanConverter.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanFrameBufferAttachment::VulkanFrameBufferAttachment(const String& nameFrameBufferAttachment,
                                                             VulkanDevice* pDevice, 
                                                             FFrameBufferType eFrameBuffer)
        : Base(nameFrameBufferAttachment)
        , m_pDevice(pDevice)
        , m_eFrameBuffer(eFrameBuffer)
        , m_bIsImageArray(false)
        , m_vkImage(VK_NULL_HANDLE)
        , m_vkImageMemory(VK_NULL_HANDLE)
        , m_vkImageView(VK_NULL_HANDLE)
    {
        F_Assert(m_pDevice != nullptr && "VulkanFrameBufferAttachment::VulkanFrameBufferAttachment");
    }

    VulkanFrameBufferAttachment::~VulkanFrameBufferAttachment()
    {
        Destroy();
    }
    
    void VulkanFrameBufferAttachment::Destroy()
    {
        if (this->m_vkImage != VK_NULL_HANDLE)
        {
            m_pDevice->DestroyVkImage(this->m_vkImage, this->m_vkImageMemory, this->m_vkImageView);
        }
        this->m_vkImage = VK_NULL_HANDLE;
        this->m_vkImageMemory = VK_NULL_HANDLE;
        this->m_vkImageView = VK_NULL_HANDLE;
    }

    bool VulkanFrameBufferAttachment::Init(uint32_t width, 
                                           uint32_t height, 
                                           bool bIsImageArray,
                                           VkSampleCountFlagBits numSamples,
                                           VkFormat formatSwapChain,
                                           VkFormat formatDepth)
    {
        this->m_bIsImageArray = bIsImageArray;
        this->m_numSamples = numSamples;
        this->m_formatSwapChain = formatSwapChain;
        this->m_formatDepth = formatDepth;

        uint32_t depth = 1;
        uint32_t numArray = 1;
        uint32_t mipMapCount = 1;
        VkImageType imageType = VK_IMAGE_TYPE_2D;
        VkFormat format = formatSwapChain;
        VkComponentMapping componentMapping;
        componentMapping.r = VK_COMPONENT_SWIZZLE_R;
		componentMapping.g = VK_COMPONENT_SWIZZLE_G;
		componentMapping.b = VK_COMPONENT_SWIZZLE_B;
		componentMapping.a = VK_COMPONENT_SWIZZLE_A;
        VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
        VkImageUsageFlags usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        VkImageViewType imageViewType = VK_IMAGE_VIEW_TYPE_2D;
        VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;

        if (IsDepth())
        {
            format = formatDepth;
            componentMapping.g = VK_COMPONENT_SWIZZLE_ZERO;
            componentMapping.b = VK_COMPONENT_SWIZZLE_ZERO;
            componentMapping.a = VK_COMPONENT_SWIZZLE_ZERO;
            usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
        }

        if (bIsImageArray)
        {
            numArray = 2;
            imageViewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        }

        if (!m_pDevice->CreateVkImage(width, 
                                      height, 
                                      depth,
                                      numArray,
                                      mipMapCount,
                                      imageType, 
                                      false,
                                      numSamples, 
                                      format, 
                                      tiling, 
                                      usage,
                                      sharingMode,
                                      false,
                                      properties, 
                                      this->m_vkImage, 
                                      this->m_vkImageMemory))
        {
            F_LogError("*********************** VulkanFrameBufferAttachment::Init: Failed to create VkImage !");
            return false;
        }
        
        if (!m_pDevice->CreateVkImageView(this->m_vkImage, 
                                          imageViewType,
                                          format, 
                                          componentMapping,
                                          aspectFlags, 
                                          mipMapCount,
                                          numArray,
                                          this->m_vkImageView))
        {
            F_LogError("*********************** VulkanFrameBufferAttachment::Init: Failed to create VkImageView !");
            return false;
        }

        return true;
    }

}; //LostPeterPluginRendererVulkan