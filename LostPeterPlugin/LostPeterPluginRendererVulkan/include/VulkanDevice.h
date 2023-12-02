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

#ifndef _VULKAN_DEVICE_H_
#define _VULKAN_DEVICE_H_

#include "VulkanPreDefine.h"

namespace LostPeter
{
    class VulkanDevice
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
        VkCommandPool m_vkCommandPoolTransfer;

        VulkanInstance* m_pInstance;
        VulkanQueue* m_pQueueGraphics;
        VulkanQueue* m_pQueueCompute;
        VulkanQueue* m_pQueueTransfer;
        VulkanQueue* m_pQueuePresent;

        VulkanFenceManager* m_pFenceManager;
        VulkanDeviceMemoryManager* m_pDeviceMemoryManager;

        ConstCharPtrVector m_aAppDeviceExtensions;    

    public:
        E_FORCEINLINE const VkDevice& GetVkDevice() const { return m_vkDevice; }
        E_FORCEINLINE const VkPhysicalDevice& GetVkPhysicalDevice() const { return m_vkPhysicalDevice; }
        E_FORCEINLINE const VkPhysicalDeviceProperties& GetVkPhysicalDeviceProperties() const { return m_vkPhysicalDeviceProperties; }
        E_FORCEINLINE const VkPhysicalDeviceLimits& GetVkPhysicalDeviceLimits() const { return m_vkPhysicalDeviceProperties.limits; }
        E_FORCEINLINE const VkPhysicalDeviceFeatures& GetVkPhysicalDeviceFeatures() const { return m_vkPhysicalDeviceFeatures; }
        E_FORCEINLINE VkPhysicalDeviceFeatures2* GetVkPhysicalDeviceFeatures2() const { return m_pVkPhysicalDeviceFeatures2; }
        E_FORCEINLINE void SetVkPhysicalDeviceFeatures2(VkPhysicalDeviceFeatures2* p) { m_pVkPhysicalDeviceFeatures2 = p; }
        E_FORCEINLINE VkSampleCountFlagBits GetVkMaxMSAASamples() const { return m_vkMaxMSAASamples; }

        E_FORCEINLINE VulkanInstance* GetInstance() const { return m_pInstance; }
        E_FORCEINLINE VulkanQueue* GetQueueGraphics() const { return m_pQueueGraphics; }
        E_FORCEINLINE VulkanQueue* GetQueueCompute() const { return m_pQueueCompute; }
        E_FORCEINLINE VulkanQueue* GetQueueTransfer() const { return m_pQueueTransfer; }
        E_FORCEINLINE VulkanQueue* GetQueuePresent() const { return m_pQueuePresent; }

        E_FORCEINLINE VulkanFenceManager* GetFenceManager() const { return m_pFenceManager; }
        E_FORCEINLINE VulkanDeviceMemoryManager* GetDeviceMemoryManager() const { return m_pDeviceMemoryManager; }

        E_FORCEINLINE void AddAppDeviceExtensions(const char* szNameExtension) { m_aAppDeviceExtensions.push_back(szNameExtension); }

    public:
        void Destroy();
        bool Init(int32 deviceIndex, bool bIsEnableValidationLayers);

    public:
        bool QueryGPU(int32 deviceIndex);
        bool CreateQueuePresent(VkSurfaceKHR surface);

        bool IsPixelFormatSupported(VkFormat format);
        bool IsPixelFormatSupported(FPixelFormatType format);

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

        //VkCommandPool
        VkCommandPool CreateVkCommandPool(VkCommandPoolCreateFlags flags,
                                          uint32_t queueFamilyIndex);
        void CreateVkCommandPool(VkCommandPoolCreateFlags flags,
                                 uint32_t queueFamilyIndex, 
                                 VkCommandPool& vkCommandPool);
        
        void DestroyVkCommandPool(const VkCommandPool& vkCommandPool);

        //VkCommandBuffer
        VkCommandBuffer AllocateVkCommandBuffer(const VkCommandPool& vkCommandPool,
                                                VkCommandBufferLevel level);
        void AllocateVkCommandBuffers(const VkCommandPool& vkCommandPool,
                                      VkCommandBufferLevel level,
                                      uint32_t commandBufferCount,
                                      VkCommandBuffer* pCommandBuffers);
        void FreeVkCommandBuffers(const VkCommandPool& vkCommandPool, 
                                  uint32_t commandBufferCount, 
                                  VkCommandBuffer* pCommandBuffer);
        
        void BeginVkCommandBuffer(const VkCommandBuffer& vkCommandBuffer,
                                  VkCommandBufferUsageFlags flags);
        void EndVkCommandBuffer(const VkCommandBuffer& vkCommandBuffer);
        
        //VkQueue
        VkQueue GetVkQueue(uint32 queueFamilyIndex, uint32_t queueIndex);
        void QueueSubmitVkCommandBuffers(const VkQueue& vkQueue,
                                         uint32_t commandBufferCount, 
                                         VkCommandBuffer* pCommandBuffer,
                                         VkFence vkFence);
        void QueueWaitIdle(const VkQueue& vkQueue);

        //VkBuffer
        bool CreateVkBuffer(VkDeviceSize size, 
                            VkBufferUsageFlags usage, 
                            VkMemoryPropertyFlags properties, 
                            VkBuffer& vkBuffer, 
                            VkDeviceMemory& vkBufferMemory);
        bool CreateVkBufferVertex(void* pData, 
                                  uint32_t bufSize, 
                                  VkBuffer& vkBuffer, 
                                  VkDeviceMemory& vkBufferMemory);
        bool CreateVkBufferVertex(void* pData, 
                                  uint32_t bufSize, 
                                  VkBuffer& vkBuffer, 
                                  VkDeviceMemory& vkBufferMemory, 
                                  VkBuffer& vkBufferTransfer, 
                                  VkDeviceMemory& vkBufferMemoryTransfer);
        bool CreateVkBufferIndex(void* pData, 
                                 uint32_t bufSize, 
                                 VkBuffer& vkBuffer, 
                                 VkDeviceMemory& vkBufferMemory);
        bool CreateVkBufferIndex(void* pData, 
                                 uint32_t bufSize, 
                                 VkBuffer& vkBuffer, 
                                 VkDeviceMemory& vkBufferMemory,
                                 VkBuffer& vkBufferTransfer, 
                                 VkDeviceMemory& vkBufferMemoryTransfer);
        bool CreateVkUniformBuffer(VkDeviceSize bufSize, 
                                   VkBuffer& vkBuffer, 
                                   VkDeviceMemory& vkBufferMemory);
        bool CreateVkUniformBuffers(VkDeviceSize bufSize, 
                                    int bufCount,
                                    VkBufferVector& aBuffer, 
                                    VkDeviceMemoryVector& aBufferMemory);
        void CopyVkBuffer(const VkCommandBuffer& vkCommandBuffer, 
                          const VkBuffer& vkBufferSrc, 
                          const VkBuffer& vkBufferDst, 
                          VkDeviceSize size);
        void CopyVkBuffer(const VkBuffer& vkBufferSrc, 
                          const VkBuffer& vkBufferDst, 
                          VkDeviceSize size);
        void WriteVkBuffer(VkDeviceMemory& vkBufferMemory, 
                           void* pData, 
                           uint32_t nDataSize, 
                           uint32_t nDataOffset,
                           VkMemoryMapFlags flags = 0);
        void DestroyVkBuffer(const VkBuffer& vkBuffer, const VkDeviceMemory& vkBufferMemory);
        void DestroyVkBuffers(VkBufferVector& aBuffer, VkDeviceMemoryVector& aBufferMemory);

        void* MapVkDeviceMemory(const VkDeviceMemory& vkBufferMemory,
                                uint32_t nDataSize, 
                                uint32_t nDataOffset,
                                VkMemoryMapFlags flags = 0);
        bool MapVkDeviceMemory(const VkDeviceMemory& vkBufferMemory,
                               uint32_t nDataSize, 
                               uint32_t nDataOffset,
                               VkMemoryMapFlags flags,
                               void** ppData);
        void UnmapVkDeviceMemory(const VkDeviceMemory& vkBufferMemory);

        //VkImage


    /////////////////////////////////////// Vulkan Utility Wrapper ////////////////////////////////////////
    public:
        //Command
        VkCommandBuffer BeginSingleTimeCommands();
        void EndSingleTimeCommands(VkCommandBuffer& vkCommandBuffer);


    };

}; //LostPeter

#endif