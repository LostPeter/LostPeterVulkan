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

#ifndef _VULKAN_DEVICE_H_
#define _VULKAN_DEVICE_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport VulkanDevice
    {
    public:
        VulkanDevice(VulkanInstance* pInstance, VkPhysicalDevice vkPhysicalDevice);
        ~VulkanDevice();

    public:
    protected:
        VkDevice m_vkDevice;
        VkPhysicalDevice m_vkPhysicalDevice;
        VkPhysicalDeviceProperties m_vkPhysicalDeviceProperties;
        VkPhysicalDeviceFeatures m_vkPhysicalDeviceFeatures;
        VkPhysicalDeviceFeatures2* m_pVkPhysicalDeviceFeatures2;
        VkQueueFamilyPropertiesVector m_aVkQueueFamilyProperties;
        VkFormat2PropertiesMap m_mapExtensionFormatProperties;
        VkSampleCountFlagBits m_vkMaxMSAASamples;

        VulkanInstance* m_pInstance;
        VulkanQueue* m_pQueueGraphics;
        VulkanQueue* m_pQueueCompute;
        VulkanQueue* m_pQueueTransfer;
        VulkanQueue* m_pQueuePresent;

        VulkanFenceManager* m_pFenceManager;
        VulkanDeviceMemoryManager* m_pDeviceMemoryManager;

        ConstCharPtrVector m_aAppDeviceExtensions;    

    public:
        UTIL_FORCEINLINE const VkDevice& GetVkDevice() const { return m_vkDevice; }
        UTIL_FORCEINLINE const VkPhysicalDevice& GetVkPhysicalDevice() const { return m_vkPhysicalDevice; }
        UTIL_FORCEINLINE const VkPhysicalDeviceProperties& GetVkPhysicalDeviceProperties() const { return m_vkPhysicalDeviceProperties; }
        UTIL_FORCEINLINE const VkPhysicalDeviceLimits& GetVkPhysicalDeviceLimits() const { return m_vkPhysicalDeviceProperties.limits; }
        UTIL_FORCEINLINE const VkPhysicalDeviceFeatures& GetVkPhysicalDeviceFeatures() const { return m_vkPhysicalDeviceFeatures; }
        UTIL_FORCEINLINE VkPhysicalDeviceFeatures2* GetVkPhysicalDeviceFeatures2() const { return m_pVkPhysicalDeviceFeatures2; }
        UTIL_FORCEINLINE void SetVkPhysicalDeviceFeatures2(VkPhysicalDeviceFeatures2* p) { m_pVkPhysicalDeviceFeatures2 = p; }
        UTIL_FORCEINLINE VkSampleCountFlagBits GetVkMaxMSAASamples() const { return m_vkMaxMSAASamples; }

        UTIL_FORCEINLINE VulkanInstance* GetInstance() const { return m_pInstance; }
        UTIL_FORCEINLINE VulkanQueue* GetQueueGraphics() const { return m_pQueueGraphics; }
        UTIL_FORCEINLINE VulkanQueue* GetQueueCompute() const { return m_pQueueCompute; }
        UTIL_FORCEINLINE VulkanQueue* GetQueueTransfer() const { return m_pQueueTransfer; }
        UTIL_FORCEINLINE VulkanQueue* GetQueuePresent() const { return m_pQueuePresent; }

        UTIL_FORCEINLINE VulkanFenceManager* GetFenceManager() const { return m_pFenceManager; }
        UTIL_FORCEINLINE VulkanDeviceMemoryManager* GetDeviceMemoryManager() const { return m_pDeviceMemoryManager; }

        UTIL_FORCEINLINE void AddAppDeviceExtensions(const char* szNameExtension) { m_aAppDeviceExtensions.push_back(szNameExtension); }

    public:
        void Destroy();
        bool Init(int32 deviceIndex, bool bIsEnableValidationLayers);

    public:
        bool QueryGPU(int32 deviceIndex);
        bool CreateQueuePresent(VkSurfaceKHR surface);

        bool IsPixelFormatSupported(VkFormat format);
        bool IsPixelFormatSupported(VulkanPixelFormatType format);

        VkSampleCountFlagBits GetMaxUsableSampleCount();
        VkFormat FindSupportedFormat(const VkFormatVector& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        VkFormat FindDepthFormat();
        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    private:
        bool createDevice(bool bIsEnableValidationLayers);
        bool checkPixelFormats();

        String getDeviceType(const VkPhysicalDeviceProperties& deviceProperties, bool& isDiscrete);
        String getQueueType(const VkQueueFamilyProperties& props);

        void getDeviceLayersAndExtensions(bool bIsEnableValidationLayers,
                                          ConstCharPtrVector& outDeviceLayers, 
                                          ConstCharPtrVector& outDeviceExtensions);

        bool isSupportPresent(VulkanQueue* pQueue, VkSurfaceKHR surface);

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    /////////////////////////////////////// Vulkan Function Wrapper ///////////////////////////////////////
    public:
        //VkSemaphore
        VkSemaphore CreateVkSemaphore();
        void CreateVkSemaphore(VkSemaphore& vkSemaphore);
        void DestroyVkSemaphore(const VkSemaphore& vkSemaphore);
        
        //VkFence
        VkFence CreateVkFence(bool isCreateSignaled);
        void CreateVkFence(bool isCreateSignaled, VkFence& vkFence);
        void DestroyVkFence(const VkFence& vkFence);

        //VkBuffer
        void CreateVkBuffer(VkDeviceSize size, 
                            VkBufferUsageFlags usage, 
                            VkMemoryPropertyFlags properties, 
                            VkBuffer& vkBuffer, 
                            VkDeviceMemory& vkBufferMemory);
        void CopyVkBuffer(const VkCommandBuffer& vkCommandBuffer, const VkBuffer& vkBufferSrc, const VkBuffer& vkBufferDst, VkDeviceSize size);
        void DestroyVkBuffer(const VkBuffer& vkBuffer, const VkDeviceMemory& vkBufferMemory);

    };

}; //LostPeter

#endif