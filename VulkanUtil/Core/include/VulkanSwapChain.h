// author: LostPeter
// time:   2022-12-03

#ifndef _VULKAN_SWAP_CHAIN_H_
#define _VULKAN_SWAP_CHAIN_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport VulkanSwapChain
    {
    public:
        VulkanSwapChain(VulkanInstance* pInstance);
        ~VulkanSwapChain();

    public:
        static int32 ms_nDefaultBackBufferCount;

    public:
    protected:
        VulkanInstance* m_pInstance;
        VulkanDevice* m_pDevice;

        VkSwapchainKHR m_vkSwapChainKHR;
        VkSurfaceKHR m_vkSurfaceKHR;
        VkSwapchainCreateInfoKHR m_vkSwapChainCreateInfoKHR;
        VkExtent2D m_vkSwapChainImageExtent;
        VkFormat m_vkSwapChainImageColorFormat;
        VkColorSpaceKHR m_vkSwapChainImageColorSpaceKHR;
        int32 m_nSwapChainImageCount;

        std::vector<VkSemaphore> m_aVkImageAcquiredSemaphore;

        int32 m_nSwapChainImageIndex;
        int32 m_nSemaphoreIndex;
        uint32 m_nNumPresentCalls;
        uint32 m_nNumAcquireCalls;
        int8 m_nLockToVSync;
        uint32 m_nPresentID;

    public:
        const VkSwapchainKHR& GetVkSwapChainKHR() const { return m_vkSwapChainKHR; }
        const VkSurfaceKHR& GetVkSurfaceKHR() const { return m_vkSurfaceKHR; }
        const VkSwapchainCreateInfoKHR& GetVkSwapChainCreateInfoKHR() const { return m_vkSwapChainCreateInfoKHR; }
        int32 GetVkSwapChainImageWidth() const { return m_vkSwapChainCreateInfoKHR.imageExtent.width; }
        int32 GetVkSwapChainImageHeight() const { return m_vkSwapChainCreateInfoKHR.imageExtent.height; }
        const VkExtent2D& GetVkSwapChainImageExtent() const { return m_vkSwapChainImageExtent; }
        const VkFormat& GetVkSwapChainImageColorFormat() const { return m_vkSwapChainImageColorFormat; }
        const VkColorSpaceKHR GetVkSwapChainImageColorSpaceKHR() const { return m_vkSwapChainImageColorSpaceKHR; }
        int32 GetVkSwapChainImageCount() const { return m_nSwapChainImageCount; } 
    
        int32 GetSwapChainImageIndex() const { return m_nSwapChainImageIndex; }
        int32 GetSemaphoreIndex() const { return m_nSemaphoreIndex; }
        int8 DoesLockToVSync() { return m_nLockToVSync; }

    public:
        void Destroy();
        bool Init(VulkanPixelFormatType& eOutPixelFormat,
                  uint32 width, 
                  uint32 height, 
                  uint32* pOutDesiredNumSwapChainImages, 
                  std::vector<VkImage>& aOutImages, 
                  int8 nLockToVSync);

    public:
        VulkanSwapStatusType Present(VulkanQueue* pQueueGraphics, 
                                     VulkanQueue* pQueuePresent, 
                                     VkSemaphore* pComplete);

	    int32 AcquireImageIndex(VkSemaphore* pOutSemaphore);

    private:
        bool chooseSwapSurfacePixelFormat(VulkanPixelFormatType& eOutPixelFormat);
        bool chooseSwapPresentMode(VkPresentModeKHR& presentMode);
        bool createSwapChain(uint32 width, 
                             uint32 height, 
                             VkPresentModeKHR presentMode,
                             uint32* pOutDesiredNumSwapChainImages, 
                             std::vector<VkImage>& aOutImages);
        bool createFence();
    };

}; //LostPeter

#endif