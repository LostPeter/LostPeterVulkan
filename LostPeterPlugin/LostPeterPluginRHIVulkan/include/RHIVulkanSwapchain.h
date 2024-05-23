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

#ifndef _RHI_VULKAN_SWAP_CHAIN_H_
#define _RHI_VULKAN_SWAP_CHAIN_H_

#include "RHIVulkanPreDefine.h"
#include "RHIVulkanObject.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanSwapChain : public RHISwapChain
                                             , public RHIVulkanObject
    {   
        friend class RHIVulkanDevice;

    protected:
        RHIVulkanSwapChain(RHIVulkanDevice* pVulkanDevice, const RHISwapChainCreateInfo& createInfo);
    public:
        virtual ~RHIVulkanSwapChain();

    public:
    protected:
        VkSwapchainKHR m_vkSwapChainKHR;
        VkSwapchainCreateInfoKHR m_vkSwapChainCreateInfoKHR;
        VkExtent2D m_vkExtent2D;
        VkFormat m_vkSwapChainImageColorFormat;
        VkColorSpaceKHR m_vkSwapChainImageColorSpaceKHR;
        VkPresentModeKHR m_vkPresentModeKHR;

        RHIVulkanQueue* m_pVulkanQueuePresent;
        RHIVulkanSurface* m_pVulkanSurface;
        RHIPixelFormatType m_ePixelFormat;
        RHIColorSpaceType m_eColorSpace;
        RHIPresentType m_ePresent;
        RHIExtent<2> m_sExtent;
        uint32 m_nSwapChainImageCount;
        uint32 m_nCurrentImageIndex;
        String m_strDebugName;

        RHIVulkanTexturePtrVector m_aVulkanTextures;
        VkSemaphore m_vkImageAvailableSemaphore;
        VkSemaphoreVector m_aWaitSemaphores;

    public:
        F_FORCEINLINE VkSwapchainKHR& GetVkSwapchainKHR() { return m_vkSwapChainKHR; }
        F_FORCEINLINE const VkSwapchainCreateInfoKHR& GetVkSwapchainCreateInfoKHR() const { return m_vkSwapChainCreateInfoKHR; }
        F_FORCEINLINE const VkExtent2D& GetVkExtent2D() const { return m_vkExtent2D; }
        F_FORCEINLINE VkFormat GetVkSwapChainImageColorFormat() const { return m_vkSwapChainImageColorFormat; }
        F_FORCEINLINE VkColorSpaceKHR GetVkSwapChainImageColorSpaceKHR() const { return m_vkSwapChainImageColorSpaceKHR; }
        F_FORCEINLINE VkPresentModeKHR GetVkPresentModeKHR() const { return m_vkPresentModeKHR; }

        F_FORCEINLINE RHIVulkanQueue* GetVulkanQueuePresent() const { return m_pVulkanQueuePresent; }
        F_FORCEINLINE RHIVulkanSurface* GetVulkanSurface() const { return m_pVulkanSurface; }
        F_FORCEINLINE RHIPixelFormatType GetPixelFormatType() const { return m_ePixelFormat; }
        F_FORCEINLINE RHIColorSpaceType GetColorSpaceType() const { return m_eColorSpace; }
        F_FORCEINLINE RHIPresentType GetPresentType() const { return m_ePresent; }
        F_FORCEINLINE const RHIExtent<2>& GetExtent() { return m_sExtent; }
        F_FORCEINLINE uint32 GetSwapChainImageCount() const { return m_nSwapChainImageCount; }
        F_FORCEINLINE uint32 GetCurrentImageIndex() const { return m_nCurrentImageIndex; }
        F_FORCEINLINE const String& GetDebugName() { return m_strDebugName; }

        F_FORCEINLINE const RHIVulkanTexturePtrVector& GetVulkanTextures() const { return m_aVulkanTextures; }
        F_FORCEINLINE RHIVulkanTexturePtrVector& GetVulkanTextures() { return m_aVulkanTextures; }
        F_FORCEINLINE VkSemaphore GetImageSemaphore() const { return m_vkImageAvailableSemaphore; }

    public:
        virtual void Destroy();

        virtual RHITexture* GetTexture(uint8 nIndex);
        virtual uint8 AcquireBackTexture();
        virtual void Present();

    public:
        void AddWaitSemaphore(VkSemaphore vkSemaphore);

    protected:
        bool chooseSwapPresentMode(VkPresentModeKHR& vkPresentModeKHR);
        void createVkSwapchainKHR();
    };
    
}; //LostPeterPluginRHIVulkan

#endif