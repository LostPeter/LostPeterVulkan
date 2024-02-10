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
        ConstCharPtrVector m_aAppDeviceExtensions;   

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
        VkCommandPool m_vkCommandPoolGraphics;
        VkCommandPool m_vkCommandPoolCompute;

        VkFormatVector m_aDepthVkFormat;
        VkFormat2PixelFormatMap m_mapDepthVkFormat2PixelFormat;

        VulkanInstance* m_pInstance;
        VulkanQueue* m_pQueueGraphics;
        VulkanQueue* m_pQueueCompute;
        VulkanQueue* m_pQueueTransfer;
        VulkanQueue* m_pQueuePresent;

        VulkanDeviceMemoryManager* m_pDeviceMemoryManager;
        VulkanFenceManager* m_pFenceManager;
        VulkanRenderPassManager* m_pRenderPassManager;
        VulkanFrameBufferManager* m_pFrameBufferManager;
        VulkanShaderModuleManager* m_pShaderModuleManager;
        VulkanDescriptorPoolManager* m_pDescriptorPoolManager;
        VulkanDescriptorSetLayoutManager* m_pDescriptorSetLayoutManager;
        VulkanDescriptorSetManager* m_pDescriptorSetManager;
        VulkanCommandPoolManager* m_pCommandPoolManager;

        VkDescriptorPool m_vkDescriptorPool;
        VkDescriptorPool m_vkDescriptorPool_ImGUI;
        
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

        F_FORCEINLINE VkCommandPool GetVkCommandPoolTransfer() const { return m_vkCommandPoolTransfer; }
        F_FORCEINLINE VkCommandPool GetVkCommandPoolGraphics() const { return m_vkCommandPoolGraphics; }
        F_FORCEINLINE VkCommandPool GetVkCommandPoolCompute() const { return m_vkCommandPoolCompute; }

        F_FORCEINLINE VulkanInstance* GetInstance() const { return m_pInstance; }
        F_FORCEINLINE VulkanQueue* GetQueueGraphics() const { return m_pQueueGraphics; }
        F_FORCEINLINE VulkanQueue* GetQueueCompute() const { return m_pQueueCompute; }
        F_FORCEINLINE VulkanQueue* GetQueueTransfer() const { return m_pQueueTransfer; }
        F_FORCEINLINE VulkanQueue* GetQueuePresent() const { return m_pQueuePresent; }

        F_FORCEINLINE VulkanDeviceMemoryManager* GetDeviceMemoryManager() const { return m_pDeviceMemoryManager; }
        F_FORCEINLINE VulkanFenceManager* GetFenceManager() const { return m_pFenceManager; }
        F_FORCEINLINE VulkanRenderPassManager* GetRenderPassManager() const { return m_pRenderPassManager; }
        F_FORCEINLINE VulkanFrameBufferManager* GetFrameBufferManager() const { return m_pFrameBufferManager; }
        F_FORCEINLINE VulkanShaderModuleManager* GetShaderModuleManager() const { return m_pShaderModuleManager; }  
        F_FORCEINLINE VulkanDescriptorPoolManager* GetDescriptorPoolManager() const { return m_pDescriptorPoolManager; }  
        F_FORCEINLINE VulkanDescriptorSetLayoutManager* GetDescriptorSetLayoutManager() const { return m_pDescriptorSetLayoutManager; }
        F_FORCEINLINE VulkanDescriptorSetManager* GetDescriptorSetManager() const { return m_pDescriptorSetManager; }
        F_FORCEINLINE VulkanCommandPoolManager* GetCommandPoolManager() const { return m_pCommandPoolManager; } 
        
        F_FORCEINLINE VkDescriptorPool GetVkDescriptorPool() const { return m_vkDescriptorPool; }
        F_FORCEINLINE VkDescriptorPool GetVkDescriptorPool_ImGUI() const { return m_vkDescriptorPool_ImGUI; }

    public:
        void Destroy();
        bool Init(int32 deviceIndex, bool bIsEnableValidationLayers);

    public:
        void AddAppDeviceExtensions(const char* szNameExtension);
        bool HasExtensionName(const char* szNameExtension);

    public: 
        bool QueryGPU(int32 deviceIndex);
        bool CreateQueuePresent(VkSurfaceKHR vkSurfaceKHR);

        bool IsPixelFormatSupported(VkFormat typeFormat);
        bool IsPixelFormatSupported(FPixelFormatType ePixelFormat);

        VkSampleCountFlagBits GetMaxUsableSampleCount();
        VkFormat FindSupportedFormat(const VkFormatVector& candidates, VkImageTiling typeImageTiling, VkFormatFeatureFlags features);
        VkFormat FindDepthVkFormat();
        FPixelFormatType FindDepthPixelFormatType();
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
        void WaitVkDeviceIdle();

        //////////////////// VkCommandPool //////////////////
        VkCommandPool CreateVkCommandPool(VkCommandPoolCreateFlags flags,
                                          uint32_t queueFamilyIndex);
        bool CreateVkCommandPool(VkCommandPoolCreateFlags flags,
                                 uint32_t queueFamilyIndex, 
                                 VkCommandPool& vkCommandPool);
        
        void DestroyVkCommandPool(const VkCommandPool& vkCommandPool);

        //////////////////// VkCommandBuffer ////////////////
        VkCommandBuffer AllocateVkCommandBuffer(VkCommandPool vkCommandPool,
                                                VkCommandBufferLevel level);
        bool AllocateVkCommandBuffers(VkCommandPool vkCommandPool,
                                      VkCommandBufferLevel level,
                                      uint32_t commandBufferCount,
                                      VkCommandBuffer* pCommandBuffers);
        void FreeVkCommandBuffers(VkCommandPool vkCommandPool, 
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
                                         uint32_t signalSemaphoreCount,
                                         VkSemaphore* pSignalSemaphores,
                                         VkFence vkFence);
        bool QueueWaitIdle(const VkQueue& vkQueue);

        //////////////////// VkSemaphore ////////////////////
        VkSemaphore CreateVkSemaphore();
        bool CreateVkSemaphore(VkSemaphore& vkSemaphore);
        void DestroyVkSemaphore(const VkSemaphore& vkSemaphore);

        void DestroyVkSemaphore(VulkanSemaphore* pSemaphore);
        void DestroyVkSemaphores(VulkanSemaphorePtrVector& aSemaphore);
        
        //////////////////// VkFence ////////////////////////
        VkFence CreateVkFence(bool isCreateSignaled);
        bool CreateVkFence(bool isCreateSignaled, VkFence& vkFence);
        void DestroyVkFence(const VkFence& vkFence);

        void DestroyVkFence(VulkanFence* pFence);
        void DestroyVkFences(VulkanFencePtrVector& aFence);
        void RecoveryFence(VulkanFence* pFence);
        void RecoveryFences(VulkanFencePtrVector& aFence);

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

        VulkanSwapStatusType VkAcquireNextImageKHR(VkSwapchainKHR vkSwapChainKHR,
                                                   uint64_t timeout,
                                                   VkSemaphore vkSemaphore,
                                                   VkFence vkFence,
                                                   uint32_t* pImageIndex);

        VulkanSwapStatusType VkQueuePresentKHR(VkSwapchainKHR vkSwapChainKHR,
                                               const VkSemaphoreVector& aWaitSemaphores,
                                               uint32_t* pImageIndices);
        VulkanSwapStatusType VkQueuePresentKHR(const VkSwapchainKHRVector& aSwapChainKHR,
                                               const VkSemaphoreVector& aWaitSemaphores,
                                               uint32_t* pImageIndices);


        //////////////////// VkViewport /////////////////////
        void CreateVkViewport(float nWidth,
                              float nHeight,
                              float offsetX,
                              float offsetY,
                              float minDepth,
                              float maxDepth,
                              VkViewport& vkViewport, 
                              VkRect2D& vkScissor);

        //////////////////// VkAttachmentDescription ////////
        void CreateVkAttachmentDescription(VkAttachmentDescriptionFlags flags,
                                           VkFormat typeFormat,
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

        
        //////////////////// VkFramebuffer //////////////////
        bool CreateVkFramebuffer(const String& nameFramebuffer,
                                 const VkImageViewVector& aImageView, 
                                 VkRenderPass& vkRenderPass,
                                 VkFramebufferCreateFlags flags,
                                 uint32_t nWidth,
                                 uint32_t nHeight,
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
        bool CreateVkImage(uint32_t nWidth, 
                           uint32_t nHeight, 
                           uint32_t nDepth, 
                           uint32_t nLayerCount,
                           uint32_t nMipMapCount, 
                           VkImageType typeImage, 
                           bool bIsCubeMap,
                           VkSampleCountFlagBits typeSamplesCountFlagBits, 
                           VkFormat typeFormat, 
                           VkImageTiling typeImageTiling, 
                           VkImageUsageFlags typeImageUsageFlags, 
                           VkSharingMode typeSharingMode,
                           bool bIsGraphicsComputeShared,
                           VkMemoryPropertyFlags properties, 
                           VkImage& vkImage, 
                           VkDeviceMemory& vkImageMemory);
        bool CreateVkImageView(VkImage vkImage, 
                               VkImageViewType typeImageView, 
                               VkFormat typeFormat, 
                               VkComponentMapping typeComponentMapping,
                               VkImageAspectFlags typeImageAspectFlags, 
                               uint32_t nMipMapCount,
                               uint32_t nLayerCount,
                               VkImageView& vkImageView);
        bool CreateVkSampler(uint32_t nMipMapCount, 
                             VkSampler& vkSampler);
        bool CreateVkSampler(FTextureFilterType eTextureFilter,
                             FTextureAddressingType eTextureAddressing,
                             FTextureBorderColorType eTextureBorderColor,
                             bool bIsEnableAnisotropy,
                             float fMaxAnisotropy,
                             float fMinLod, 
                             float fMaxLod, 
                             float fMipLodBias,
                             VkSampler& vkSampler);
        void DestroyVkImage(const VkImage& vkImage, const VkDeviceMemory& vkImageMemory, const VkImageView& vkImageView);
        void DestroyVkImageView(const VkImageView& vkImageView);
        void DestroyVkSampler(const VkSampler& vkSampler);
                        
        void TransitionVkImageLayout(VkCommandBuffer cmdBuffer,
                                     VkImage vkImage, 
                                     VkImageLayout oldLayout, 
                                     VkImageLayout newLayout,
                                     uint32_t nMipBase,
                                     uint32_t nMipCount,
                                     uint32_t nLayerBase,
                                     uint32_t nLayerCount,
                                     VkImageAspectFlags typeImageAspectFlags = VK_IMAGE_ASPECT_COLOR_BIT);
        void CopyVkBufferToVkImage(VkCommandBuffer cmdBuffer,
                                   VkBuffer vkBuffer, 
                                   VkImage vkImage, 
                                   uint32_t nWidth, 
                                   uint32_t nHeight,
                                   uint32_t nDepth,
                                   uint32_t nPixelSize,
                                   uint32_t nLayerCount);
        void GenerateVkImageMipMaps(VkCommandBuffer cmdBuffer,
                                    VkImage vkImage, 
                                    VkFormat imageFormat, 
                                    int32_t nWidth, 
                                    int32_t nHeight, 
                                    uint32_t nMipMapCount,
                                    uint32_t nLayerCount,
                                    bool bIsAutoMipMap);

        bool CreateTexture1D(const String& pathAsset, 
                             FPixelFormatType& ePixelFormatRet,
                             uint32_t& nMipMapCount,
                             VkImage& vkImage, 
                             VkDeviceMemory& vkImageMemory);

        bool CreateTexture2D(const String& pathAsset, 
                             VkImageType typeImage,
                             VkSampleCountFlagBits typeSamplesCountFlagBits,
                             VkFormat& typeFormat,
                             FPixelFormatType& ePixelFormatRet,
                             bool bIsAutoMipMap, 
                             uint32_t& nMipMapCount, 
                             VkImage& vkImage, 
                             VkDeviceMemory& vkImageMemory,
                             VkBuffer& vkBuffer, 
                             VkDeviceMemory& vkBufferMemory);
        bool CreateTexture2D(const String& pathAsset, 
                             VkImageType typeImage,
                             VkSampleCountFlagBits typeSamplesCountFlagBits,
                             VkFormat& typeFormat,
                             FPixelFormatType& ePixelFormatRet,
                             bool bIsAutoMipMap, 
                             uint32_t& nMipMapCount, 
                             VkImage& vkImage, 
                             VkDeviceMemory& vkImageMemory);
        bool CreateTexture2D(const String& pathAsset, 
                             FPixelFormatType& ePixelFormatRet,
                             uint32_t& nMipMapCount,
                             VkImage& vkImage, 
                             VkDeviceMemory& vkImageMemory);
        
        bool CreateTexture2DArray(const StringVector& aPathAsset, 
                                  VkImageType typeImage,
                                  VkSampleCountFlagBits typeSamplesCountFlagBits,
                                  VkFormat& typeFormat,
                                  FPixelFormatType& ePixelFormatRet,
                                  bool bIsAutoMipMap, 
                                  uint32_t& nMipMapCount, 
                                  VkImage& vkImage, 
                                  VkDeviceMemory& vkImageMemory,
                                  VkBuffer& vkBuffer, 
                                  VkDeviceMemory& vkBufferMemory);
        bool CreateTexture2DArray(const StringVector& aPathAsset, 
                                  VkImageType typeImage,
                                  VkSampleCountFlagBits typeSamplesCountFlagBits,
                                  VkFormat& typeFormat,
                                  FPixelFormatType& ePixelFormatRet,
                                  bool bIsAutoMipMap, 
                                  uint32_t& nMipMapCount, 
                                  VkImage& vkImage, 
                                  VkDeviceMemory& vkImageMemory);
        bool CreateTexture2DArray(const StringVector& aPathAsset, 
                                  FPixelFormatType& ePixelFormatRet,
                                  uint32_t& nMipMapCount,
                                  VkImage& vkImage, 
                                  VkDeviceMemory& vkImageMemory);

        bool CreateTexture3D(VkFormat typeFormat,
                             const uint8* pDataRGBA,
                             uint32_t nWidth,
                             uint32_t nHeight,
                             uint32_t nDepth,
                             uint32_t nPixelSize,
                             VkImage& vkImage, 
                             VkDeviceMemory& vkImageMemory,
                             VkBuffer& vkBuffer, 
                             VkDeviceMemory& vkBufferMemory);
        bool CreateTexture3D(VkFormat typeFormat,
                             const uint8* pDataRGBA,
                             uint32_t nWidth,
                             uint32_t nHeight,
                             uint32_t nDepth,
                             uint32_t nPixelSize,
                             VkImage& vkImage, 
                             VkDeviceMemory& vkImageMemory);
        
        bool CreateTextureCubeMap(const StringVector& aPathAsset, 
                                  VkSampleCountFlagBits typeSamplesCountFlagBits,
                                  VkFormat& typeFormat,
                                  FPixelFormatType& ePixelFormatRet,
                                  bool bIsAutoMipMap, 
                                  uint32_t& nMipMapCount, 
                                  VkImage& vkImage, 
                                  VkDeviceMemory& vkImageMemory,
                                  VkBuffer& vkBuffer, 
                                  VkDeviceMemory& vkBufferMemory);
        bool CreateTextureCubeMap(const StringVector& aPathAsset, 
                                  VkSampleCountFlagBits typeSamplesCountFlagBits,
                                  VkFormat& typeFormat,
                                  FPixelFormatType& ePixelFormatRet,
                                  bool bIsAutoMipMap, 
                                  uint32_t& nMipMapCount, 
                                  VkImage& vkImage, 
                                  VkDeviceMemory& vkImageMemory);
        bool CreateTextureCubeMap(const StringVector& aPathAsset,
                                  FPixelFormatType& ePixelFormatRet,
                                  uint32_t& nMipMapCount, 
                                  VkImage& vkImage, 
                                  VkDeviceMemory& vkImageMemory);

        
        bool CreateTextureRenderTarget1D(const FColor& clDefault,
                                         bool isSetColor,
                                         uint32_t nWidth, 
                                         uint32_t nMipMapCount,
                                         VkSampleCountFlagBits typeSamplesCountFlagBits,
                                         VkFormat typeFormat,
                                         VkImageUsageFlags typeImageUsageFlags, 
                                         bool bIsGraphicsComputeShared,
                                         VkImage& vkImage, 
                                         VkDeviceMemory& vkImageMemory,
                                         VkBuffer& vkBuffer, 
                                         VkDeviceMemory& vkBufferMemory);
        bool CreateTextureRenderTarget1D(const FColor& clDefault,
                                         bool isSetColor,
                                         uint32_t nWidth, 
                                         uint32_t nMipMapCount,
                                         VkSampleCountFlagBits typeSamplesCountFlagBits,
                                         VkFormat typeFormat,
                                         VkImageUsageFlags typeImageUsageFlags, 
                                         bool bIsGraphicsComputeShared,
                                         VkImage& vkImage, 
                                         VkDeviceMemory& vkImageMemory);
        
        bool CreateTextureRenderTarget2D(const FColor& clDefault,
                                         bool isSetColor,
                                         uint32_t nWidth, 
                                         uint32_t nHeight,
                                         uint32_t nMipMapCount,
                                         VkImageType typeImage,
                                         VkSampleCountFlagBits typeSamplesCountFlagBits,
                                         VkFormat typeFormat,
                                         VkImageUsageFlags typeImageUsageFlags, 
                                         bool bIsGraphicsComputeShared,
                                         VkImage& vkImage, 
                                         VkDeviceMemory& vkImageMemory,
                                         VkBuffer& vkBuffer, 
                                         VkDeviceMemory& vkBufferMemory);
        bool CreateTextureRenderTarget2D(const FColor& clDefault,
                                         bool isSetColor,
                                         uint32_t nWidth, 
                                         uint32_t nHeight,
                                         uint32_t nMipMapCount,
                                         VkSampleCountFlagBits typeSamplesCountFlagBits,
                                         VkFormat typeFormat,
                                         VkImageUsageFlags typeImageUsageFlags, 
                                         bool bIsGraphicsComputeShared,
                                         VkImage& vkImage, 
                                         VkDeviceMemory& vkImageMemory);

        bool CreateTextureRenderTarget2D(uint8* pData,
                                         uint32_t nWidth, 
                                         uint32_t nHeight,
                                         uint32_t nMipMapCount,
                                         VkImageType typeImage,
                                         VkSampleCountFlagBits typeSamplesCountFlagBits,
                                         VkFormat typeFormat,
                                         VkImageUsageFlags typeImageUsageFlags, 
                                         bool bIsGraphicsComputeShared,
                                         VkImage& vkImage, 
                                         VkDeviceMemory& vkImageMemory,
                                         VkBuffer& vkBuffer, 
                                         VkDeviceMemory& vkBufferMemory);
        bool CreateTextureRenderTarget2D(uint8* pData,
                                         uint32_t nWidth, 
                                         uint32_t nHeight,
                                         uint32_t nMipMapCount,
                                         VkSampleCountFlagBits typeSamplesCountFlagBits,
                                         VkFormat typeFormat,
                                         VkImageUsageFlags typeImageUsageFlags, 
                                         bool bIsGraphicsComputeShared,
                                         VkImage& vkImage, 
                                         VkDeviceMemory& vkImageMemory);
        
        bool CreateTextureRenderTarget2DArray(const FColor& clDefault,
                                              bool isSetColor,
                                              uint32_t nWidth, 
                                              uint32_t nHeight,
                                              uint32_t nLayerCount,
                                              uint32_t nMipMapCount,
                                              VkImageType typeImage,
                                              VkSampleCountFlagBits typeSamplesCountFlagBits,
                                              VkFormat typeFormat,
                                              VkImageUsageFlags typeImageUsageFlags, 
                                              bool bIsGraphicsComputeShared,
                                              VkImage& vkImage, 
                                              VkDeviceMemory& vkImageMemory,
                                              VkBuffer& vkBuffer, 
                                              VkDeviceMemory& vkBufferMemory);
        bool CreateTextureRenderTarget2DArray(const FColor& clDefault,
                                              bool isSetColor,
                                              uint32_t nWidth, 
                                              uint32_t nHeight,
                                              uint32_t nLayerCount,
                                              uint32_t nMipMapCount,
                                              VkSampleCountFlagBits typeSamplesCountFlagBits,
                                              VkFormat typeFormat,
                                              VkImageUsageFlags typeImageUsageFlags, 
                                              bool bIsGraphicsComputeShared,
                                              VkImage& vkImage, 
                                              VkDeviceMemory& vkImageMemory);

        bool CreateTextureRenderTarget3D(const FColor& clDefault,
                                         bool isSetColor,
                                         uint32_t nWidth, 
                                         uint32_t nHeight,
                                         uint32_t nDepth,
                                         uint32_t nMipMapCount,
                                         VkSampleCountFlagBits typeSamplesCountFlagBits,
                                         VkFormat typeFormat,
                                         VkImageUsageFlags typeImageUsageFlags, 
                                         bool bIsGraphicsComputeShared,
                                         VkImage& vkImage, 
                                         VkDeviceMemory& vkImageMemory,
                                         VkBuffer& vkBuffer, 
                                         VkDeviceMemory& vkBufferMemory);                                           
        bool CreateTextureRenderTarget3D(const FColor& clDefault,
                                         bool isSetColor,
                                         uint32_t nWidth, 
                                         uint32_t nHeight,
                                         uint32_t nDepth,
                                         uint32_t nMipMapCount,
                                         VkSampleCountFlagBits typeSamplesCountFlagBits,
                                         VkFormat typeFormat,
                                         VkImageUsageFlags typeImageUsageFlags, 
                                         bool bIsGraphicsComputeShared,
                                         VkImage& vkImage, 
                                         VkDeviceMemory& vkImageMemory);
        
        bool CreateTextureRenderTargetCubeMap(uint32_t nWidth, 
                                              uint32_t nHeight,
                                              uint32_t nMipMapCount,
                                              VkSampleCountFlagBits typeSamplesCountFlagBits,
                                              VkFormat typeFormat,
                                              VkImageUsageFlags typeImageUsageFlags, 
                                              bool bIsGraphicsComputeShared,
                                              VkImage& vkImage, 
                                              VkDeviceMemory& vkImageMemory,
                                              VkBuffer& vkBuffer, 
                                              VkDeviceMemory& vkBufferMemory);
        bool CreateTextureRenderTargetCubeMap(uint32_t nWidth, 
                                              uint32_t nHeight,
                                              uint32_t nMipMapCount,
                                              VkSampleCountFlagBits typeSamplesCountFlagBits,
                                              VkFormat typeFormat,
                                              VkImageUsageFlags typeImageUsageFlags, 
                                              bool bIsGraphicsComputeShared,
                                              VkImage& vkImage, 
                                              VkDeviceMemory& vkImageMemory);

        bool CreateTextureFrameBufferColor(uint32_t nWidth, 
                                           uint32_t nHeight,
                                           uint32_t nDepth,
                                           VkSampleCountFlagBits typeSamplesCountFlagBits, 
                                           VkFormat typeFormat, 
                                           VkImage& vkImage, 
                                           VkDeviceMemory& vkImageMemory);

        bool CreateTextureFrameBufferDepth(uint32_t nWidth, 
                                           uint32_t nHeight,
                                           uint32_t nDepth,
                                           VkSampleCountFlagBits typeSamplesCountFlagBits, 
                                           VkFormat typeFormat, 
                                           VkImage& vkImage, 
                                           VkDeviceMemory& vkImageMemory);


        //////////////////// VkShaderModule /////////////////
        bool CreateVkShaderModule(FShaderType eShader, 
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
                             const FColor& clBg,
                             float fDepth,
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