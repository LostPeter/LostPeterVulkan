/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-19
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanSwapChain.h"
#include "../include/VulkanInstance.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanQueue.h"
#include "../include/VulkanSemaphore.h"
#include "../include/VulkanConverter.h"

namespace LostPeterPluginRendererVulkan
{
    int32 VulkanSwapChain::ms_nDefaultBackBufferCount = 3;
    VulkanSwapChain::VulkanSwapChain(VulkanDevice* pDevice)
        : m_pDevice(pDevice)
        , m_pWindow(nullptr)
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
        F_Assert(m_pDevice && "VulkanSwapChain::VulkanSwapChain")
    }

    VulkanSwapChain::~VulkanSwapChain()
    {
        Destroy();
    }

    void VulkanSwapChain::Destroy()
    {
        destroySemaphore();
        
        m_pDevice->DestroyVkSwapchainKHR(this->m_vkSwapChainKHR);
        m_vkSwapChainKHR = VK_NULL_HANDLE;

        m_pDevice->GetInstance()->DestroyVkSurfaceKHR(this->m_vkSurfaceKHR);
        m_vkSurfaceKHR = VK_NULL_HANDLE;

        m_nSwapChainImageIndex = -1;
        m_nSemaphoreIndex = 0;
        m_nNumPresentCalls = 0;
        m_nNumAcquireCalls = 0;
        m_nLockToVSync = 0;
        m_nPresentID = 0;
    }
    void VulkanSwapChain::destroySemaphore()
    {
        size_t count = m_aVkImageAcquiredSemaphore.size();
        for (size_t i = 0; i < count; ++i)
        {
            VulkanSemaphore* pSemaphore = m_aVkImageAcquiredSemaphore[i];
            F_DELETE(pSemaphore)
        }
        m_aVkImageAcquiredSemaphore.clear();
    }

    bool VulkanSwapChain::Init(GLFWwindow* pWindow,
                               FPixelFormatType& eOutPixelFormat,
                               uint32 nWidth, 
                               uint32 nHeight, 
                               uint32* pOutDesiredNumSwapChainImages, 
                               VkImageVector& aOutImages, 
                               int32 nLockToVSync)
    {
        F_Assert(pWindow && "VulkanSwapChain::Init")
        m_pWindow = pWindow;
        m_nLockToVSync = nLockToVSync;
        m_nPresentID = 0;

        //1> CreateSurface
        if (!m_pDevice->GetInstance()->CreateVkSurfaceKHR(pWindow, m_vkSurfaceKHR))
        {
            F_LogError("*********************** VulkanSwapChain::Init: 1> CreateVkSurfaceKHR failed !");
            return false;
        }
        F_LogInfo("VulkanSwapChain::Init: 1> CreateVkSurfaceKHR success !");

        //2> CreateQueuePresent
        if (!m_pDevice->CreateQueuePresent(m_vkSurfaceKHR))
        {
            F_LogError("*********************** VulkanSwapChain::Init: 2> CreateQueuePresent failed !");
            return false;
        }
        F_LogInfo("VulkanSwapChain::Init: 2> CreateQueuePresent success !");

        //3> chooseSwapSurfacePixelFormat
        if (!chooseSwapSurfacePixelFormat(eOutPixelFormat))
        {
            F_LogError("*********************** VulkanSwapChain::Init: 3> chooseSwapSurfacePixelFormat failed !");
            return false;
        }
        F_LogInfo("VulkanSwapChain::Init: 3> chooseSwapSurfacePixelFormat success !");
        
        //4> chooseSwapPresentMode
        VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
        if (!chooseSwapPresentMode(presentMode))
        {
            F_LogError("*********************** VulkanSwapChain::Init: 4> chooseSwapPresentMode failed !");
            return false;
        }
        F_LogInfo("VulkanSwapChain::Init: 4> chooseSwapPresentMode success !");

        //5> createSwapChain
        if (!createSwapChain(nWidth, nHeight, presentMode, pOutDesiredNumSwapChainImages, aOutImages))
        {
            F_LogError("*********************** VulkanSwapChain::Init: 5> createSwapChain failed !");
            return false;
        }
        F_LogInfo("VulkanSwapChain::Init: 5> createSwapChain success !");

        //6> createSemaphore
        if (!createSemaphore())
        {
            F_LogError("*********************** VulkanSwapChain::Init: 6> createSemaphore failed !");
            return false;
        }
        F_LogInfo("VulkanSwapChain::Init: 6> createSemaphore success !");


        F_LogInfo("VulkanSwapChain::Init: SwapChain: SwapChain count: [%d], Format: [%d], ColorSpace: [%d], Size: [%dx%d], Present: [%d]", 
            m_vkSwapChainCreateInfoKHR.minImageCount, 
            m_vkSwapChainCreateInfoKHR.imageFormat, 
            m_vkSwapChainCreateInfoKHR.imageColorSpace, 
            m_vkSwapChainCreateInfoKHR.imageExtent.width, 
            m_vkSwapChainCreateInfoKHR.imageExtent.height, 
            m_vkSwapChainCreateInfoKHR.presentMode);
        return true;
    }
    bool VulkanSwapChain::chooseSwapSurfacePixelFormat(FPixelFormatType& eOutPixelFormat)
    {
        //1> chooseSwapSurfacePixelFormat
        uint32 numFormats;
        if (!E_CheckVkResult(vkGetPhysicalDeviceSurfaceFormatsKHR(m_pDevice->GetVkPhysicalDevice(), m_vkSurfaceKHR, &numFormats, nullptr), "vkGetPhysicalDeviceSurfaceFormatsKHR"))
        {
            F_LogError("*********************** VulkanSwapChain::chooseSwapSurfacePixelFormat: vkGetPhysicalDeviceSurfaceFormatsKHR 1 failed !");
            return false;
        }
        std::vector<VkSurfaceFormatKHR> availableFormats(numFormats);
        if (!E_CheckVkResult(vkGetPhysicalDeviceSurfaceFormatsKHR(m_pDevice->GetVkPhysicalDevice(), m_vkSurfaceKHR, &numFormats, availableFormats.data()), "vkGetPhysicalDeviceSurfaceFormatsKHR"))
        {
            F_LogError("*********************** VulkanSwapChain::chooseSwapSurfacePixelFormat: vkGetPhysicalDeviceSurfaceFormatsKHR 2 failed !");
            return false;
        }
        size_t countFormats = availableFormats.size();

        VkSurfaceFormatKHR currFormat = {};
        if (eOutPixelFormat != F_PixelFormat_Unknown)
        {
            bool bFound = false;
            const FPixelFormatDes& des = FPixelFormat::GetPixelFormatDes(eOutPixelFormat);
            if (des.isSupported)
            {
                VkFormat requested = VulkanConverter::Transform2VkFormat(eOutPixelFormat);
                for (size_t i = 0; i < countFormats; ++i)
                {
                    F_LogInfo("VulkanSwapChain::chooseSwapSurfacePixelFormat: Requested PixelFormat: [%s], To Search vkGetPhysicalDeviceSurfaceFormatsKHR: [%d], PixelFormat: [%d], total count: [%d]", des.name.c_str(), (int32)i, (int32)availableFormats[i].format, (int32)countFormats);
                    if (availableFormats[i].format == requested)
                    {
                        bFound = true;
                        currFormat = availableFormats[i];
                        F_LogInfo("VulkanSwapChain::chooseSwapSurfacePixelFormat: Requested PixelFormat: [%s], Search vkGetPhysicalDeviceSurfaceFormatsKHR: [%d], PixelFormat: [%d] found, total count: [%d]", des.name.c_str(), (int32)i, (int32)availableFormats[i].format, (int32)countFormats);
                        break;
                    }
                }

                if (!bFound)
                {
                    F_LogInfo("VulkanSwapChain::chooseSwapSurfacePixelFormat: Requested PixelFormat: [%s] not supported by this SwapChain !", des.name.c_str());
                    eOutPixelFormat = F_PixelFormat_Unknown;
                }
            }
            else
            {
                F_LogInfo("VulkanSwapChain::chooseSwapSurfacePixelFormat: Requested PixelFormat: [%s] not supported by this Vulkan implementation !", des.name.c_str());
                eOutPixelFormat = F_PixelFormat_Unknown;
            }
        }

        if (eOutPixelFormat == F_PixelFormat_Unknown)
        {
            bool bFound = false;
            for (size_t i = 0; i < countFormats; ++i)
            {
                for (int32 j = 1; j < (int32)F_PixelFormat_Count; ++j)
                {
                    FPixelFormatType eType = (FPixelFormatType)j;
                    const FPixelFormatDes& des = FPixelFormat::GetPixelFormatDes(eType);
                    if (des.isSupported)
                    {
                        VkFormat requested = VulkanConverter::Transform2VkFormat(eType);
                        F_LogInfo("VulkanSwapChain::chooseSwapSurfacePixelFormat: To check vulkan support PixelFormat: [%s], VkFormat: [%d], search index: [%d], vkGetPhysicalDeviceSurfaceFormatsKHR : [%d], count: [%d] !", des.name.c_str(), (int32)requested, j, (int32)i, (int)countFormats);
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
                    F_LogInfo("VulkanSwapChain::chooseSwapSurfacePixelFormat: Found vkGetPhysicalDeviceSurfaceFormatsKHR supported PixelFormat: [%s], Search vkGetPhysicalDeviceSurfaceFormatsKHR: [%d], PixelFormat: [%d] found, total count: [%d]", FPixelFormat::GetPixelFormatDes(eOutPixelFormat).name.c_str(), (int32)i, availableFormats[i].format, (int32)countFormats);
                    break;
                }
            }
        }
        
        if (eOutPixelFormat == F_PixelFormat_Unknown)
        {
            F_LogInfo("VulkanSwapChain::chooseSwapSurfacePixelFormat: Can not find a proper PixelFormat for the SwapChain !");
            return false;
        }

        m_vkSwapChainImageColorFormat = currFormat.format;
        m_vkSwapChainImageColorSpaceKHR = currFormat.colorSpace;
        F_LogInfo("VulkanSwapChain::chooseSwapSurfacePixelFormat: SwapChain pixel format: [%d], color space: [%d]", m_vkSwapChainImageColorFormat, m_vkSwapChainImageColorSpaceKHR);
        return true;
    }

    bool VulkanSwapChain::chooseSwapPresentMode(VkPresentModeKHR& presentMode)
    {
        uint32 numFoundPresentModes = 0;
        if (!E_CheckVkResult(vkGetPhysicalDeviceSurfacePresentModesKHR(m_pDevice->GetVkPhysicalDevice(), m_vkSurfaceKHR, &numFoundPresentModes, nullptr), "vkGetPhysicalDeviceSurfacePresentModesKHR"))
        {
            F_LogError("*********************** VulkanSwapChain::chooseSwapPresentMode: vkGetPhysicalDeviceSurfacePresentModesKHR 1 failed !");
            return false;
        }
        std::vector<VkPresentModeKHR> foundPresentModes(numFoundPresentModes);
        if (!E_CheckVkResult(vkGetPhysicalDeviceSurfacePresentModesKHR(m_pDevice->GetVkPhysicalDevice(), m_vkSurfaceKHR, &numFoundPresentModes, foundPresentModes.data()), "vkGetPhysicalDeviceSurfacePresentModesKHR"))
        {
            F_LogError("*********************** VulkanSwapChain::chooseSwapPresentMode: vkGetPhysicalDeviceSurfacePresentModesKHR 2 failed !");
            return false;
        }
        
        bool foundPresentModeMailbox   = false;
        bool foundPresentModeImmediate = false;
        bool foundPresentModeFIFO      = false;
        F_LogInfo("VulkanSwapChain::chooseSwapPresentMode: Found present mode count: [%d]", (int32)numFoundPresentModes);
        for (int32 i = 0; i < (int32)numFoundPresentModes; ++i)
        {
            switch (foundPresentModes[i])
            {
            case VK_PRESENT_MODE_MAILBOX_KHR:
                foundPresentModeMailbox = true;
                F_LogInfo("VulkanSwapChain::chooseSwapPresentMode: VK_PRESENT_MODE_MAILBOX_KHR: [%d]", (int32)VK_PRESENT_MODE_MAILBOX_KHR);
                break;
            case VK_PRESENT_MODE_IMMEDIATE_KHR:
                foundPresentModeImmediate = true;
                F_LogInfo("VulkanSwapChain::chooseSwapPresentMode: VK_PRESENT_MODE_IMMEDIATE_KHR: [%d]", (int32)VK_PRESENT_MODE_IMMEDIATE_KHR);
                break;
            case VK_PRESENT_MODE_FIFO_KHR:
                foundPresentModeFIFO = true;
                F_LogInfo("VulkanSwapChain::chooseSwapPresentMode: VK_PRESENT_MODE_FIFO_KHR: [%d]", (int32)VK_PRESENT_MODE_FIFO_KHR);
                break;
            default:
                F_LogInfo("VulkanSwapChain::chooseSwapPresentMode: VkPresentModeKHR: [%d]", (int32)foundPresentModes[i]);
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
            F_LogInfo("VulkanSwapChain::chooseSwapPresentMode: Can not find desired Present Mode, Using: [%d]", (int32)foundPresentModes[0]);
            presentMode = foundPresentModes[0];
        }
        F_LogInfo("VulkanSwapChain::chooseSwapPresentMode: Selected VkPresentModeKHR mode: [%d]", presentMode);

        return true;
    }
    bool VulkanSwapChain::createSwapChain(uint32 nWidth, 
                                          uint32 nHeight, 
                                          VkPresentModeKHR presentMode,
                                          uint32* pOutDesiredNumSwapChainImages, 
                                          VkImageVector& aOutImages)
    {
        VkSurfaceCapabilitiesKHR surfProperties;
        if (!E_CheckVkResult(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_pDevice->GetVkPhysicalDevice(), m_vkSurfaceKHR, &surfProperties), "vkGetPhysicalDeviceSurfaceCapabilitiesKHR"))
        {
            F_LogError("*********************** VulkanSwapChain::createSwapChain: vkGetPhysicalDeviceSurfaceCapabilitiesKHR failed !");
            return false;
        }
        F_LogInfo("VulkanSwapChain::createSwapChain: Surface minSize: [%d x %d], maxSize: [%d x %d], curSize: [%d x %d]",
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
        
        uint32 desiredNumBuffers = surfProperties.maxImageCount > 0 ? FMath::Clamp(*pOutDesiredNumSwapChainImages, surfProperties.minImageCount, surfProperties.maxImageCount) : *pOutDesiredNumSwapChainImages;
        VkExtent2D ext;
        ext.width = FMath::Clamp(nWidth, surfProperties.minImageExtent.width, surfProperties.maxImageExtent.width);
        ext.height = FMath::Clamp(nHeight, surfProperties.minImageExtent.height, surfProperties.maxImageExtent.height);
        
        E_ZeroStruct(m_vkSwapChainCreateInfoKHR, VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR);
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
            m_vkSwapChainCreateInfoKHR.imageExtent.width = nWidth;
        }
        if (m_vkSwapChainCreateInfoKHR.imageExtent.height == 0) 
        {
            m_vkSwapChainCreateInfoKHR.imageExtent.height = nHeight;
        }

        VkBool32 supportsPresent;
        if (!E_CheckVkResult(vkGetPhysicalDeviceSurfaceSupportKHR(m_pDevice->GetVkPhysicalDevice(), m_pDevice->GetQueuePresent()->GetFamilyIndex(), m_vkSurfaceKHR, &supportsPresent), "vkGetPhysicalDeviceSurfaceSupportKHR"))
        {
            F_LogError("*********************** VulkanSwapChain::createSwapChain: vkGetPhysicalDeviceSurfaceSupportKHR failed !");
            return false;
        }
        if (!supportsPresent) 
        {
            F_LogError("*********************** VulkanSwapChain::createSwapChain: Present queue not support !");
        }

        if (!m_pDevice->CreateVkSwapchainKHR(m_vkSwapChainCreateInfoKHR, m_vkSwapChainKHR))
        {
            F_LogError("*********************** VulkanSwapChain::createSwapChain: CreateVkSwapchainKHR failed !");
            return false;
        }

        uint32 numSwapChainImages;
        if (!m_pDevice->GetVkSwapchainImagesKHR(m_vkSwapChainKHR, numSwapChainImages, nullptr))
        {
            F_LogError("*********************** VulkanSwapChain::createSwapChain: GetVkSwapchainImagesKHR failed to get swap chain image count !");
            return false;
        }
        aOutImages.resize(numSwapChainImages);
        if (!m_pDevice->GetVkSwapchainImagesKHR(m_vkSwapChainKHR, numSwapChainImages, &aOutImages))
        {
            F_LogError("*********************** VulkanSwapChain::createSwapChain: GetVkSwapchainImagesKHR failed to get swap chain images !");
            return false;
        }
        
        *pOutDesiredNumSwapChainImages = numSwapChainImages;
        m_nSwapChainImageCount = (int32)numSwapChainImages;
        m_vkSwapChainImageExtent = m_vkSwapChainCreateInfoKHR.imageExtent;

        return true;
    }
    bool VulkanSwapChain::createSemaphore()
    {
        destroySemaphore();
        for (int32 i = 0; i < m_nSwapChainImageCount; ++i)
        {
            VulkanSemaphore* pSemaphore = new VulkanSemaphore(m_pDevice);
            if (!pSemaphore->Init())
            {
                F_LogError("*********************** VulkanSwapChain::createSemaphore: pSemaphore->Init() failed !");
                return false;
            }
            m_aVkImageAcquiredSemaphore.push_back(pSemaphore);
        }

        return true;
    }

	VulkanSwapStatusType VulkanSwapChain::AcquireImageIndex(VkSemaphore* pOutSemaphore,
                                                            int32& nSwapChainImageIndex)
    {
        uint32 imageIndex = 0;
        const int32 prev = m_nSemaphoreIndex;
        m_nSemaphoreIndex = (m_nSemaphoreIndex + 1) % m_aVkImageAcquiredSemaphore.size();
        VkSemaphore vkSemaphore = m_aVkImageAcquiredSemaphore[m_nSemaphoreIndex]->GetVkSemaphore();
        VulkanSwapStatusType typeSwapStatus = m_pDevice->VkAcquireNextImageKHR(m_vkSwapChainKHR,
                                                                               F_C_MAX_UINT64,
                                                                               vkSemaphore,
                                                                               VK_NULL_HANDLE,
                                                                               &imageIndex);
        if (typeSwapStatus == Vulkan_SwapStatus_OutOfDate ||
            typeSwapStatus == Vulkan_SwapStatus_Lost)
        {
            m_nSemaphoreIndex = prev;
        }
        m_nNumAcquireCalls += 1;
        *pOutSemaphore = m_aVkImageAcquiredSemaphore[m_nSemaphoreIndex]->GetVkSemaphore();
        m_nSwapChainImageIndex = (int32)imageIndex;
        nSwapChainImageIndex = m_nSwapChainImageIndex;

        return typeSwapStatus;
    }

    VulkanSwapStatusType VulkanSwapChain::Present(VkSemaphore* pComplete)
    {
        if (m_nSwapChainImageIndex == -1) 
        {
		    return Vulkan_SwapStatus_Normal;
        }
        m_nPresentID += 1;

        VkSemaphoreVector aWaitSemaphores;
        if (pComplete != nullptr)
        {
            aWaitSemaphores.push_back(*pComplete);
        }

        uint32_t nSwapChainImageIndex = (uint32_t)m_nSwapChainImageIndex;
        VulkanSwapStatusType typeSwapStatus = m_pDevice->VkQueuePresentKHR(m_vkSwapChainKHR,
                                                                           aWaitSemaphores,
                                                                           &nSwapChainImageIndex);
        m_nNumPresentCalls += 1;
        return typeSwapStatus;
    }

}; //LostPeterPluginRendererVulkan