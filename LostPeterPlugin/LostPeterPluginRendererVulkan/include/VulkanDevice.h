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

namespace LostPeterPluginRendererVulkan
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
        F_FORCEINLINE const VkDevice& GetVkDevice() const { return m_vkDevice; }
        F_FORCEINLINE const VkPhysicalDevice& GetVkPhysicalDevice() const { return m_vkPhysicalDevice; }
        F_FORCEINLINE const VkPhysicalDeviceProperties& GetVkPhysicalDeviceProperties() const { return m_vkPhysicalDeviceProperties; }
        F_FORCEINLINE const VkPhysicalDeviceLimits& GetVkPhysicalDeviceLimits() const { return m_vkPhysicalDeviceProperties.limits; }
        F_FORCEINLINE const VkPhysicalDeviceFeatures& GetVkPhysicalDeviceFeatures() const { return m_vkPhysicalDeviceFeatures; }
        F_FORCEINLINE VkPhysicalDeviceFeatures2* GetVkPhysicalDeviceFeatures2() const { return m_pVkPhysicalDeviceFeatures2; }
        F_FORCEINLINE void SetVkPhysicalDeviceFeatures2(VkPhysicalDeviceFeatures2* p) { m_pVkPhysicalDeviceFeatures2 = p; }
        F_FORCEINLINE VkSampleCountFlagBits GetVkMaxMSAASamples() const { return m_vkMaxMSAASamples; }

        F_FORCEINLINE VulkanInstance* GetInstance() const { return m_pInstance; }
        F_FORCEINLINE VulkanQueue* GetQueueGraphics() const { return m_pQueueGraphics; }
        F_FORCEINLINE VulkanQueue* GetQueueCompute() const { return m_pQueueCompute; }
        F_FORCEINLINE VulkanQueue* GetQueueTransfer() const { return m_pQueueTransfer; }
        F_FORCEINLINE VulkanQueue* GetQueuePresent() const { return m_pQueuePresent; }

        F_FORCEINLINE VulkanFenceManager* GetFenceManager() const { return m_pFenceManager; }
        F_FORCEINLINE VulkanDeviceMemoryManager* GetDeviceMemoryManager() const { return m_pDeviceMemoryManager; }

        F_FORCEINLINE void AddAppDeviceExtensions(const char* szNameExtension) { m_aAppDeviceExtensions.push_back(szNameExtension); }

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
        ////////// VkSemaphore //////////
        VkSemaphore CreateVkSemaphore();
        void CreateVkSemaphore(VkSemaphore& vkSemaphore);
        void DestroyVkSemaphore(const VkSemaphore& vkSemaphore);
        
        ////////// VkFence //////////////
        VkFence CreateVkFence(bool isCreateSignaled);
        void CreateVkFence(bool isCreateSignaled, VkFence& vkFence);
        void DestroyVkFence(const VkFence& vkFence);

        ////////// VkCommandPool ////////
        VkCommandPool CreateVkCommandPool(VkCommandPoolCreateFlags flags,
                                          uint32_t queueFamilyIndex);
        void CreateVkCommandPool(VkCommandPoolCreateFlags flags,
                                 uint32_t queueFamilyIndex, 
                                 VkCommandPool& vkCommandPool);
        
        void DestroyVkCommandPool(const VkCommandPool& vkCommandPool);

        ////////// VkCommandBuffer //////
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
        
        ////////// VkQueue //////////////
        VkQueue GetVkQueue(uint32 queueFamilyIndex, uint32_t queueIndex);
        void QueueSubmitVkCommandBuffers(const VkQueue& vkQueue,
                                         uint32_t commandBufferCount, 
                                         VkCommandBuffer* pCommandBuffer,
                                         VkFence vkFence);
        void QueueWaitIdle(const VkQueue& vkQueue);

        ////////// VkBuffer /////////////
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
        void CopyVkBuffer(const VkCommandBuffer& vkCommandBuffer, 
                          const VkBuffer& vkBufferSrc, 
                          const VkBuffer& vkBufferDst, 
                          size_t nSrcOffset, 
                          size_t nDstOffset, 
                          VkDeviceSize size);
        void CopyVkBuffer(const VkBuffer& vkBufferSrc, 
                          const VkBuffer& vkBufferDst, 
                          VkDeviceSize size);
        void CopyVkBuffer(const VkBuffer& vkBufferSrc, 
                          const VkBuffer& vkBufferDst, 
                          size_t nSrcOffset, 
                          size_t nDstOffset, 
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

        ////////// VkImage //////////////
        bool CreateVkImage(uint32_t width, 
                           uint32_t height, 
                           uint32_t depth, 
                           uint32_t numArray,
                           uint32_t mipMapCount, 
                           VkImageType type, 
                           bool isCubeMap,
                           VkSampleCountFlagBits numSamples, 
                           VkFormat format, 
                           VkImageTiling tiling, 
                           VkImageUsageFlags usage, 
                           VkSharingMode sharingMode,
                           bool isGraphicsComputeShared,
                           VkMemoryPropertyFlags properties, 
                           VkImage& vkImage, 
                           VkDeviceMemory& vkImageMemory);
        bool CreateVkImageView(VkImage vkImage, 
                               VkImageViewType type, 
                               VkFormat format, 
                               VkImageAspectFlags aspectFlags, 
                               uint32_t mipMapCount,
                               uint32_t numArray,
                               VkImageView& vkImageView);
        bool CreateVkSampler(uint32_t mipMapCount, 
                             VkSampler& vkSampler);
        bool CreateVkSampler(FTextureFilterType eTextureFilter,
                             FTextureAddressingType eTextureAddressing,
                             FTextureBorderColorType eTextureBorderColor,
                             bool enableAnisotropy,
                             float maxAnisotropy,
                             float minLod, 
                             float maxLod, 
                             float mipLodBias,
                             VkSampler& vkSampler);
        void DestroyVkImage(VkImage vkImage, VkDeviceMemory vkImageMemory, VkImageView vkImageView);
        void DestroyVkImageView(VkImageView vkImageView);
        void DestroyVkImageSampler(VkSampler vkSampler);
                        
        void TransitionVkImageLayout(VkCommandBuffer cmdBuffer,
                                     VkImage vkImage, 
                                     VkImageLayout oldLayout, 
                                     VkImageLayout newLayout,
                                     uint32_t mipBase,
                                     uint32_t mipCount,
                                     uint32_t numBase,
                                     uint32_t numArray,
                                     VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT);
        void CopyVkBufferToVkImage(VkCommandBuffer cmdBuffer,
                                   VkBuffer vkBuffer, 
                                   VkImage vkImage, 
                                   uint32_t width, 
                                   uint32_t height,
                                   uint32_t depth,
                                   uint32_t numArray);
        void GenerateVkImageMipMaps(VkCommandBuffer cmdBuffer,
                                    VkImage vkImage, 
                                    VkFormat imageFormat, 
                                    int32_t width, 
                                    int32_t height, 
                                    uint32_t mipMapCount,
                                    uint32_t numArray,
                                    bool autoMipMap);

        bool CreateTexture1D(const String& pathAsset, 
                             uint32_t& mipMapCount,
                             VkImage& vkImage, 
                             VkDeviceMemory& vkImageMemory);

        bool CreateTexture2D(const String& pathAsset, 
                             VkImageType type,
                             VkSampleCountFlagBits numSamples,
                             VkFormat format,
                             bool autoMipMap, 
                             uint32_t& mipMapCount, 
                             VkImage& vkImage, 
                             VkDeviceMemory& vkImageMemory,
                             VkBuffer& vkBuffer, 
                             VkDeviceMemory& vkBufferMemory);
        bool CreateTexture2D(const String& pathAsset, 
                             VkImageType type,
                             VkSampleCountFlagBits numSamples,
                             VkFormat format,
                             bool autoMipMap, 
                             uint32_t& mipMapCount, 
                             VkImage& vkImage, 
                             VkDeviceMemory& vkImageMemory);
        bool CreateTexture2D(const String& pathAsset, 
                             uint32_t& mipMapCount,
                             VkImage& vkImage, 
                             VkDeviceMemory& vkImageMemory);
        
        bool CreateTexture2DArray(const StringVector& aPathAsset, 
                                  VkImageType type,
                                  VkSampleCountFlagBits numSamples,
                                  VkFormat format,
                                  bool autoMipMap, 
                                  uint32_t& mipMapCount, 
                                  VkImage& vkImage, 
                                  VkDeviceMemory& vkImageMemory,
                                  VkBuffer& vkBuffer, 
                                  VkDeviceMemory& vkBufferMemory);
        bool CreateTexture2DArray(const StringVector& aPathAsset, 
                                  VkImageType type,
                                  VkSampleCountFlagBits numSamples,
                                  VkFormat format,
                                  bool autoMipMap, 
                                  uint32_t& mipMapCount, 
                                  VkImage& vkImage, 
                                  VkDeviceMemory& vkImageMemory);
        bool CreateTexture2DArray(const StringVector& aPathAsset, 
                                  uint32_t& mipMapCount,
                                  VkImage& vkImage, 
                                  VkDeviceMemory& vkImageMemory);

        bool CreateTexture3D(VkFormat format,
                             const uint8* pDataRGBA,
                             uint32_t size,
                             uint32_t width,
                             uint32_t height,
                             uint32_t depth,
                             VkImage& vkImage, 
                             VkDeviceMemory& vkImageMemory,
                             VkBuffer& vkBuffer, 
                             VkDeviceMemory& vkBufferMemory);
        bool CreateTexture3D(VkFormat format,
                             const uint8* pDataRGBA,
                             uint32_t size,
                             uint32_t width,
                             uint32_t height,
                             uint32_t depth,
                             VkImage& vkImage, 
                             VkDeviceMemory& vkImageMemory);
        
        bool CreateTextureCubeMap(const StringVector& aPathAsset, 
                                  VkSampleCountFlagBits numSamples,
                                  VkFormat format,
                                  bool autoMipMap, 
                                  uint32_t& mipMapCount, 
                                  VkImage& vkImage, 
                                  VkDeviceMemory& vkImageMemory,
                                  VkBuffer& vkBuffer, 
                                  VkDeviceMemory& vkBufferMemory);
        bool CreateTextureCubeMap(const StringVector& aPathAsset, 
                                  VkSampleCountFlagBits numSamples,
                                  VkFormat format,
                                  bool autoMipMap, 
                                  uint32_t& mipMapCount, 
                                  VkImage& vkImage, 
                                  VkDeviceMemory& vkImageMemory);
        bool CreateTextureCubeMap(const StringVector& aPathAsset,
                                  uint32_t& mipMapCount, 
                                  VkImage& vkImage, 
                                  VkDeviceMemory& vkImageMemory);

        
        bool CreateTextureRenderTarget1D(const FVector4& clDefault,
                                         bool isSetColor,
                                         uint32_t width, 
                                         uint32_t mipMapCount,
                                         VkSampleCountFlagBits numSamples,
                                         VkFormat format,
                                         VkImageUsageFlags usage, 
                                         bool isGraphicsComputeShared,
                                         VkImage& vkImage, 
                                         VkDeviceMemory& vkImageMemory,
                                         VkBuffer& vkBuffer, 
                                         VkDeviceMemory& vkBufferMemory);
        bool CreateTextureRenderTarget1D(const FVector4& clDefault,
                                         bool isSetColor,
                                         uint32_t width, 
                                         uint32_t mipMapCount,
                                         VkSampleCountFlagBits numSamples,
                                         VkFormat format,
                                         VkImageUsageFlags usage, 
                                         bool isGraphicsComputeShared,
                                         VkImage& vkImage, 
                                         VkDeviceMemory& vkImageMemory);
        
        bool CreateTextureRenderTarget2D(const FVector4& clDefault,
                                         bool isSetColor,
                                         uint32_t width, 
                                         uint32_t height,
                                         uint32_t mipMapCount,
                                         VkImageType type,
                                         VkSampleCountFlagBits numSamples,
                                         VkFormat format,
                                         VkImageUsageFlags usage, 
                                         bool isGraphicsComputeShared,
                                         VkImage& vkImage, 
                                         VkDeviceMemory& vkImageMemory,
                                         VkBuffer& vkBuffer, 
                                         VkDeviceMemory& vkBufferMemory);
        bool CreateTextureRenderTarget2D(const FVector4& clDefault,
                                         bool isSetColor,
                                         uint32_t width, 
                                         uint32_t height,
                                         uint32_t mipMapCount,
                                         VkSampleCountFlagBits numSamples,
                                         VkFormat format,
                                         VkImageUsageFlags usage, 
                                         bool isGraphicsComputeShared,
                                         VkImage& vkImage, 
                                         VkDeviceMemory& vkImageMemory);

        bool CreateTextureRenderTarget2D(uint8* pData,
                                         uint32_t width, 
                                         uint32_t height,
                                         uint32_t mipMapCount,
                                         VkImageType type,
                                         VkSampleCountFlagBits numSamples,
                                         VkFormat format,
                                         VkImageUsageFlags usage, 
                                         bool isGraphicsComputeShared,
                                         VkImage& vkImage, 
                                         VkDeviceMemory& vkImageMemory,
                                         VkBuffer& vkBuffer, 
                                         VkDeviceMemory& vkBufferMemory);
        bool CreateTextureRenderTarget2D(uint8* pData,
                                         uint32_t width, 
                                         uint32_t height,
                                         uint32_t mipMapCount,
                                         VkSampleCountFlagBits numSamples,
                                         VkFormat format,
                                         VkImageUsageFlags usage, 
                                         bool isGraphicsComputeShared,
                                         VkImage& vkImage, 
                                         VkDeviceMemory& vkImageMemory);
        
        bool CreateTextureRenderTarget2DArray(const FVector4& clDefault,
                                              bool isSetColor,
                                              uint32_t width, 
                                              uint32_t height,
                                              uint32_t numArray,
                                              uint32_t mipMapCount,
                                              VkImageType type,
                                              VkSampleCountFlagBits numSamples,
                                              VkFormat format,
                                              VkImageUsageFlags usage, 
                                              bool isGraphicsComputeShared,
                                              VkImage& vkImage, 
                                              VkDeviceMemory& vkImageMemory,
                                              VkBuffer& vkBuffer, 
                                              VkDeviceMemory& vkBufferMemory);
        bool CreateTextureRenderTarget2DArray(const FVector4& clDefault,
                                              bool isSetColor,
                                              uint32_t width, 
                                              uint32_t height,
                                              uint32_t numArray,
                                              uint32_t mipMapCount,
                                              VkSampleCountFlagBits numSamples,
                                              VkFormat format,
                                              VkImageUsageFlags usage, 
                                              bool isGraphicsComputeShared,
                                              VkImage& vkImage, 
                                              VkDeviceMemory& vkImageMemory);

        bool CreateTextureRenderTarget3D(const FVector4& clDefault,
                                         bool isSetColor,
                                         uint32_t width, 
                                         uint32_t height,
                                         uint32_t depth,
                                         uint32_t mipMapCount,
                                         VkSampleCountFlagBits numSamples,
                                         VkFormat format,
                                         VkImageUsageFlags usage, 
                                         bool isGraphicsComputeShared,
                                         VkImage& vkImage, 
                                         VkDeviceMemory& vkImageMemory,
                                         VkBuffer& vkBuffer, 
                                         VkDeviceMemory& vkBufferMemory);                                           
        bool CreateTextureRenderTarget3D(const FVector4& clDefault,
                                         bool isSetColor,
                                         uint32_t width, 
                                         uint32_t height,
                                         uint32_t depth,
                                         uint32_t mipMapCount,
                                         VkSampleCountFlagBits numSamples,
                                         VkFormat format,
                                         VkImageUsageFlags usage, 
                                         bool isGraphicsComputeShared,
                                         VkImage& vkImage, 
                                         VkDeviceMemory& vkImageMemory);
        
        bool CreateTextureRenderTargetCubeMap(uint32_t width, 
                                              uint32_t height,
                                              uint32_t mipMapCount,
                                              VkSampleCountFlagBits numSamples,
                                              VkFormat format,
                                              VkImageUsageFlags usage, 
                                              bool isGraphicsComputeShared,
                                              VkImage& vkImage, 
                                              VkDeviceMemory& vkImageMemory,
                                              VkBuffer& vkBuffer, 
                                              VkDeviceMemory& vkBufferMemory);
        bool CreateTextureRenderTargetCubeMap(uint32_t width, 
                                              uint32_t height,
                                              uint32_t mipMapCount,
                                              VkSampleCountFlagBits numSamples,
                                              VkFormat format,
                                              VkImageUsageFlags usage, 
                                              bool isGraphicsComputeShared,
                                              VkImage& vkImage, 
                                              VkDeviceMemory& vkImageMemory);

    
    public:
        ////////// Command //////////////
        VkCommandBuffer BeginSingleTimeCommands();
        void EndSingleTimeCommands(VkCommandBuffer& vkCommandBuffer);

    /////////////////////////////////////// Vulkan Utility Wrapper ////////////////////////////////////////

    };

}; //LostPeterPluginRendererVulkan

#endif