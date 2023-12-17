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

namespace LostPeterPluginRendererVulkan
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
        GLFWwindow* m_pWindow;

        VkSwapchainKHR m_vkSwapChainKHR;
        VkSurfaceKHR m_vkSurfaceKHR;
        VkSwapchainCreateInfoKHR m_vkSwapChainCreateInfoKHR;
        VkExtent2D m_vkSwapChainImageExtent;
        VkFormat m_vkSwapChainImageColorFormat;
        VkColorSpaceKHR m_vkSwapChainImageColorSpaceKHR;
        int32 m_nSwapChainImageCount;

        VulkanSemaphorePtrVector m_aVkImageAcquiredSemaphore;

        int32 m_nSwapChainImageIndex;
        int32 m_nSemaphoreIndex;
        uint32 m_nNumPresentCalls;
        uint32 m_nNumAcquireCalls;
        int32 m_nLockToVSync;
        uint32 m_nPresentID;

    public:
        F_FORCEINLINE GLFWwindow* GetWindow() const { return m_pWindow; }
        F_FORCEINLINE VkSwapchainKHR& GetVkSwapChainKHR() { return m_vkSwapChainKHR; }
        F_FORCEINLINE const VkSwapchainKHR& GetVkSwapChainKHR() const { return m_vkSwapChainKHR; }
        F_FORCEINLINE VkSurfaceKHR& GetVkSurfaceKHR() { return m_vkSurfaceKHR; }
        F_FORCEINLINE const VkSurfaceKHR& GetVkSurfaceKHR() const { return m_vkSurfaceKHR; }
        F_FORCEINLINE const VkSwapchainCreateInfoKHR& GetVkSwapChainCreateInfoKHR() const { return m_vkSwapChainCreateInfoKHR; }
        F_FORCEINLINE int32 GetVkSwapChainImageWidth() const { return m_vkSwapChainCreateInfoKHR.imageExtent.width; }
        F_FORCEINLINE int32 GetVkSwapChainImageHeight() const { return m_vkSwapChainCreateInfoKHR.imageExtent.height; }
        F_FORCEINLINE VkExtent2D& GetVkSwapChainImageExtent() { return m_vkSwapChainImageExtent; }
        F_FORCEINLINE const VkExtent2D& GetVkSwapChainImageExtent() const { return m_vkSwapChainImageExtent; }
        F_FORCEINLINE const VkFormat& GetVkSwapChainImageColorFormat() const { return m_vkSwapChainImageColorFormat; }
        F_FORCEINLINE VkColorSpaceKHR& GetVkSwapChainImageColorSpaceKHR() { return m_vkSwapChainImageColorSpaceKHR; }
        F_FORCEINLINE const VkColorSpaceKHR& GetVkSwapChainImageColorSpaceKHR() const { return m_vkSwapChainImageColorSpaceKHR; }
        F_FORCEINLINE int32 GetVkSwapChainImageCount() const { return m_nSwapChainImageCount; } 
    
        F_FORCEINLINE int32 GetSwapChainImageIndex() const { return m_nSwapChainImageIndex; }
        F_FORCEINLINE int32 GetSemaphoreIndex() const { return m_nSemaphoreIndex; }
        F_FORCEINLINE int32 DoesLockToVSync() { return m_nLockToVSync; }

    public:
        void Destroy();
        bool Init(GLFWwindow* pWindow,
                  FPixelFormatType& eOutPixelFormat,
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
        void destroySemaphore();

        bool chooseSwapSurfacePixelFormat(FPixelFormatType& eOutPixelFormat);
        bool chooseSwapPresentMode(VkPresentModeKHR& presentMode);
        bool createSwapChain(uint32 width, 
                             uint32 height, 
                             VkPresentModeKHR presentMode,
                             uint32* pOutDesiredNumSwapChainImages, 
                             VkImageVector& aOutImages);
        bool createSemaphore();
    };

}; //LostPeterPluginRendererVulkan

#endif