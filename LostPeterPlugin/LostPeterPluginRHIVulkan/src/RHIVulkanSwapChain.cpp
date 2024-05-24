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

#include "../include/RHIVulkanSwapChain.h"
#include "../include/RHIVulkanTexture.h"
#include "../include/RHIVulkanDevice.h"
#include "../include/RHIVulkanPhysicalDevice.h"
#include "../include/RHIVulkanSurface.h"
#include "../include/RHIVulkanQueue.h"
#include "../include/RHIVulkanConverter.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanSwapChain::RHIVulkanSwapChain(RHIVulkanDevice* pVulkanDevice, const RHISwapChainCreateInfo& createInfo)
        : RHISwapChain(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)
        , m_vkSwapChainKHR(VK_NULL_HANDLE)
        , m_pVulkanQueuePresent((RHIVulkanQueue*)createInfo.pQueuePresent)
        , m_pVulkanSurface((RHIVulkanSurface*)createInfo.pSurface)
        , m_ePixelFormat(createInfo.ePixelFormat)
        , m_eColorSpace(createInfo.eColorSpace)
        , m_ePresent(createInfo.ePresent)
        , m_sExtent(createInfo.sExtent)
        , m_nSwapChainImageCount(createInfo.nTextureCount)
        , m_nCurrentImageIndex(0)
        , m_strDebugName(createInfo.strDebugName)
        , m_vkImageAvailableSemaphore(VK_NULL_HANDLE)
    {
        F_Assert(m_pVulkanDevice && m_pVulkanQueuePresent && m_pVulkanSurface && "RHIVulkanSwapChain::RHIVulkanSwapChain")

        m_vkExtent2D = RHIVulkanConverter::TransformToVkExtent2D(m_sExtent);
        m_vkSwapChainImageColorFormat = RHIVulkanConverter::TransformToVkFormat(m_ePixelFormat);
        m_vkSwapChainImageColorSpaceKHR = RHIVulkanConverter::TransformToVkColorSpaceKHR(m_eColorSpace);
        chooseSwapPresentMode(m_vkPresentModeKHR);
        createVkSwapchainKHR();
    }

    RHIVulkanSwapChain::~RHIVulkanSwapChain()
    {
        Destroy();
    }

    void RHIVulkanSwapChain::Destroy()
    {
        m_pVulkanDevice->WaitVkDeviceIdle();

        size_t count = m_aVulkanTextures.size();
        for (size_t i = 0; i < count; i++)
        {
            F_DELETE(m_aVulkanTextures[i])
        }
        m_aVulkanTextures.clear();

        if (m_vkImageAvailableSemaphore != VK_NULL_HANDLE)
        {
            m_pVulkanDevice->DestroyVkSemaphore(m_vkImageAvailableSemaphore);
        }
        m_vkImageAvailableSemaphore = VK_NULL_HANDLE;

        if (m_vkSwapChainKHR != VK_NULL_HANDLE)
        {
            m_pVulkanDevice->DestroyVkSwapchainKHR(m_vkSwapChainKHR);
        }
        m_vkSwapChainKHR = VK_NULL_HANDLE;
    }

    RHITexture* RHIVulkanSwapChain::GetTexture(uint8 nIndex)
    {
        F_Assert("RHIVulkanSwapChain::GetTexture" && nIndex < (uint8)m_aVulkanTextures.size());
        return m_aVulkanTextures[nIndex];
    }

    uint8 RHIVulkanSwapChain::AcquireBackTexture()
    {
        RHISwapStatusType eSwapStatus = m_pVulkanDevice->VkAcquireNextImageKHR(m_vkSwapChainKHR,
                                                                               UINT64_MAX,
                                                                               m_vkImageAvailableSemaphore,
                                                                               VK_NULL_HANDLE,
                                                                               &m_nCurrentImageIndex);
        if (eSwapStatus != RHISwapStatusType::RHI_SwapStatus_Normal)
        {
            F_LogError("*********************** RHIVulkanSwapChain::AcquireBackTexture: VkAcquireNextImageKHR failed !");
        }
        return (uint8)m_nCurrentImageIndex;
    }

    void RHIVulkanSwapChain::Present()
    {
        if (m_pVulkanQueuePresent == nullptr)
            return;

        RHISwapStatusType eSwapStatus = m_pVulkanDevice->VkQueuePresentKHR(m_pVulkanQueuePresent->GetVkQueue(),
                                                                           m_vkSwapChainKHR,
                                                                           m_aWaitSemaphores,
                                                                           &m_nCurrentImageIndex);
        if (eSwapStatus != RHISwapStatusType::RHI_SwapStatus_Normal)
        {
            F_LogError("*********************** RHIVulkanSwapChain::Present: VkQueuePresentKHR failed !");
        }
        m_aWaitSemaphores.clear();
    }

    void RHIVulkanSwapChain::AddWaitSemaphore(VkSemaphore vkSemaphore)
    {
        m_aWaitSemaphores.emplace_back(vkSemaphore);
    }

    bool RHIVulkanSwapChain::chooseSwapPresentMode(VkPresentModeKHR& vkPresentModeKHR)
    {
        uint32 numFoundPresentModes = 0;
        if (!RHI_CheckVkResult(vkGetPhysicalDeviceSurfacePresentModesKHR(m_pVulkanDevice->GetVulkanPhysicalDevice()->GetVkPhysicalDevice(), m_pVulkanSurface->GetVulkanSurface(), &numFoundPresentModes, nullptr), "vkGetPhysicalDeviceSurfacePresentModesKHR"))
        {
            F_LogError("*********************** RHIVulkanSwapChain::chooseSwapPresentMode: vkGetPhysicalDeviceSurfacePresentModesKHR 1 failed !");
            return false;
        }
        std::vector<VkPresentModeKHR> foundPresentModes(numFoundPresentModes);
        if (!RHI_CheckVkResult(vkGetPhysicalDeviceSurfacePresentModesKHR(m_pVulkanDevice->GetVulkanPhysicalDevice()->GetVkPhysicalDevice(), m_pVulkanSurface->GetVulkanSurface(), &numFoundPresentModes, foundPresentModes.data()), "vkGetPhysicalDeviceSurfacePresentModesKHR"))
        {
            F_LogError("*********************** RHIVulkanSwapChain::chooseSwapPresentMode: vkGetPhysicalDeviceSurfacePresentModesKHR 2 failed !");
            return false;
        }
        
        bool foundPresentModeMailbox   = false;
        bool foundPresentModeImmediate = false;
        bool foundPresentModeFIFO      = false;
        F_LogInfo("RHIVulkanSwapChain::chooseSwapPresentMode: Found present mode count: [%d]", (int32)numFoundPresentModes);
        for (int32 i = 0; i < (int32)numFoundPresentModes; ++i)
        {
            switch (foundPresentModes[i])
            {
            case VK_PRESENT_MODE_MAILBOX_KHR:
                foundPresentModeMailbox = true;
                F_LogInfo("RHIVulkanSwapChain::chooseSwapPresentMode: VK_PRESENT_MODE_MAILBOX_KHR: [%d]", (int32)VK_PRESENT_MODE_MAILBOX_KHR);
                break;
            case VK_PRESENT_MODE_IMMEDIATE_KHR:
                foundPresentModeImmediate = true;
                F_LogInfo("RHIVulkanSwapChain::chooseSwapPresentMode: VK_PRESENT_MODE_IMMEDIATE_KHR: [%d]", (int32)VK_PRESENT_MODE_IMMEDIATE_KHR);
                break;
            case VK_PRESENT_MODE_FIFO_KHR:
                foundPresentModeFIFO = true;
                F_LogInfo("RHIVulkanSwapChain::chooseSwapPresentMode: VK_PRESENT_MODE_FIFO_KHR: [%d]", (int32)VK_PRESENT_MODE_FIFO_KHR);
                break;
            default:
                F_LogInfo("RHIVulkanSwapChain::chooseSwapPresentMode: VkPresentModeKHR: [%d]", (int32)foundPresentModes[i]);
                break;
            }
        }

        if (foundPresentModeImmediate && m_ePresent != RHIPresentType::RHI_Present_VSync) 
        {
            vkPresentModeKHR = VK_PRESENT_MODE_IMMEDIATE_KHR;
        }
        else if (foundPresentModeMailbox) 
        {
            vkPresentModeKHR = VK_PRESENT_MODE_MAILBOX_KHR;
        }
        else if (foundPresentModeFIFO) 
        {
            vkPresentModeKHR = VK_PRESENT_MODE_FIFO_KHR;
        }
        else
        {
            F_LogInfo("RHIVulkanSwapChain::chooseSwapPresentMode: Can not find desired Present Mode, Using: [%d]", (int32)foundPresentModes[0]);
            vkPresentModeKHR = foundPresentModes[0];
        }
        F_LogInfo("RHIVulkanSwapChain::chooseSwapPresentMode: Selected VkPresentModeKHR mode: [%d]", vkPresentModeKHR);

        return true;
    }

    void RHIVulkanSwapChain::createVkSwapchainKHR()
    {
        VkSurfaceCapabilitiesKHR surfaceProperties;
        if (!m_pVulkanDevice->GetVulkanPhysicalDevice()->GetVkSurfaceCapabilitiesKHR(m_pVulkanSurface->GetVulkanSurface(), surfaceProperties))
        {
            F_LogError("*********************** RHIVulkanSwapChain::createVkSwapchainKHR: GetVkSurfaceCapabilitiesKHR failed, Name: [%s] !", m_strName);
            return;
        }
        m_vkExtent2D.width = FMath::Clamp(m_vkExtent2D.width, surfaceProperties.minImageExtent.width, surfaceProperties.maxImageExtent.width);
        m_vkExtent2D.height = FMath::Clamp(m_vkExtent2D.height, surfaceProperties.minImageExtent.height, surfaceProperties.maxImageExtent.height);
        m_nSwapChainImageCount = surfaceProperties.maxImageCount > 0 ? FMath::Clamp(m_nSwapChainImageCount, surfaceProperties.minImageCount, surfaceProperties.maxImageCount) : m_nSwapChainImageCount;

        if (!m_pVulkanDevice->CheckSwapChainFormatSupport(m_pVulkanSurface, m_ePixelFormat))
        {
            F_LogError("*********************** RHIVulkanSwapChain::createVkSwapchainKHR: CheckSwapChainFormatSupport failed, Name: [%s] !", m_strName);
            return;
        }
        F_LogInfo("RHIVulkanSwapChain::createVkSwapchainKHR: Surface: [%s], minSize: [%u x %u], maxSize: [%u x %u], curSize: [%u x %u], extent: [%u x %u], Count: [%u], PixelFormat: [%s] !",
            m_strName.c_str(),
            surfaceProperties.minImageExtent.width, surfaceProperties.minImageExtent.height,
            surfaceProperties.maxImageExtent.width, surfaceProperties.maxImageExtent.height,
            surfaceProperties.currentExtent.width, surfaceProperties.currentExtent.height,
            m_vkExtent2D.width, m_vkExtent2D.height,
            m_nSwapChainImageCount,
            RHI_GetPixelFormatTypeName(m_ePixelFormat).c_str());

        VkSurfaceTransformFlagBitsKHR preTransform;
        if (surfaceProperties.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) 
        {
            preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        }
        else 
        {
            preTransform = surfaceProperties.currentTransform;
        }

        VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
        if (surfaceProperties.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR) 
        {
            compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        }

        RHI_ZeroStruct(m_vkSwapChainCreateInfoKHR, VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR);
        m_vkSwapChainCreateInfoKHR.surface				= m_pVulkanSurface->GetVulkanSurface();
        m_vkSwapChainCreateInfoKHR.minImageCount		= m_nSwapChainImageCount;
        m_vkSwapChainCreateInfoKHR.imageFormat			= m_vkSwapChainImageColorFormat;
        m_vkSwapChainCreateInfoKHR.imageColorSpace		= m_vkSwapChainImageColorSpaceKHR;
        m_vkSwapChainCreateInfoKHR.imageExtent	        = m_vkExtent2D;
        m_vkSwapChainCreateInfoKHR.imageUsage			= VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        m_vkSwapChainCreateInfoKHR.preTransform		    = preTransform;
        m_vkSwapChainCreateInfoKHR.imageArrayLayers	    = 1;
        m_vkSwapChainCreateInfoKHR.imageSharingMode	    = VK_SHARING_MODE_EXCLUSIVE;
        m_vkSwapChainCreateInfoKHR.presentMode			= m_vkPresentModeKHR;
        m_vkSwapChainCreateInfoKHR.oldSwapchain		    = VK_NULL_HANDLE;
        m_vkSwapChainCreateInfoKHR.clipped				= VK_TRUE;
        m_vkSwapChainCreateInfoKHR.compositeAlpha		= compositeAlpha;
        
        if (m_vkSwapChainCreateInfoKHR.imageExtent.width == 0) 
        {
            m_vkSwapChainCreateInfoKHR.imageExtent.width = m_vkExtent2D.width;
        }
        if (m_vkSwapChainCreateInfoKHR.imageExtent.height == 0) 
        {
            m_vkSwapChainCreateInfoKHR.imageExtent.height = m_vkExtent2D.height;
        }

        VkBool32 supportsPresent;
        if (!RHI_CheckVkResult(vkGetPhysicalDeviceSurfaceSupportKHR(m_pVulkanDevice->GetVulkanPhysicalDevice()->GetVkPhysicalDevice(), m_pVulkanDevice->GetVulkanQueuePresent()->GetFamilyIndex(), m_pVulkanSurface->GetVulkanSurface(), &supportsPresent), "vkGetPhysicalDeviceSurfaceSupportKHR"))
        {
            F_LogError("*********************** RHIVulkanSwapChain::createVkSwapchainKHR: vkGetPhysicalDeviceSurfaceSupportKHR failed, Name: [%s] !", m_strName);
            return;
        }
        if (!supportsPresent) 
        {
            F_LogError("*********************** RHIVulkanSwapChain::createVkSwapchainKHR: Present queue not support, Name: [%s] !", m_strName);
            return;
        }

        if (!m_pVulkanDevice->CreateVkSwapchainKHR(m_vkSwapChainCreateInfoKHR, m_vkSwapChainKHR))
        {
            F_LogError("*********************** RHIVulkanSwapChain::createVkSwapchainKHR: CreateVkSwapchainKHR failed, Name: [%s] !", m_strName);
            return;
        }

        RHITextureCreateInfo textureInfo = {};
        textureInfo.sExtent.x = m_vkExtent2D.width;
        textureInfo.sExtent.y = m_vkExtent2D.height;
        textureInfo.sExtent.z = 1;
        textureInfo.nMipLevels= 1;
        textureInfo.eTextureDimension = RHITextureDimensionType::RHI_TextureDimension_2D;
        textureInfo.ePixelFormat = m_ePixelFormat;
        textureInfo.eSampleCount = RHISampleCountType::RHI_SampleCount_1_Bit;
        textureInfo.eUsages = RHITextureUsageBitsType::RHI_TextureUsageBits_CopyDst | RHITextureUsageBitsType::RHI_TextureUsageBits_RenderAttachment;

        vkGetSwapchainImagesKHR(m_pVulkanDevice->GetVkDevice(), m_vkSwapChainKHR, &m_nSwapChainImageCount, nullptr);
        VkImageVector swapChainImages(m_nSwapChainImageCount);
        vkGetSwapchainImagesKHR(m_pVulkanDevice->GetVkDevice(), m_vkSwapChainKHR, &m_nSwapChainImageCount, swapChainImages.data());
        for (uint32 i = 0; i < m_nSwapChainImageCount; i++) 
        {
            RHIVulkanTexture* pVulkanTexture = (RHIVulkanTexture*)m_pVulkanDevice->CreateTexture(textureInfo, swapChainImages[i]);
            m_aVulkanTextures.push_back(pVulkanTexture);
        }
        m_nSwapChainImageCount = static_cast<uint32>(swapChainImages.size());

        if (!m_pVulkanDevice->CreateVkSemaphore(m_vkImageAvailableSemaphore))
        {
            F_LogError("*********************** RHIVulkanSwapChain::createVkSwapchainKHR: CreateVkSemaphore failed, Name: [%s] !", m_strName);
            return;
        }

        if (RHI_IsDebug())
        {
            if (m_strDebugName.empty())
                m_strDebugName = m_strName;
            m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_SWAPCHAIN_KHR, reinterpret_cast<uint64_t>(m_vkSwapChainKHR), m_strDebugName.c_str());
        }
        F_LogInfo("RHIVulkanSwapChain::createVkSwapchainKHR: Create VkSwapchainKHR success, Name: [%s] !", m_strName);
    }
    
}; //LostPeterPluginRHIVulkan