/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-12-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/PreInclude.h"
#include "../include/VulkanSwapChain.h"
#include "../include/VulkanInstance.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanQueue.h"
#include "../include/VulkanPixelFormat.h"
#include "../include/VulkanMath.h"

namespace LostPeter
{
    int32 VulkanSwapChain::ms_nDefaultBackBufferCount = 3;
    VulkanSwapChain::VulkanSwapChain(VulkanDevice* pDevice)
        : m_pDevice(pDevice)
        , m_vkSwapChainKHR(VK_NULL_HANDLE)
        , m_vkSurfaceKHR(VK_NULL_HANDLE)
        , m_vkSwapChainImageColorFormat(VK_FORMAT_B8G8R8A8_SRGB)
        , m_vkSwapChainImageColorSpaceKHR(VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        , m_nSwapChainImageCount(ms_nDefaultBackBufferCount)
        , m_nSwapChainImageIndex(-1)
        , m_nSemaphoreIndex(0)
        , m_nNumPresentCalls(0)
        , m_nNumAcquireCalls(0)
        , m_nLockToVSync(0)
        , m_nPresentID(0)
    {
        assert(m_pDevice && "VulkanSwapChain::VulkanSwapChain");
    }

    VulkanSwapChain::~VulkanSwapChain()
    {
        Destroy();
    }

    void VulkanSwapChain::Destroy()
    {
        size_t count = m_aVkImageAcquiredSemaphore.size();
        for (size_t i = 0; i < count; ++i)
        {
            vkDestroySemaphore(m_pDevice->GetVkDevice(), m_aVkImageAcquiredSemaphore[i], nullptr);
        }
        m_aVkImageAcquiredSemaphore.clear();
        
        if (m_vkSwapChainKHR != VK_NULL_HANDLE)
        {
            vkDestroySwapchainKHR(m_pDevice->GetVkDevice(), m_vkSwapChainKHR, nullptr);
        }
        m_vkSwapChainKHR = VK_NULL_HANDLE;

        if (m_vkSurfaceKHR != VK_NULL_HANDLE)
        {
            vkDestroySurfaceKHR(m_pDevice->GetInstance()->GetVkInstance(), m_vkSurfaceKHR, nullptr);
        }
        m_vkSurfaceKHR = VK_NULL_HANDLE;

        m_nSwapChainImageIndex = -1;
        m_nSemaphoreIndex = 0;
        m_nNumPresentCalls = 0;
        m_nNumAcquireCalls = 0;
        m_nLockToVSync = 0;
        m_nPresentID = 0;
    }

    bool VulkanSwapChain::Init(VulkanPixelFormatType& eOutPixelFormat,
                               uint32 width, 
                               uint32 height, 
                               uint32* pOutDesiredNumSwapChainImages, 
                               VkImageVector& aOutImages, 
                               int8 nLockToVSync)
    {
        m_nLockToVSync = nLockToVSync;
        m_nPresentID = 0;

        //1> CreateSurface
        //VulkanLauncher::GetPlatform()->CreateSurface(m_pDevice->GetInstance()->GetVkInstance(), &m_vkSurfaceKHR);
        if (m_vkSurfaceKHR == VK_NULL_HANDLE)
        {
            Util_LogError("*********************** VulkanSwapChain::Init: 1> VulkanLauncher::GetPlatform()->CreateSurface failed !");
            return false;
        }
        Util_LogInfo("VulkanSwapChain::Init: 1> VulkanLauncher::GetPlatform()->CreateSurface success !");

        //2> CreateQueuePresent
        if (!m_pDevice->CreateQueuePresent(m_vkSurfaceKHR))
        {
            Util_LogError("*********************** VulkanSwapChain::Init: 2> m_pDevice->CreateQueuePresent failed !");
            return false;
        }
        Util_LogInfo("VulkanSwapChain::Init: 2> m_pDevice->CreateQueuePresent success !");

        //3> chooseSwapSurfacePixelFormat
        if (!chooseSwapSurfacePixelFormat(eOutPixelFormat))
        {
            Util_LogError("*********************** VulkanSwapChain::Init: 3> chooseSwapSurfacePixelFormat failed !");
            return false;
        }
        Util_LogInfo("VulkanSwapChain::Init: 3> chooseSwapSurfacePixelFormat success !");
        
        //4> chooseSwapPresentMode
        VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
        if (!chooseSwapPresentMode(presentMode))
        {
            Util_LogError("*********************** VulkanSwapChain::Init: 4> chooseSwapPresentMode failed !");
            return false;
        }
        Util_LogInfo("VulkanSwapChain::Init: 4> chooseSwapPresentMode success !");

        //5> createSwapChain
        if (!createSwapChain(width, height, presentMode, pOutDesiredNumSwapChainImages, aOutImages))
        {
            Util_LogError("*********************** VulkanSwapChain::Init: 5> createSwapChain failed !");
            return false;
        }
        Util_LogInfo("VulkanSwapChain::Init: 5> createSwapChain success !");

        //6> createFence
        if (!createFence())
        {
            Util_LogError("*********************** VulkanSwapChain::Init: 6> createFence failed !");
            return false;
        }
        Util_LogInfo("VulkanSwapChain::Init: 6> createFence success !");


        Util_LogInfo("VulkanSwapChain::Init: SwapChain: SwapChain count: [%d], Format: [%d], ColorSpace: [%d], Size: [%dx%d], Present: [%d]", 
            m_vkSwapChainCreateInfoKHR.minImageCount, 
            m_vkSwapChainCreateInfoKHR.imageFormat, 
            m_vkSwapChainCreateInfoKHR.imageColorSpace, 
            m_vkSwapChainCreateInfoKHR.imageExtent.width, 
            m_vkSwapChainCreateInfoKHR.imageExtent.height, 
            m_vkSwapChainCreateInfoKHR.presentMode);
        return true;
    }

    VulkanSwapStatusType VulkanSwapChain::Present(VulkanQueue* pQueueGraphics, 
                                                  VulkanQueue* pQueuePresent, 
                                                  VkSemaphore* pComplete)
    {
        if (m_nSwapChainImageIndex == -1) 
        {
		    return Vulkan_SwapStatus_Normal;
        }
        m_nPresentID += 1;

        VkPresentInfoKHR createInfo;
        Util_ZeroStruct(createInfo, VK_STRUCTURE_TYPE_PRESENT_INFO_KHR);
        createInfo.waitSemaphoreCount = pComplete == nullptr ? 0 : 1;
        createInfo.pWaitSemaphores = pComplete;
        createInfo.swapchainCount = 1;
        createInfo.pSwapchains = &m_vkSwapChainKHR;
        createInfo.pImageIndices = (uint32*)&m_nSwapChainImageIndex;
        
        VkResult presentResult = vkQueuePresentKHR(pQueuePresent->GetVkQueue(), &createInfo);
        if (presentResult == VK_ERROR_OUT_OF_DATE_KHR) 
        {
            return Vulkan_SwapStatus_OutOfDate;
        }
        else if (presentResult == VK_ERROR_SURFACE_LOST_KHR) 
        {
            return Vulkan_SwapStatus_Lost;
        }
        else if (presentResult != VK_SUCCESS && presentResult != VK_SUBOPTIMAL_KHR) 
        {
            Util_LogError("*********************** VulkanSwapChain::Present: vkQueuePresentKHR: %s", Utile_VkResult2String(presentResult).c_str());
            
            throw std::runtime_error("*********************** VulkanSwapChain::Present: Failed to present swap chain image !");
            return Vulkan_SwapStatus_Error;
        }

        m_nNumPresentCalls += 1;
        return Vulkan_SwapStatus_Normal;
    }

	int32 VulkanSwapChain::AcquireImageIndex(VkSemaphore* pOutSemaphore)
    {
        uint32 imageIndex = 0;
        const int32 prev = m_nSemaphoreIndex;
        m_nSemaphoreIndex = (m_nSemaphoreIndex + 1) % m_aVkImageAcquiredSemaphore.size();
        VkResult result = vkAcquireNextImageKHR(m_pDevice->GetVkDevice(), m_vkSwapChainKHR, C_MAX_UINT64, m_aVkImageAcquiredSemaphore[m_nSemaphoreIndex], VK_NULL_HANDLE, &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) 
        {
            m_nSemaphoreIndex = prev;
            return (int32)Vulkan_SwapStatus_OutOfDate;
        }
        else if (result == VK_ERROR_SURFACE_LOST_KHR) 
        {
            m_nSemaphoreIndex = prev;
            return (int32)Vulkan_SwapStatus_Lost;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            m_nSemaphoreIndex = prev;
            Util_LogError("*********************** VulkanSwapChain::AcquireImageIndex: vkAcquireNextImageKHR: %s", Utile_VkResult2String(result).c_str());
            
            throw std::runtime_error("*********************** VulkanSwapChain::AcquireImageIndex: Failed to acquire swap chain image index !");
            return (int32)Vulkan_SwapStatus_Error;
        }
        m_nNumAcquireCalls += 1;
        *pOutSemaphore = m_aVkImageAcquiredSemaphore[m_nSemaphoreIndex];
        m_nSwapChainImageIndex = (int32)imageIndex;

        return m_nSwapChainImageIndex;
    }

    bool VulkanSwapChain::chooseSwapSurfacePixelFormat(VulkanPixelFormatType& eOutPixelFormat)
    {
        //1> chooseSwapSurfacePixelFormat
        uint32 numFormats;
        if (!Util_CheckVkResult(vkGetPhysicalDeviceSurfaceFormatsKHR(m_pDevice->GetVkPhysicalDevice(), m_vkSurfaceKHR, &numFormats, nullptr), "vkGetPhysicalDeviceSurfaceFormatsKHR"))
        {
            Util_LogError("*********************** VulkanSwapChain::chooseSwapSurfacePixelFormat: vkGetPhysicalDeviceSurfaceFormatsKHR 1 failed !");
            return false;
        }
        std::vector<VkSurfaceFormatKHR> availableFormats(numFormats);
        if (!Util_CheckVkResult(vkGetPhysicalDeviceSurfaceFormatsKHR(m_pDevice->GetVkPhysicalDevice(), m_vkSurfaceKHR, &numFormats, availableFormats.data()), "vkGetPhysicalDeviceSurfaceFormatsKHR"))
        {
            Util_LogError("*********************** VulkanSwapChain::chooseSwapSurfacePixelFormat: vkGetPhysicalDeviceSurfaceFormatsKHR 2 failed !");
            return false;
        }
        size_t countFormats = availableFormats.size();

        VkSurfaceFormatKHR currFormat = {};
        if (eOutPixelFormat != Vulkan_PixelFormat_Unknown)
        {
            bool bFound = false;
            const VulkanPixelFormatDes& des = VulkanPixelFormat::GetPixelFormatDes(eOutPixelFormat);
            if (des.isSupported)
            {
                VkFormat requested = Util_Transform2VkFormat(eOutPixelFormat);
                for (size_t i = 0; i < countFormats; ++i)
                {
                    Util_LogInfo("VulkanSwapChain::chooseSwapSurfacePixelFormat: Requested PixelFormat: [%s], To Search vkGetPhysicalDeviceSurfaceFormatsKHR: [%d], PixelFormat: [%d], total count: [%d]", des.name.c_str(), (int32)i, (int32)availableFormats[i].format, (int32)countFormats);
                    if (availableFormats[i].format == requested)
                    {
                        bFound = true;
                        currFormat = availableFormats[i];
                        Util_LogInfo("VulkanSwapChain::chooseSwapSurfacePixelFormat: Requested PixelFormat: [%s], Search vkGetPhysicalDeviceSurfaceFormatsKHR: [%d], PixelFormat: [%d] found, total count: [%d]", des.name.c_str(), (int32)i, (int32)availableFormats[i].format, (int32)countFormats);
                        break;
                    }
                }

                if (!bFound)
                {
                    Util_LogInfo("VulkanSwapChain::chooseSwapSurfacePixelFormat: Requested PixelFormat: [%s] not supported by this SwapChain !", des.name.c_str());
                    eOutPixelFormat = Vulkan_PixelFormat_Unknown;
                }
            }
            else
            {
                Util_LogInfo("VulkanSwapChain::chooseSwapSurfacePixelFormat: Requested PixelFormat: [%s] not supported by this Vulkan implementation !", des.name.c_str());
                eOutPixelFormat = Vulkan_PixelFormat_Unknown;
            }
        }

        if (eOutPixelFormat == Vulkan_PixelFormat_Unknown)
        {
            bool bFound = false;
            for (size_t i = 0; i < countFormats; ++i)
            {
                for (int32 j = 1; j < (int32)Vulkan_PixelFormat_Count; ++j)
                {
                    VulkanPixelFormatType eType = (VulkanPixelFormatType)j;
                    const VulkanPixelFormatDes& des = VulkanPixelFormat::GetPixelFormatDes(eType);
                    if (des.isSupported)
                    {
                        VkFormat requested = Util_Transform2VkFormat(eType);
                        Util_LogInfo("VulkanSwapChain::chooseSwapSurfacePixelFormat: To check vulkan support PixelFormat: [%s], VkFormat: [%d], search index: [%d], vkGetPhysicalDeviceSurfaceFormatsKHR : [%d], count: [%d] !", des.name.c_str(), (int32)requested, j, (int32)i, (int)countFormats);
                        if (requested == availableFormats[i].format)
                        {
                            bFound = true;
                            eOutPixelFormat = eType;
                            currFormat = availableFormats[i];
                            break;
                        }
                    }
                }
                
                if (bFound)
                {
                    Util_LogInfo("VulkanSwapChain::chooseSwapSurfacePixelFormat: Found vkGetPhysicalDeviceSurfaceFormatsKHR supported PixelFormat: [%s], Search vkGetPhysicalDeviceSurfaceFormatsKHR: [%d], PixelFormat: [%d] found, total count: [%d]", VulkanPixelFormat::GetPixelFormatDes(eOutPixelFormat).name.c_str(), (int32)i, availableFormats[i].format, (int32)countFormats);
                    break;
                }
            }
        }
        
        if (eOutPixelFormat == Vulkan_PixelFormat_Unknown)
        {
            Util_LogInfo("VulkanSwapChain::chooseSwapSurfacePixelFormat: Can not find a proper PixelFormat for the SwapChain !");
            return false;
        }

        m_vkSwapChainImageColorFormat = currFormat.format;
        m_vkSwapChainImageColorSpaceKHR = currFormat.colorSpace;
        Util_LogInfo("VulkanSwapChain::chooseSwapSurfacePixelFormat: SwapChain pixel format: [%d], color space: [%d]", m_vkSwapChainImageColorFormat, m_vkSwapChainImageColorSpaceKHR);
        return true;
    }

    bool VulkanSwapChain::chooseSwapPresentMode(VkPresentModeKHR& presentMode)
    {
        uint32 numFoundPresentModes = 0;
        if (!Util_CheckVkResult(vkGetPhysicalDeviceSurfacePresentModesKHR(m_pDevice->GetVkPhysicalDevice(), m_vkSurfaceKHR, &numFoundPresentModes, nullptr), "vkGetPhysicalDeviceSurfacePresentModesKHR"))
        {
            Util_LogError("*********************** VulkanSwapChain::chooseSwapPresentMode: vkGetPhysicalDeviceSurfacePresentModesKHR 1 failed !");
            return false;
        }
        std::vector<VkPresentModeKHR> foundPresentModes(numFoundPresentModes);
        if (!Util_CheckVkResult(vkGetPhysicalDeviceSurfacePresentModesKHR(m_pDevice->GetVkPhysicalDevice(), m_vkSurfaceKHR, &numFoundPresentModes, foundPresentModes.data()), "vkGetPhysicalDeviceSurfacePresentModesKHR"))
        {
            Util_LogError("*********************** VulkanSwapChain::chooseSwapPresentMode: vkGetPhysicalDeviceSurfacePresentModesKHR 2 failed !");
            return false;
        }
        
        bool foundPresentModeMailbox   = false;
        bool foundPresentModeImmediate = false;
        bool foundPresentModeFIFO      = false;
        Util_LogInfo("VulkanSwapChain::chooseSwapPresentMode: Found present mode count: [%d]", (int32)numFoundPresentModes);
        for (int32 i = 0; i < (int32)numFoundPresentModes; ++i)
        {
            switch (foundPresentModes[i])
            {
            case VK_PRESENT_MODE_MAILBOX_KHR:
                foundPresentModeMailbox = true;
                Util_LogInfo("VulkanSwapChain::chooseSwapPresentMode: VK_PRESENT_MODE_MAILBOX_KHR: [%d]", (int32)VK_PRESENT_MODE_MAILBOX_KHR);
                break;
            case VK_PRESENT_MODE_IMMEDIATE_KHR:
                foundPresentModeImmediate = true;
                Util_LogInfo("VulkanSwapChain::chooseSwapPresentMode: VK_PRESENT_MODE_IMMEDIATE_KHR: [%d]", (int32)VK_PRESENT_MODE_IMMEDIATE_KHR);
                break;
            case VK_PRESENT_MODE_FIFO_KHR:
                foundPresentModeFIFO = true;
                Util_LogInfo("VulkanSwapChain::chooseSwapPresentMode: VK_PRESENT_MODE_FIFO_KHR: [%d]", (int32)VK_PRESENT_MODE_FIFO_KHR);
                break;
            default:
                Util_LogInfo("VulkanSwapChain::chooseSwapPresentMode: VkPresentModeKHR: [%d]", (int32)foundPresentModes[i]);
                break;
            }
        }
    
        if (foundPresentModeImmediate && !m_nLockToVSync) 
        {
            presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
        }
        else if (foundPresentModeMailbox) 
        {
            presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
        }
        else if (foundPresentModeFIFO) 
        {
            presentMode = VK_PRESENT_MODE_FIFO_KHR;
        }
        else
        {
            Util_LogInfo("VulkanSwapChain::chooseSwapPresentMode: Can not find desired Present Mode, Using: [%d]", (int32)foundPresentModes[0]);
            presentMode = foundPresentModes[0];
        }
        Util_LogInfo("VulkanSwapChain::chooseSwapPresentMode: Selected VkPresentModeKHR mode: [%d]", presentMode);

        return true;
    }

    bool VulkanSwapChain::createSwapChain(uint32 width, 
                                          uint32 height, 
                                          VkPresentModeKHR presentMode,
                                          uint32* pOutDesiredNumSwapChainImages, 
                                          VkImageVector& aOutImages)
    {
        VkSurfaceCapabilitiesKHR surfProperties;
        if (!Util_CheckVkResult(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_pDevice->GetVkPhysicalDevice(), m_vkSurfaceKHR, &surfProperties), "vkGetPhysicalDeviceSurfaceCapabilitiesKHR"))
        {
            Util_LogError("*********************** VulkanSwapChain::createSwapChain: vkGetPhysicalDeviceSurfaceCapabilitiesKHR failed !");
            return false;
        }
        Util_LogInfo("VulkanSwapChain::createSwapChain: Surface minSize: [%d x %d], maxSize: [%d x %d], curSize: [%d x %d]",
            (int32)surfProperties.minImageExtent.width, (int32)surfProperties.minImageExtent.height,
            (int32)surfProperties.maxImageExtent.width, (int32)surfProperties.maxImageExtent.height,
            (int32)surfProperties.currentExtent.width, (int32)surfProperties.currentExtent.height);
        
        VkSurfaceTransformFlagBitsKHR preTransform;
        if (surfProperties.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) 
        {
            preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        }
        else 
        {
            preTransform = surfProperties.currentTransform;
        }

        VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
        if (surfProperties.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR) 
        {
            compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        }
        
        uint32 desiredNumBuffers = surfProperties.maxImageCount > 0 ? VulkanMath::Clamp(*pOutDesiredNumSwapChainImages, surfProperties.minImageCount, surfProperties.maxImageCount) : *pOutDesiredNumSwapChainImages;
        VkExtent2D ext;
        ext.width = VulkanMath::Clamp(width, surfProperties.minImageExtent.width, surfProperties.maxImageExtent.width);
        ext.height = VulkanMath::Clamp(height, surfProperties.minImageExtent.height, surfProperties.maxImageExtent.height);
        
        Util_ZeroStruct(m_vkSwapChainCreateInfoKHR, VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR);
        m_vkSwapChainCreateInfoKHR.surface				= m_vkSurfaceKHR;
        m_vkSwapChainCreateInfoKHR.minImageCount		= desiredNumBuffers;
        m_vkSwapChainCreateInfoKHR.imageFormat			= m_vkSwapChainImageColorFormat;
        m_vkSwapChainCreateInfoKHR.imageColorSpace		= m_vkSwapChainImageColorSpaceKHR;
        m_vkSwapChainCreateInfoKHR.imageExtent	        = ext;
        m_vkSwapChainCreateInfoKHR.imageUsage			= VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        m_vkSwapChainCreateInfoKHR.preTransform		    = preTransform;
        m_vkSwapChainCreateInfoKHR.imageArrayLayers	    = 1;
        m_vkSwapChainCreateInfoKHR.imageSharingMode	    = VK_SHARING_MODE_EXCLUSIVE;
        m_vkSwapChainCreateInfoKHR.presentMode			= presentMode;
        m_vkSwapChainCreateInfoKHR.oldSwapchain		    = VK_NULL_HANDLE;
        m_vkSwapChainCreateInfoKHR.clipped				= VK_TRUE;
        m_vkSwapChainCreateInfoKHR.compositeAlpha		= compositeAlpha;
        
        if (m_vkSwapChainCreateInfoKHR.imageExtent.width == 0) 
        {
            m_vkSwapChainCreateInfoKHR.imageExtent.width = width;
        }
        if (m_vkSwapChainCreateInfoKHR.imageExtent.height == 0) 
        {
            m_vkSwapChainCreateInfoKHR.imageExtent.height = height;
        }

        VkBool32 supportsPresent;
        if (!Util_CheckVkResult(vkGetPhysicalDeviceSurfaceSupportKHR(m_pDevice->GetVkPhysicalDevice(), m_pDevice->GetQueuePresent()->GetFamilyIndex(), m_vkSurfaceKHR, &supportsPresent), "vkGetPhysicalDeviceSurfaceSupportKHR"))
        {
            Util_LogError("*********************** VulkanSwapChain::createSwapChain: vkGetPhysicalDeviceSurfaceSupportKHR failed !");
            return false;
        }
        if (!supportsPresent) 
        {
            Util_LogError("*********************** VulkanSwapChain::createSwapChain: Present queue not support !");
        }

        if (!Util_CheckVkResult(vkCreateSwapchainKHR(m_pDevice->GetVkDevice(), &m_vkSwapChainCreateInfoKHR, nullptr, &m_vkSwapChainKHR), "vkCreateSwapchainKHR"))
        {
            Util_LogError("*********************** VulkanSwapChain::createSwapChain: vkCreateSwapchainKHR failed !");
            return false;
        }

        uint32 numSwapChainImages;
        if (!Util_CheckVkResult(vkGetSwapchainImagesKHR(m_pDevice->GetVkDevice(), m_vkSwapChainKHR, &numSwapChainImages, nullptr), "vkGetSwapchainImagesKHR"))
        {
            Util_LogError("*********************** VulkanSwapChain::createSwapChain: vkGetSwapchainImagesKHR failed !");
            return false;
        }
        aOutImages.resize(numSwapChainImages);
        if (!Util_CheckVkResult(vkGetSwapchainImagesKHR(m_pDevice->GetVkDevice(), m_vkSwapChainKHR, &numSwapChainImages, aOutImages.data()), "vkGetSwapchainImagesKHR"))
        {
            Util_LogError("*********************** VulkanSwapChain::createSwapChain: vkGetSwapchainImagesKHR failed !");
            return false;
        }
        
        *pOutDesiredNumSwapChainImages = numSwapChainImages;
        m_nSwapChainImageCount = (int32)numSwapChainImages;
        m_vkSwapChainImageExtent = m_vkSwapChainCreateInfoKHR.imageExtent;

        return true;
    }

    bool VulkanSwapChain::createFence()
    {
        m_aVkImageAcquiredSemaphore.resize(m_nSwapChainImageCount);
        for (int32 i = 0; i < m_nSwapChainImageCount; ++i)
        {
            VkSemaphoreCreateInfo createInfo;
            Util_ZeroStruct(createInfo, VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO);
            if (!Util_CheckVkResult(vkCreateSemaphore(m_pDevice->GetVkDevice(), &createInfo, nullptr, &m_aVkImageAcquiredSemaphore[i]), "vkCreateSemaphore"))
            {
                Util_LogError("*********************** VulkanSwapChain::createFence: vkCreateSemaphore failed !");
                return false;
            }
        }

        return true;
    }

}; //LostPeter