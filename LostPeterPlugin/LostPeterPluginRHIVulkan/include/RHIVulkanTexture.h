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

#ifndef _RHI_VULKAN_TEXTURE_H_
#define _RHI_VULKAN_TEXTURE_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanTexture : public RHITexture
    {   
    public:
        RHIVulkanTexture(RHIVulkanDevice* pVulkanDevice, const RHITextureCreateInfo& createInfo);
        RHIVulkanTexture(RHIVulkanDevice* pVulkanDevice, const RHITextureCreateInfo& createInfo, VkImage& vkImage);
        virtual ~RHIVulkanTexture();

    public:
    protected:
        RHIVulkanDevice* m_pVulkanDevice;
        VkImage m_vkImage;
        VmaAllocation m_vmaAllocation;
        bool m_bIsOwnMemory;
        VkImageAspectFlags m_vkImageAspectFlags;
        RHIVulkanTextureView* m_pVulkanTextureView;

        RHIExtent<3> m_sExtent;
        uint8 m_nMipLevels;
        RHITextureDimensionType m_eTextureDimension;
        RHIPixelFormatType m_ePixelFormat;
        RHISampleCountType m_eSampleCount;
        RHITextureUsageFlags m_eUsages;
        RHITextureStateType m_eState;
      	String m_strDebugName;

    public:
        F_FORCEINLINE RHIVulkanDevice* GetVulkanDevice() { return m_pVulkanDevice; }
        F_FORCEINLINE VkImage& GetVkImage() { return m_vkImage; }
        VkImageView GetVkImageView();
        F_FORCEINLINE VmaAllocation& GetVmaAllocation() { return m_vmaAllocation; }
        F_FORCEINLINE bool IsOwnMemory() const { return m_bIsOwnMemory; }
        F_FORCEINLINE VkImageAspectFlags GetVkImageAspectFlags() { return m_vkImageAspectFlags; }
        F_FORCEINLINE RHIVulkanTextureView* GetVulkanTextureView() { return m_pVulkanTextureView; }

        F_FORCEINLINE const RHIExtent<3>& GetExtent() const { return m_sExtent; }
        F_FORCEINLINE uint8 GetMipLevels() const { return m_nMipLevels; }
        F_FORCEINLINE RHITextureDimensionType GetTextureDimensionType() const { return m_eTextureDimension; }
        F_FORCEINLINE RHIPixelFormatType GetPixelFormatType() const { return m_ePixelFormat; }
        F_FORCEINLINE RHISampleCountType GetSampleCountType() const { return m_eSampleCount; }
        F_FORCEINLINE RHITextureUsageFlags GetTextureUsageFlags() const { return m_eUsages; }
        F_FORCEINLINE RHITextureStateType GetTextureStateType() const { return m_eState; }
        F_FORCEINLINE const String& GetDebugName() { return m_strDebugName; }

    public:
        virtual void Destroy();
        virtual void DestroyTextureView();
        virtual RHITextureView* CreateTextureView(const RHITextureViewCreateInfo& createInfo);

    public:


    protected:
        void createVkImage();
        void tansitionToInitState();

    protected:
        void refreshParam(const RHITextureCreateInfo& createInfo);
    };
    
}; //LostPeterPluginRHIVulkan

#endif