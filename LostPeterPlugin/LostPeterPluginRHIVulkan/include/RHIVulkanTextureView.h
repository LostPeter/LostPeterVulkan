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

#ifndef _RHI_VULKAN_TEXTURE_VIEW_H_
#define _RHI_VULKAN_TEXTURE_VIEW_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanTextureView : public RHITextureView
    {
    public:
        RHIVulkanTextureView(RHIVulkanTexture* pVulkanTexture, const RHITextureViewCreateInfo& createInfo);
        virtual ~RHIVulkanTextureView();

    public:
    protected:
        RHIVulkanTexture* m_pVulkanTexture;
        VkImageView m_vkImageView;

        RHITextureViewType m_eTextureView;
        RHITextureViewDimensionType m_eTextureViewDimension;
        RHITextureAspectType m_eTextureAspect;
        uint8 m_nBaseMipLevel;
        uint8 m_nMipLevelCount;
        uint8 m_nBaseArrayLayer;
        uint8 m_nArrayLayerCount;

    public:
        F_FORCEINLINE RHIVulkanTexture* GetVulkanTexture() { return m_pVulkanTexture; }
        F_FORCEINLINE VkImageView& GetVkImageView() { return m_vkImageView; }
        
        F_FORCEINLINE RHITextureViewType GetTextureViewType() const { return m_eTextureView; }
        F_FORCEINLINE RHITextureViewDimensionType GetTextureViewDimensionType() const { return m_eTextureViewDimension; }
        F_FORCEINLINE RHITextureAspectType GetTextureAspectType() const { return m_eTextureAspect; }
        F_FORCEINLINE uint8 GetBaseMipLevel() const { return m_nBaseMipLevel; }
        F_FORCEINLINE uint8 GetMipLevelCount() const { return m_nMipLevelCount; }
        F_FORCEINLINE uint8 GetBaseArrayLayer() const { return m_nBaseArrayLayer; }
        F_FORCEINLINE uint8 GetArrayLayerCount() const { return m_nArrayLayerCount; }


    public:
        virtual void Destroy();

    public:
        

    protected:
        void createVkImageView();
    };

}; //LostPeterPluginRHIVulkan

#endif