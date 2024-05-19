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

#include "../include/RHIVulkanTextureView.h"
#include "../include/RHIVulkanTexture.h"
#include "../include/RHIVulkanDevice.h"
#include "../include/RHIVulkanConverter.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanTextureView::RHIVulkanTextureView(RHIVulkanTexture* pVulkanTexture, const RHITextureViewCreateInfo& createInfo)
        : RHITextureView(pVulkanTexture, createInfo)
        , m_pVulkanTexture(pVulkanTexture)
        , m_vkImageView(VK_NULL_HANDLE)
        , m_eTextureView(createInfo.eTextureView)
        , m_eTextureViewDimension(createInfo.eTextureViewDimension)
        , m_eTextureAspect(createInfo.eTextureAspect)
        , m_nBaseMipLevel(createInfo.nBaseMipLevel)
        , m_nMipLevelCount(createInfo.nMipLevelCount)
        , m_nBaseArrayLayer(createInfo.nBaseArrayLayer)
        , m_nArrayLayerCount(createInfo.nArrayLayerCount)
    {
        F_Assert(m_pVulkanTexture && "RHIVulkanTextureView::RHIVulkanTextureView")

        createVkImageView();
    }

    RHIVulkanTextureView::~RHIVulkanTextureView()
    {
        Destroy();
    }

    void RHIVulkanTextureView::Destroy()
    {
        if (m_vkImageView != VK_NULL_HANDLE) 
        {
            m_pVulkanTexture->GetVulkanDevice()->DestroyVkImageView(m_vkImageView);
        }
        m_vkImageView = VK_NULL_HANDLE;
    }

    void RHIVulkanTextureView::createVkImageView()
    {
        VkComponentMapping componentMapping = {};
        if (!m_pVulkanTexture->GetVulkanDevice()->CreateVkImageView(m_pVulkanTexture->GetVkImage(),
                                                                    RHIVulkanConverter::TransformToVkImageViewType(m_eTextureViewDimension),
                                                                    RHIVulkanConverter::TransformToVkFormat(m_pVulkanTexture->GetPixelFormatType()),
                                                                    componentMapping,
                                                                    m_pVulkanTexture->GetVkImageAspectFlags(),
                                                                    (uint32_t)m_nBaseMipLevel,
                                                                    (uint32_t)m_nMipLevelCount,
                                                                    (uint32_t)m_nBaseArrayLayer,
                                                                    (uint32_t)m_nArrayLayerCount,
                                                                    m_vkImageView))
        {
            F_LogError("*********************** RHIVulkanTextureView::createVkImageView: CreateVkImageView failed, name: [%s] !", m_pVulkanTexture->GetDebugName().c_str());
            return;
        }
    }
    
}; //LostPeterPluginRHIVulkan