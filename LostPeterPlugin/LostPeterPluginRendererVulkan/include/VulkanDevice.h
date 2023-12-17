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
        F_FORCEINLINE VkDevice& GetVkDevice() { return m_vkDevice; }
        F_FORCEINLINE const VkDevice& GetVkDevice() const { return m_vkDevice; }
        F_FORCEINLINE VkPhysicalDevice& GetVkPhysicalDevice() { return m_vkPhysicalDevice; }
        F_FORCEINLINE const VkPhysicalDevice& GetVkPhysicalDevice() const { return m_vkPhysicalDevice; }
        F_FORCEINLINE VkPhysicalDeviceProperties* GetVkPhysicalDevicePropertiesPtr() { return &m_vkPhysicalDeviceProperties; }
        F_FORCEINLINE VkPhysicalDeviceProperties& GetVkPhysicalDeviceProperties() { return m_vkPhysicalDeviceProperties; }
        F_FORCEINLINE const VkPhysicalDeviceProperties& GetVkPhysicalDeviceProperties() const { return m_vkPhysicalDeviceProperties; }
        F_FORCEINLINE const VkPhysicalDeviceLimits& GetVkPhysicalDeviceLimits() const { return m_vkPhysicalDeviceProperties.limits; }
        F_FORCEINLINE VkPhysicalDeviceFeatures& GetVkPhysicalDeviceFeatures() { return m_vkPhysicalDeviceFeatures; }
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
        bool CreateQueuePresent(VkSurfaceKHR vkSurfaceKHR);

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

        bool isSupportPresent(VulkanQueue* pQueue, VkSurfaceKHR vkSurfaceKHR);

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    /////////////////////////////////////// Vulkan Function Wrapper ///////////////////////////////////////
    public:
        //////////////////// VkDevice ///////////////////////
        void DestroyVkDevice(const VkDevice& vkDevice);

        //////////////////// VkCommandPool //////////////////
        VkCommandPool CreateVkCommandPool(VkCommandPoolCreateFlags flags,
                                          uint32_t queueFamilyIndex);
        bool CreateVkCommandPool(VkCommandPoolCreateFlags flags,
                                 uint32_t queueFamilyIndex, 
                                 VkCommandPool& vkCommandPool);
        
        void DestroyVkCommandPool(const VkCommandPool& vkCommandPool);

        //////////////////// VkCommandBuffer ////////////////
        VkCommandBuffer AllocateVkCommandBuffer(const VkCommandPool& vkCommandPool,
                                                VkCommandBufferLevel level);
        bool AllocateVkCommandBuffers(const VkCommandPool& vkCommandPool,
                                      VkCommandBufferLevel level,
                                      uint32_t commandBufferCount,
                                      VkCommandBuffer* pCommandBuffers);
        void FreeVkCommandBuffers(const VkCommandPool& vkCommandPool, 
                                  uint32_t commandBufferCount, 
                                  VkCommandBuffer* pCommandBuffer);
        
        bool BeginVkCommandBuffer(const VkCommandBuffer& vkCommandBuffer,
                                  VkCommandBufferUsageFlags flags);
        bool EndVkCommandBuffer(const VkCommandBuffer& vkCommandBuffer);
        
        //////////////////// VkQueue ////////////////////////
        VkQueue GetVkQueue(uint32 queueFamilyIndex, uint32_t queueIndex);
        bool QueueSubmitVkCommandBuffers(const VkQueue& vkQueue,
                                         uint32_t commandBufferCount, 
                                         VkCommandBuffer* pCommandBuffer,
                                         VkFence vkFence);
        bool QueueWaitIdle(const VkQueue& vkQueue);

        //////////////////// VkSemaphore ////////////////////
        VkSemaphore CreateVkSemaphore();
        bool CreateVkSemaphore(VkSemaphore& vkSemaphore);
        void DestroyVkSemaphore(const VkSemaphore& vkSemaphore);
        
        //////////////////// VkFence ////////////////////////
        VkFence CreateVkFence(bool isCreateSignaled);
        bool CreateVkFence(bool isCreateSignaled, VkFence& vkFence);
        void DestroyVkFence(const VkFence& vkFence);

        //////////////////// VkDescriptorPool ///////////////
        bool CreateVkDescriptorPool(uint32_t descriptorCount,    
                                    VkDescriptorPool& vkDescriptorPool);
        void DestroyVkDescriptorPool(const VkDescriptorPool& vkDescriptorPool);

        //////////////////// VkSwapchainKHR /////////////////
        bool CreateVkSwapchainKHR(VkSwapchainCreateInfoKHR& vkSwapChainCreateInfoKHR,
                                  VkSwapchainKHR& vkSwapChainKHR);
        bool GetVkSwapchainImagesKHR(const VkSwapchainKHR& vkSwapChainKHR,
                                     uint32& numSwapChainImages,
                                     VkImageVector* pVkImages);
        void DestroyVkSwapchainKHR(const VkSwapchainKHR& vkSwapChainKHR);

        //////////////////// VkViewport /////////////////////
        void CreateVkViewport(float width,
                              float height,
                              float offsetX,
                              float offsetY,
                              float minDepth,
                              float maxDepth,
                              VkViewport& vkViewport, 
                              VkRect2D& vkScissor);

        //////////////////// VkAttachmentDescription ////////
        void CreateVkAttachmentDescription(VkAttachmentDescriptionFlags flags,
                                           VkFormat format,
                                           VkSampleCountFlagBits samples,
                                           VkAttachmentLoadOp loadOp,
                                           VkAttachmentStoreOp storeOp,
                                           VkAttachmentLoadOp stencilLoadOp,
                                           VkAttachmentStoreOp stencilStoreOp,
                                           VkImageLayout initialLayout,
                                           VkImageLayout finalLayout,
                                           VkAttachmentDescription& vkAttachmentDescription);

        //////////////////// VkRenderPass ///////////////////
        bool CreateVkRenderPass(const String& nameRenderPass,
                                const VkAttachmentDescriptionVector& aAttachmentDescription,
                                const VkSubpassDescriptionVector& aSubpassDescription,
                                const VkSubpassDependencyVector& aSubpassDependency,
                                VkRenderPassMultiviewCreateInfo* pMultiviewCI,
                                VkRenderPass& vkRenderPass);
        void DestroyVkRenderPass(const VkRenderPass& vkRenderPass);

        bool CreateRenderPass_KhrDepth(const String& nameRenderPass,
                                       VkFormat formatSwapChain, 
                                       VkFormat formatDepth, 
                                       VkRenderPass& vkRenderPass);
        bool CreateRenderPass_KhrDepthImgui(const String& nameRenderPass,
                                            VkFormat formatColor, 
                                            VkFormat formatDepth, 
                                            VkFormat formatSwapChain, 
                                            VkRenderPass& vkRenderPass);
        bool CreateRenderPass_ColorDepthMSAA(const String& nameRenderPass,
                                             VkFormat formatColor, 
                                             VkFormat formatDepth, 
                                             VkFormat formatSwapChain, 
                                             VkSampleCountFlagBits samples, 
                                             VkRenderPass& vkRenderPass);
        bool CreateRenderPass_ColorDepthImguiMSAA(const String& nameRenderPass,
                                                  VkFormat formatColor,     
                                                  VkFormat formatDepth, 
                                                  VkFormat formatSwapChain, 
                                                  VkSampleCountFlagBits samples, 
                                                  VkRenderPass& vkRenderPass);

        //////////////////// VkFramebuffer //////////////////
        bool CreateVkFramebuffer(const String& nameFramebuffer,
                                 const VkImageViewVector& aImageView, 
                                 VkRenderPass& vkRenderPass,
                                 VkFramebufferCreateFlags flags,
                                 uint32_t width,
                                 uint32_t height,
                                 uint32_t layers,
                                 VkFramebuffer& vkFramebuffer);
        void DestroyVkFramebuffer(const VkFramebuffer& vkFramebuffer);

        //////////////////// VkBuffer ///////////////////////
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

        //////////////////// VkImage ////////////////////////
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
                               VkComponentMapping components,
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
        void DestroyVkImage(const VkImage& vkImage, const VkDeviceMemory& vkImageMemory, const VkImageView& vkImageView);
        void DestroyVkImageView(const VkImageView& vkImageView);
        void DestroyVkSampler(const VkSampler& vkSampler);
                        
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

        //////////////////// VkShaderModule /////////////////
        bool CreateVkShaderModule(FShaderType typeShader, 
                                  const String& pathFile,
                                  VkShaderModule& vkShaderModule);
        bool CreateVkShaderModule(const String& strTypeShader, 
                                  const String& pathFile,
                                  VkShaderModule& vkShaderModule);
        void DestroyVkShaderModule(const VkShaderModule& vkShaderModule);

        //////////////////// VkDescriptorSetLayout //////////
        bool CreateVkDescriptorSetLayout(const VkDescriptorSetLayoutBindingVector& aDescriptorSetLayoutBinding, 
                                         VkDescriptorSetLayout& vkDescriptorSetLayout);
        void DestroyVkDescriptorSetLayout(const VkDescriptorSetLayout& vkDescriptorSetLayout);

        //////////////////// VkPipelineLayout ///////////////
        bool CreateVkPipelineLayout(const VkDescriptorSetLayoutVector& aDescriptorSetLayout,
                                    VkPipelineLayout& vkPipelineLayout);
        void DestroyVkPipelineLayout(const VkPipelineLayout& vkPipelineLayout);

        //////////////////// VkPipelineCache ////////////////
        bool CreateVkPipelineCache(VkPipelineCache& vkPipelineCache);
        void DestroyVkPipelineCache(const VkPipelineCache& vkPipelineCache);

        //////////////////// VkPipeline /////////////////////
        bool CreateVkPipeline_Graphics(VkShaderModule vertShaderModule, const String& vertMain,
                                       VkShaderModule fragShaderModule, const String& fragMain,
                                       VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                       VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                       VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors,
                                       VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, float lineWidth,
                                       VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                       VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                       VkBool32 bBlend, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
                                       VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
                                       VkColorComponentFlags colorWriteMask,
                                       VkSampleCountFlagBits msaaSamples,
                                       VkPipelineCache vkPipelineCache,
                                       VkPipeline& vkPipeline);
        bool CreateVkPipeline_Graphics(VkShaderModule vertShaderModule, const String& vertMain,
                                       VkShaderModule tescShaderModule, const String& tescMain,
                                       VkShaderModule teseShaderModule, const String& teseMain,
                                       VkShaderModule fragShaderModule, const String& fragMain,
                                       VkPipelineTessellationStateCreateFlags tessellationFlags, uint32_t tessellationPatchControlPoints,
                                       VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                       VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                       VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors,
                                       VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, float lineWidth,
                                       VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                       VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                       VkBool32 bBlend, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
                                       VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
                                       VkColorComponentFlags colorWriteMask,
                                       VkSampleCountFlagBits msaaSamples,
                                       VkPipelineCache vkPipelineCache,
                                       VkPipeline& vkPipeline);
        bool CreateVkPipeline_Graphics(const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos,
                                       bool tessellationIsUsed, VkPipelineTessellationStateCreateFlags tessellationFlags, uint32_t tessellationPatchControlPoints,
                                       VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                       VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                       VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors,
                                       VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, float lineWidth,
                                       VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                       VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                       VkBool32 bBlend, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
                                       VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
                                       VkColorComponentFlags colorWriteMask,
                                       VkSampleCountFlagBits msaaSamples,
                                       VkPipelineCache vkPipelineCache,
                                       VkPipeline& vkPipeline);
        bool CreateVkPipeline_Compute(VkShaderModule compShaderModule,
                                      const String& compMain,
                                      VkPipelineLayout pipelineLayout, 
                                      VkPipelineCreateFlags flags,
                                      VkPipelineCache vkPipelineCache,
                                      VkPipeline& vkPipeline);
        bool CreateVkPipeline_Compute(const VkPipelineShaderStageCreateInfo& shaderStageCreateInfo,
                                      VkPipelineLayout pipelineLayout, 
                                      VkPipelineCreateFlags flags,
                                      VkPipelineCache vkPipelineCache,
                                      VkPipeline& vkPipeline);
        void DestroyVkPipeline(const VkPipeline& vkPipeline);

        //////////////////// VkDescriptorSet ////////////////
        bool CreateVkDescriptorSet(uint32_t descriptorSetCount,
                                   VkDescriptorSetLayout vkDescriptorSetLayout, 
                                   VkDescriptorPool vkDescriptorPool,
                                   VkDescriptorSet& vkDescriptorSet);
        bool CreateVkDescriptorSets(uint32_t countSwapChain, 
                                    VkDescriptorSetLayout vkDescriptorSetLayout,
                                    VkDescriptorPool vkDescriptorPool,
                                    VkDescriptorSetVector& aDescriptorSets);

        VkDescriptorSetLayoutBinding CreateVkDescriptorSetLayoutBinding_Uniform(uint32_t binding,
                                                                                VkDescriptorType descriptorType,
                                                                                uint32_t descriptorCount,
                                                                                VkShaderStageFlags stageFlags);
        void CreateVkDescriptorSetLayoutBinding_Uniform(uint32_t binding,
                                                        VkDescriptorType descriptorType,
                                                        uint32_t descriptorCount,
                                                        VkShaderStageFlags stageFlags,
                                                        VkDescriptorSetLayoutBinding& descriptorSetLayoutBinding);
        VkDescriptorSetLayoutBinding CreateVkDescriptorSetLayoutBinding_Image(uint32_t binding,
                                                                              VkDescriptorType descriptorType,
                                                                              uint32_t descriptorCount,
                                                                              VkShaderStageFlags stageFlags,
                                                                              VkSampler* pImmutableSamplers);
        void CreateVkDescriptorSetLayoutBinding_Image(uint32_t binding,
                                                      VkDescriptorType descriptorType,
                                                      uint32_t descriptorCount,
                                                      VkShaderStageFlags stageFlags,
                                                      VkSampler* pImmutableSamplers,
                                                      VkDescriptorSetLayoutBinding& descriptorSetLayoutBinding);
        
        void PushVkDescriptorSet_Uniform(VkWriteDescriptorSetVector& aWriteDescriptorSets,
                                         VkDescriptorSet dstSet,
                                         uint32_t dstBinding,
                                         uint32_t dstArrayElement,
                                         uint32_t descriptorCount,
                                         VkDescriptorBufferInfo& bufferInfo);
        void PushVkDescriptorSet_Image(VkWriteDescriptorSetVector& aWriteDescriptorSets,
                                       VkDescriptorSet dstSet,
                                       uint32_t dstBinding,
                                       uint32_t dstArrayElement,
                                       uint32_t descriptorCount,
                                       VkDescriptorType descriptorType,
                                       VkDescriptorImageInfo& imageInfo);
        void UpdateVkDescriptorSets(VkWriteDescriptorSetVector& aWriteDescriptorSets);
    

    public:
        //////////////////// VkCommandBuffer ////////////////
        VkCommandBuffer BeginSingleTimeCommands();
        void EndSingleTimeCommands(VkCommandBuffer& vkCommandBuffer);


        void BeginRenderPass(VkCommandBuffer& commandBuffer, 
                             const VkRenderPass& renderPass, 
                             const VkFramebuffer& frameBuffer,
                             const VkOffset2D& offset,
                             const VkExtent2D& extent,
                             const FVector4& clBg,
                             float depth,
                             uint32_t stencil);
            void BindViewport(VkCommandBuffer& commandBuffer, const VkViewport& vkViewport, const VkRect2D& scissor, bool isNegativeViewport = true);
            void BindPipeline(VkCommandBuffer& commandBuffer, VkPipelineBindPoint pipelineBindPoint, const VkPipeline& vkPipeline);
            void BindVertexBuffer(VkCommandBuffer& commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets);
            void BindIndexBuffer(VkCommandBuffer& commandBuffer, const VkBuffer& vkIndexBuffer, VkDeviceSize offset, VkIndexType indexType);
            void BindDescriptorSets(VkCommandBuffer& commandBuffer, const VkPipelineBindPoint& pipelineBindPoint, const VkPipelineLayout& layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets);
            void Draw(VkCommandBuffer& commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
            void DrawIndexed(VkCommandBuffer& commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
            void DrawIndirect(VkCommandBuffer& commandBuffer, const VkBuffer& buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
            void DrawIndexedIndirect(VkCommandBuffer& commandBuffer, const VkBuffer& buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
        void EndRenderPass(VkCommandBuffer& commandBuffer);

        void Dispatch(VkCommandBuffer& commandBuffer, uint32_t groupCountX,  uint32_t groupCountY,  uint32_t groupCountZ);
        void DispatchIndirect(VkCommandBuffer& commandBuffer, const VkBuffer& buffer,  VkDeviceSize offset);


    /////////////////////////////////////// Vulkan Utility Wrapper ////////////////////////////////////////

    };

}; //LostPeterPluginRendererVulkan

#endif