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

        if (RHI_IsDebug())
        {
            const String& strName = m_pVulkanTexture->GetName();
            const String& strDebugName = m_pVulkanTexture->GetDebugName();
            String strNameView = strDebugName;
            if (strDebugName.empty())
                strNameView = strName;
            m_pVulkanTexture->GetVulkanDevice()->SetDebugObject(VK_OBJECT_TYPE_IMAGE_VIEW, reinterpret_cast<uint64_t>(m_vkImageView), strNameView.c_str());
            F_LogInfo("RHIVulkanTextureView::createVkImageView: Create VkImageView success, Name: [%s] !", strNameView.c_str());
        }
    }
    
}; //LostPeterPluginRHIVulkan