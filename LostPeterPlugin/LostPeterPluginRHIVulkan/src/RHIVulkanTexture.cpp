/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIVulkanTexture.h"
#include "../include/RHIVulkanTextureView.h"
#include "../include/RHIVulkanDevice.h"
#include "../include/RHIVulkanConverter.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanTexture::RHIVulkanTexture(RHIVulkanDevice* pVulkanDevice, const RHITextureCreateInfo& createInfo)
        : RHITexture(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)
        , m_vkImage(VK_NULL_HANDLE)
        , m_vmaAllocation(VK_NULL_HANDLE)
        , m_bIsOwnMemory(true)
        , m_vkImageAspectFlags(VK_IMAGE_ASPECT_COLOR_BIT)
        , m_pVulkanTextureView(nullptr)
        , m_sExtent(createInfo.sExtent)
        , m_nMipLevels(createInfo.nMipLevels)
        , m_eTextureDimension(createInfo.eTextureDimension)
        , m_ePixelFormat(createInfo.ePixelFormat)
        , m_eSampleCount(createInfo.eSampleCount)
        , m_eUsages(createInfo.eUsages)
        , m_eState(createInfo.eState)
        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanTexture::RHIVulkanTexture")

        refreshParam(createInfo);
        createVkImage();
        tansitionToInitState();
    }

    RHIVulkanTexture::RHIVulkanTexture(RHIVulkanDevice* pVulkanDevice, const RHITextureCreateInfo& createInfo, VkImage& vkImage)
        : RHITexture(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)
        , m_vkImage(vkImage)
        , m_vmaAllocation(VK_NULL_HANDLE)
        , m_bIsOwnMemory(false)
        , m_vkImageAspectFlags(VK_IMAGE_ASPECT_COLOR_BIT)
        , m_pVulkanTextureView(nullptr)
        , m_sExtent(createInfo.sExtent)
        , m_nMipLevels(createInfo.nMipLevels)
        , m_eTextureDimension(createInfo.eTextureDimension)
        , m_ePixelFormat(createInfo.ePixelFormat)
        , m_eSampleCount(createInfo.eSampleCount)
        , m_eUsages(createInfo.eUsages)
        , m_eState(createInfo.eState)
        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanTexture::RHIVulkanTexture")

        refreshParam(createInfo);
    }

    RHIVulkanTexture::~RHIVulkanTexture()
    {
        Destroy();
    }

    void RHIVulkanTexture::Destroy()
    {
        DestroyTextureView();
        if (m_vkImage != VK_NULL_HANDLE && m_bIsOwnMemory)
        {
            vmaDestroyImage(m_pVulkanDevice->GetVmaAllocator(), m_vkImage, m_vmaAllocation);
        }
        m_vkImage = VK_NULL_HANDLE;
        m_vmaAllocation = VK_NULL_HANDLE;
    }

    void RHIVulkanTexture::DestroyTextureView()
    {
        F_DELETE(m_pVulkanTextureView)
        m_pTextureView = nullptr;
    }

    RHITextureView* RHIVulkanTexture::CreateTextureView(const RHITextureViewCreateInfo& createInfo)
    {
        if (m_pVulkanTextureView != nullptr)
        {
            return m_pVulkanTextureView;
        }
        m_pVulkanTextureView = new RHIVulkanTextureView(this, createInfo);
        m_pTextureView = m_pVulkanTextureView;
        return m_pVulkanTextureView;
    }

    VkImageView RHIVulkanTexture::GetVkImageView()
    {
        if (m_pVulkanTextureView == nullptr)
            return VK_NULL_HANDLE;
        return m_pVulkanTextureView->GetVkImageView();
    }

    void RHIVulkanTexture::createVkImage()
    {
        VkImageCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.arrayLayers = m_sExtent.z;
        imageInfo.mipLevels = m_nMipLevels;
        imageInfo.extent = RHIVulkanConverter::TransformToVkExtent3D(m_sExtent);
        imageInfo.samples = RHIVulkanConverter::TransformToVkSampleCountFlagBits(m_eSampleCount);
        imageInfo.imageType = RHIVulkanConverter::TransformToVkImageType(m_eTextureDimension);
        imageInfo.format = RHIVulkanConverter::TransformToVkFormat(m_ePixelFormat);
        imageInfo.usage = RHIVulkanConverter::TransformToVkImageUsageFlagsFromTextureUsageFlags(m_eUsages);

        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.usage = VMA_MEMORY_USAGE_AUTO;

        if (vmaCreateImage(m_pVulkanDevice->GetVmaAllocator(), &imageInfo, &allocInfo, &m_vkImage, &m_vmaAllocation, nullptr) == VK_SUCCESS)
        {
            F_LogError("*********************** RHIVulkanTexture::createVkImage: vmaCreateImage failed, name: [%s] !", m_strDebugName.c_str());
            return;
        }

        if (RHI_IsDebug())
        {
            if (!m_strDebugName.empty())
            {
                m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_IMAGE, reinterpret_cast<uint64_t>(m_vkImage), m_strDebugName.c_str());
            }
        }
    }       

    void RHIVulkanTexture::tansitionToInitState()
    {

    }

    void RHIVulkanTexture::refreshParam(const RHITextureCreateInfo& createInfo)
    {
        if (createInfo.eUsages & RHITextureUsageBitsType::RHI_TextureUsageBits_DepthStencilAttachment) 
        {
            m_vkImageAspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
            if (createInfo.ePixelFormat == RHIPixelFormatType::RHI_PixelFormat_D32FloatS8UInt || 
                createInfo.ePixelFormat == RHIPixelFormatType::RHI_PixelFormat_D24UNormS8UInt) 
            {
                m_vkImageAspectFlags |= VK_IMAGE_ASPECT_STENCIL_BIT;
            }
        }
    }
    
}; //LostPeterPluginRHIVulkan