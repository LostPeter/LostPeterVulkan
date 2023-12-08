/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-07
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanTextureManager.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanTexture.h"
#include "../include/VulkanConverter.h"

namespace LostPeter
{
    VulkanTextureManager::VulkanTextureManager(VulkanDevice* pDevice, bool bCanRestrictImageViewUsage)
        : m_pDevice(pDevice)
        , m_bCanRestrictImageViewUsage(bCanRestrictImageViewUsage)
    {
        F_Assert(m_pDevice && "VulkanTextureManager::VulkanTextureManager")
    }

    VulkanTextureManager::~VulkanTextureManager()
    {
        Destroy();
    }

    FPixelFormatType VulkanTextureManager::GetNativeFormat(FTextureType eTexture, FPixelFormatType ePixelFormat, int32 nUsage)
    {
        if (ePixelFormat == F_PixelFormat_BYTE_R8G8B8_SRGB)
            return F_PixelFormat_BYTE_A8R8G8B8_SRGB;
        if (ePixelFormat == F_PixelFormat_BYTE_R8G8B8_UNORM)
            return F_PixelFormat_BYTE_A8R8G8B8_UNORM;
        if (ePixelFormat == F_PixelFormat_BYTE_B8G8R8_SRGB)
            return F_PixelFormat_BYTE_A8B8G8R8_SRGB;
        if (ePixelFormat == F_PixelFormat_BYTE_B8G8R8_UNORM)
            return F_PixelFormat_BYTE_A8B8G8R8_UNORM;

        if (VulkanConverter::Transform2VkFormat(ePixelFormat))
            return ePixelFormat;

        return F_PixelFormat_BYTE_A8R8G8B8_SRGB;
    }

    bool VulkanTextureManager::CheckSupport(FPixelFormatType ePixelFormat, uint32 nTextureFlags) const
    {
        F_Assert(nTextureFlags != E_TextureUsage_NotSRV && "VulkanTextureManager::CheckSupport: Invalid textureFlags combination !")

        const VkFormat vkFormat = VulkanConverter::Transform2VkFormat(ePixelFormat);

        VkFormatProperties props;
        //vkGetPhysicalDeviceFormatProperties(m_pDevice->m_vkPhysicalDevice, vkFormat, &props);

        uint32 features = 0;
        if (!(nTextureFlags & E_TextureUsage_NotSRV))
            features |= VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT;

        if (nTextureFlags & E_TextureUsage_NotSRV)
            features |= VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT;

        if (nTextureFlags & E_TextureUsage_RenderTarget)
        {
            if (FPixelFormat::IsDepth(ePixelFormat))
            {
                features |= VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
            }
            else
            {
                features |= VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT |
                            VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT;
            }
        }

        if (nTextureFlags & E_TextureUsage_AutoMipMap)
        {
            features |= VK_FORMAT_FEATURE_BLIT_SRC_BIT | VK_FORMAT_FEATURE_BLIT_DST_BIT;
            return false;
        }

        if ((props.optimalTilingFeatures & features) == features)
            return true;

        return false;
    }

    Texture* VulkanTextureManager::createImpl(uint32 nGroup, const String& strName, const String2StringMap* pParams /*= nullptr*/)
    {
        return new VulkanTexture(nGroup, strName);
    }

}; //LostPeter