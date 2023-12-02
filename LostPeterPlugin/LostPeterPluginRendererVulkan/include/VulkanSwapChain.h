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

#ifndef _VULKAN_SWAP_CHAIN_H_
#define _VULKAN_SWAP_CHAIN_H_

#include "VulkanPreDefine.h"

namespace LostPeter
{
    class VulkanSwapChain
    {
    public:
        VulkanSwapChain(VulkanDevice* pDevice);
        ~VulkanSwapChain();

    public:
        static int32 ms_nDefaultBackBufferCount;

    public:
    protected:
        VulkanDevice* m_pDevice;

        VkSwapchainKHR m_vkSwapChainKHR;
        VkSurfaceKHR m_vkSurfaceKHR;
        VkSwapchainCreateInfoKHR m_vkSwapChainCreateInfoKHR;
        VkExtent2D m_vkSwapChainImageExtent;
        VkFormat m_vkSwapChainImageColorFormat;
        VkColorSpaceKHR m_vkSwapChainImageColorSpaceKHR;
        int32 m_nSwapChainImageCount;

        VkSemaphoreVector m_aVkImageAcquiredSemaphore;

        int32 m_nSwapChainImageIndex;
        int32 m_nSemaphoreIndex;
        uint32 m_nNumPresentCalls;
        uint32 m_nNumAcquireCalls;
        int32 m_nLockToVSync;
        uint32 m_nPresentID;

    public:
        E_FORCEINLINE const VkSwapchainKHR& GetVkSwapChainKHR() const { return m_vkSwapChainKHR; }
        E_FORCEINLINE const VkSurfaceKHR& GetVkSurfaceKHR() const { return m_vkSurfaceKHR; }
        E_FORCEINLINE const VkSwapchainCreateInfoKHR& GetVkSwapChainCreateInfoKHR() const { return m_vkSwapChainCreateInfoKHR; }
        E_FORCEINLINE int32 GetVkSwapChainImageWidth() const { return m_vkSwapChainCreateInfoKHR.imageExtent.width; }
        E_FORCEINLINE int32 GetVkSwapChainImageHeight() const { return m_vkSwapChainCreateInfoKHR.imageExtent.height; }
        E_FORCEINLINE const VkExtent2D& GetVkSwapChainImageExtent() const { return m_vkSwapChainImageExtent; }
        E_FORCEINLINE const VkFormat& GetVkSwapChainImageColorFormat() const { return m_vkSwapChainImageColorFormat; }
        E_FORCEINLINE const VkColorSpaceKHR GetVkSwapChainImageColorSpaceKHR() const { return m_vkSwapChainImageColorSpaceKHR; }
        E_FORCEINLINE int32 GetVkSwapChainImageCount() const { return m_nSwapChainImageCount; } 
    
        E_FORCEINLINE int32 GetSwapChainImageIndex() const { return m_nSwapChainImageIndex; }
        E_FORCEINLINE int32 GetSemaphoreIndex() const { return m_nSemaphoreIndex; }
        E_FORCEINLINE int32 DoesLockToVSync() { return m_nLockToVSync; }

    public:
        void Destroy();
        bool Init(FPixelFormatType& eOutPixelFormat,
                  uint32 width, 
                  uint32 height, 
                  uint32* pOutDesiredNumSwapChainImages, 
                  VkImageVector& aOutImages, 
                  int32 nLockToVSync);

    public:
        VulkanSwapStatusType Present(VulkanQueue* pQueueGraphics, 
                                     VulkanQueue* pQueuePresent, 
                                     VkSemaphore* pComplete);

	    int32 AcquireImageIndex(VkSemaphore* pOutSemaphore);

    private:
        bool chooseSwapSurfacePixelFormat(FPixelFormatType& eOutPixelFormat);
        bool chooseSwapPresentMode(VkPresentModeKHR& presentMode);
        bool createSwapChain(uint32 width, 
                             uint32 height, 
                             VkPresentModeKHR presentMode,
                             uint32* pOutDesiredNumSwapChainImages, 
                             VkImageVector& aOutImages);
        bool createFence();
    };

}; //LostPeter

#endif