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

#ifndef _RHI_VULKAN_DEVICE_H_
#define _RHI_VULKAN_DEVICE_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanDevice : public RHIDevice
    {
    public:
        RHIVulkanDevice(RHIVulkanPhysicalDevice* pVulkanPhysicalDevice, const RHIDeviceCreateInfo& createInfo);
        virtual ~RHIVulkanDevice();

    public:
    protected:
        ConstCharPtrVector m_aAppDeviceExtensions; 

    protected:
        RHIVulkanPhysicalDevice* m_pVulkanPhysicalDevice;
        VkDevice m_vkDevice;
        VmaAllocator m_vmaAllocator;
        RHIVulkanObjectManager* m_pVulkanObjectManager;

        RHIVulkanBindGroupLayoutCache* m_pVulkanBindGroupLayoutCache;
        RHIVulkanBindGroupCache* m_pVulkanBindGroupCache;
        RHIVulkanShaderModuleCache* m_pVulkanShaderModuleCache;
        RHIVulkanPipelineLayoutCache* m_pVulkanPipelineLayoutCache;
        RHIVulkanPipelineCache* m_pVulkanPipelineCache;
        RHIVulkanRenderPassCache* m_pVulkanRenderPassCache;

        String m_strDebugName;

        RHIVulkanCommandPool* m_pCommandPoolTransfer;
        RHIVulkanCommandPool* m_pCommandPoolGraphics;
        RHIVulkanCommandPool* m_pCommandPoolCompute;
        RHIVulkanCommandPoolPtrVector m_aVulkanCommandPools;
        RHIVulkanCommandPoolPtrMap m_mapVulkanCommandPools;

        RHIVulkanQueue* m_pQueueGraphics;
        RHIVulkanQueue* m_pQueueCompute;
        RHIVulkanQueue* m_pQueueTransfer;
        RHIVulkanQueue* m_pQueuePresent;
        RHIVulkanQueuePtrVector m_aVulkanQueues;
        RHIVulkanQueuePtrMap m_mapVulkanQueues;

    public:
        F_FORCEINLINE RHIVulkanPhysicalDevice* GetVulkanPhysicalDevice() const { return m_pVulkanPhysicalDevice; }
        F_FORCEINLINE VkDevice& GetVkDevice() { return m_vkDevice; }
        F_FORCEINLINE VmaAllocator& GetVmaAllocator() { return m_vmaAllocator; }
        F_FORCEINLINE RHIVulkanObjectManager* GetVulkanObjectManager() const { return m_pVulkanObjectManager; }

        F_FORCEINLINE RHIVulkanBindGroupLayoutCache* GetVulkanBindGroupLayoutCache() const { return m_pVulkanBindGroupLayoutCache; }
        F_FORCEINLINE RHIVulkanBindGroupCache* GetVulkanBindGroupCache() const { return m_pVulkanBindGroupCache; }
        F_FORCEINLINE RHIVulkanShaderModuleCache* GetVulkanShaderModuleCache() const { return m_pVulkanShaderModuleCache; }
        F_FORCEINLINE RHIVulkanPipelineLayoutCache* GetVulkanPipelineLayoutCache() const { return m_pVulkanPipelineLayoutCache; }
        F_FORCEINLINE RHIVulkanPipelineCache* GetVulkanPipelineCache() const { return m_pVulkanPipelineCache; }
        F_FORCEINLINE RHIVulkanRenderPassCache* GetVulkanRenderPassCache() const { return m_pVulkanRenderPassCache; }

        F_FORCEINLINE const String& GetDebugName() { return m_strDebugName; }

        F_FORCEINLINE RHIVulkanCommandPool* GetVulkanCommandPoolTransfer() const { return m_pCommandPoolTransfer; }
        F_FORCEINLINE RHIVulkanCommandPool* GetVulkanCommandPoolGraphics() const { return m_pCommandPoolGraphics; }
        F_FORCEINLINE RHIVulkanCommandPool* GetVulkanCommandPoolCompute() const { return m_pCommandPoolCompute; }

        F_FORCEINLINE RHIVulkanQueue* GetVulkanQueueGraphics() const { return m_pQueueGraphics; }
        F_FORCEINLINE RHIVulkanQueue* GetVulkanQueueCompute() const { return m_pQueueCompute; }
        F_FORCEINLINE RHIVulkanQueue* GetVulkanQueueTransfer() const { return m_pQueueTransfer; }
        F_FORCEINLINE RHIVulkanQueue* GetVulkanQueuePresent() const { return m_pQueuePresent; }

    public:
        virtual void Destroy();

        virtual uint32 GetQueueCount(RHIQueueType eQueue);
        virtual RHIQueue* GetQueue(RHIQueueType eQueue);
        virtual RHICommandPool* GetCommandPool(RHIQueueType eQueue);
        
        virtual RHISurface* CreateSurface(const RHISurfaceCreateInfo& createInfo);
        virtual RHISwapChain* CreateSwapChain(const RHISwapChainCreateInfo& createInfo);
        virtual RHIBuffer* CreateBuffer(const RHIBufferCreateInfo& createInfo);
        virtual RHITexture* CreateTexture(const RHITextureCreateInfo& createInfo);
        virtual RHITexture* CreateTexture(const RHITextureCreateInfo& createInfo, VkImage& vkImage);
        virtual RHISampler* CreateSampler(const RHISamplerCreateInfo& createInfo);
        virtual RHIBindGroupLayoutCache* CreateBindGroupLayoutCache(const RHIBindGroupLayoutCacheCreateInfo& createInfo);
        virtual RHIBindGroupLayout* CreateBindGroupLayout(const RHIBindGroupLayoutCreateInfo& createInfo);
        virtual RHIBindGroupPool* CreateBindGroupPool(const RHIBindGroupPoolCreateInfo& createInfo);
        virtual RHIBindGroupCache* CreateBindGroupCache(const RHIBindGroupCacheCreateInfo& createInfo);
        virtual RHIBindGroup* CreateBindGroup(const RHIBindGroupCreateInfo& createInfo);
        virtual RHIShaderModuleCache* CreateShaderModuleCache(const RHIShaderModuleCacheCreateInfo& createInfo);
        virtual RHIShaderModule* CreateShaderModule(const RHIShaderModuleCreateInfo& createInfo);
        virtual RHIPipelineLayoutCache* CreatePipelineLayoutCache(const RHIPipelineLayoutCacheCreateInfo& createInfo);
        virtual RHIPipelineLayout* CreatePipelineLayout(const RHIPipelineLayoutCreateInfo& createInfo);
        virtual RHIPipelineCache* CreatePipelineCache(const RHIPipelineCacheCreateInfo& createInfo);
        virtual RHIPipelineCompute* CreatePipelineCompute(const RHIPipelineComputeCreateInfo& createInfo);
        virtual RHIPipelineGraphics* CreatePipelineGraphics(const RHIPipelineGraphicsCreateInfo& createInfo);
        virtual RHIRenderPassCache* CreateRenderPassCache(const RHIRenderPassCacheCreateInfo& createInfo);
        virtual RHIRenderPass* CreateRenderPass(const RHIRenderPassCreateInfo& createInfo);
        virtual RHIFrameBuffer* CreateFrameBuffer(const RHIFrameBufferCreateInfo& createInfo);
        virtual RHIFence* CreateFence(const RHIFenceCreateInfo& createInfo);
        virtual RHISemaphore* CreateSemaphore(const RHISemaphoreCreateInfo& createInfo);
        virtual RHICommandPool* CreateCommandPool(const RHICommandPoolCreateInfo& createInfo);
        virtual RHICommandBuffer* CreateCommandBuffer(const RHICommandBufferCreateInfo& createInfo);
        virtual RHIQueue* CreateQueue(const RHIQueueCreateInfo& createInfo);

        virtual bool CheckSwapChainFormatSupport(RHISurface* pSurface, RHIPixelFormatType ePixelFormat);
        virtual bool IsPixelFormatSupported(RHIPixelFormatType ePixelFormat);

    public:
        bool IsPixelFormatSupported(VkFormat typeFormat);
        
        void AddAppDeviceExtensions(const char* szNameExtension);
        bool HasExtensionName(const char* szNameExtension);

        bool IsSupportPresent(RHIVulkanQueue* pQueue, VkSurfaceKHR vkSurfaceKHR);
        bool CreateQueuePresent(VkSurfaceKHR vkSurfaceKHR);

        void SetDebugObject(VkObjectType objectType, uint64_t objectHandle, const char* objectName);

    protected:
        void destroyCache();
        void destroyVMA();
        void destroyVulkanQueue();
        void destroyVulkanCommandPool();

        bool init(bool bIsEnableValidationLayers);
        bool createDevice(bool bIsEnableValidationLayers);
        bool createVmaAllocator();
        bool checkPixelFormats();

    private:
        void getDeviceLayersAndExtensions(bool bIsEnableValidationLayers,
                                          ConstCharPtrVector& outDeviceLayers, 
                                          ConstCharPtrVector& outDeviceExtensions);

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
        bool AllocateVkCommandBuffer(VkCommandPool vkCommandPool,
                                     VkCommandBufferLevel level,
                                     VkCommandBuffer& vkCommandBuffer);
        bool AllocateVkCommandBuffers(VkCommandPool vkCommandPool,
                                      VkCommandBufferLevel level,
                                      const void* pNext,
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

        void DestroyVulkanSemaphore(RHIVulkanSemaphore* pVulkanSemaphore);
        void DestroyVulkanSemaphores(RHIVulkanSemaphorePtrVector& aVulkanSemaphore);
        
        //////////////////// VkFence ////////////////////////
        VkFence CreateVkFence(bool isCreateSignaled);
        bool CreateVkFence(bool isCreateSignaled, VkFence& vkFence);
        void DestroyVkFence(const VkFence& vkFence);

        bool WaitVkFence(const VkFence& vkFence);
        bool WaitVkFences(const VkFenceVector& aFences);
        bool ResetVkFence(const VkFence& vkFence);
        bool ResetVkFences(const VkFenceVector& aFences);

        void DestroyVulkanFence(RHIVulkanFence* pVulkanFence);
        void DestroyVulkanFences(RHIVulkanFencePtrVector& aVulkanFence);
        void WaitFence(RHIVulkanFence* pVulkanFence);
        void WaitFences(RHIVulkanFencePtrVector& aVulkanFence);
        void ResetFence(RHIVulkanFence* pVulkanFence);
        void ResetFences(RHIVulkanFencePtrVector& aVulkanFence);

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

        RHISwapStatusType VkAcquireNextImageKHR(VkSwapchainKHR vkSwapChainKHR,
                                                uint64_t timeout,
                                                VkSemaphore vkSemaphore,
                                                VkFence vkFence,
                                                uint32_t* pImageIndex);

        RHISwapStatusType VkQueuePresentKHR(VkQueue vkQueue,
                                            VkSwapchainKHR vkSwapChainKHR,
                                            const VkSemaphoreVector& aWaitSemaphores,
                                            uint32_t* pImageIndices);
        RHISwapStatusType VkQueuePresentKHR(VkQueue vkQueue,
                                            const VkSwapchainKHRVector& aSwapChainKHR,
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

        //////////////////// VkRenderPass ///////////////////
        bool CreateVkRenderPass(const String& nameRenderPass,
                                const VkAttachmentDescriptionVector& aAttachmentDescription,
                                const VkSubpassDescriptionVector& aSubpassDescription,
                                const VkSubpassDependencyVector& aSubpassDependency,
                                VkRenderPassMultiviewCreateInfo* pMultiviewCI,
                                VkRenderPass& vkRenderPass);
        bool CreateVkRenderPass(const String& nameRenderPass,
                                const RHIGraphicsPassColorAttachmentVector& aColorAttachment,
                                const RHIGraphicsPassDepthStencilAttachmentVector& aDepthStencilAttachment,
                                const RHIGraphicsSubpassDescriptionVector& aSubpassDescription,
                                const RHIGraphicsSubpassDependencyVector& aSubpassDependency,
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
        bool CreateVkFramebuffer(const String& nameFramebuffer,
                                 const RHIVulkanTextureViewPtrVector& aTextureView,
                                 RHIVulkanRenderPass* pVulkanRenderPass,
                                 const RHIExtent<3> sExtent,
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
                               uint32_t nBaseMipLevel,
                               uint32_t nMipMapCount,
                               uint32_t nBaseArrayLayer,
                               uint32_t nLayerCount,
                               VkImageView& vkImageView);
        bool CreateVkSampler(uint32_t nMipMapCount, 
                             VkSampler& vkSampler);
        bool CreateVkSampler(RHIAddressType eAddressingU,
                             RHIAddressType eAddressingV,
                             RHIAddressType eAddressingW,
                             RHIBorderColorType eBorderColor,
                             RHIFilterType eFilterMin,
                             RHIFilterType eFilterMag,
                             RHIFilterType eFilterMip,
                             RHIComparisonFuncType eComparisonFunc,
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
                             RHIPixelFormatType& ePixelFormatRet,
                             uint32_t& nMipMapCount,
                             VkImage& vkImage, 
                             VkDeviceMemory& vkImageMemory);

        bool CreateTexture2D(const String& pathAsset, 
                             VkImageType typeImage,
                             VkSampleCountFlagBits typeSamplesCountFlagBits,
                             VkFormat& typeFormat,
                             RHIPixelFormatType& ePixelFormatRet,
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
                             RHIPixelFormatType& ePixelFormatRet,
                             bool bIsAutoMipMap, 
                             uint32_t& nMipMapCount, 
                             VkImage& vkImage, 
                             VkDeviceMemory& vkImageMemory);
        bool CreateTexture2D(const String& pathAsset, 
                             RHIPixelFormatType& ePixelFormatRet,
                             uint32_t& nMipMapCount,
                             VkImage& vkImage, 
                             VkDeviceMemory& vkImageMemory);
        
        bool CreateTexture2DArray(const StringVector& aPathAsset, 
                                  VkImageType typeImage,
                                  VkSampleCountFlagBits typeSamplesCountFlagBits,
                                  VkFormat& typeFormat,
                                  RHIPixelFormatType& ePixelFormatRet,
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
                                  RHIPixelFormatType& ePixelFormatRet,
                                  bool bIsAutoMipMap, 
                                  uint32_t& nMipMapCount, 
                                  VkImage& vkImage, 
                                  VkDeviceMemory& vkImageMemory);
        bool CreateTexture2DArray(const StringVector& aPathAsset, 
                                  RHIPixelFormatType& ePixelFormatRet,
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
                                  RHIPixelFormatType& ePixelFormatRet,
                                  bool bIsAutoMipMap, 
                                  uint32_t& nMipMapCount, 
                                  VkImage& vkImage, 
                                  VkDeviceMemory& vkImageMemory,
                                  VkBuffer& vkBuffer, 
                                  VkDeviceMemory& vkBufferMemory);
        bool CreateTextureCubeMap(const StringVector& aPathAsset, 
                                  VkSampleCountFlagBits typeSamplesCountFlagBits,
                                  VkFormat& typeFormat,
                                  RHIPixelFormatType& ePixelFormatRet,
                                  bool bIsAutoMipMap, 
                                  uint32_t& nMipMapCount, 
                                  VkImage& vkImage, 
                                  VkDeviceMemory& vkImageMemory);
        bool CreateTextureCubeMap(const StringVector& aPathAsset,
                                  RHIPixelFormatType& ePixelFormatRet,
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
        bool CreateVkShaderModule(RHIShaderStageBitsType eShaderStageBits, 
                                  const String& pathFile,
                                  VkShaderModule& vkShaderModule);
        bool CreateVkShaderModule(const String& strTypeShader, 
                                  const String& pathFile,
                                  VkShaderModule& vkShaderModule);
        bool CreateVkShaderModule(size_t nSizeByteCode,
                                  const void* pByteCode,
                                  VkShaderModule& vkShaderModule);
        void DestroyVkShaderModule(const VkShaderModule& vkShaderModule);

        //////////////////// VkDescriptorSetLayout //////////
        bool CreateVkDescriptorSetLayout(const VkDescriptorSetLayoutBindingVector& aDescriptorSetLayoutBinding, 
                                         VkDescriptorSetLayout& vkDescriptorSetLayout);
        void DestroyVkDescriptorSetLayout(const VkDescriptorSetLayout& vkDescriptorSetLayout);

        //////////////////// VkPipelineLayout ///////////////
        bool CreateVkPipelineLayout(const VkDescriptorSetLayoutVector& aDescriptorSetLayout,
                                    const VkPushConstantRangeVector& aPushConstantRange,
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
                                       VkRenderPass renderPass, VkPipelineLayout vkPipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors,
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
                                       VkRenderPass renderPass, VkPipelineLayout vkPipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors,
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
                                       VkRenderPass renderPass, VkPipelineLayout vkPipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors,
                                       VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, float lineWidth,
                                       VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                       VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                       VkBool32 bBlend, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
                                       VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
                                       VkColorComponentFlags colorWriteMask,
                                       VkSampleCountFlagBits msaaSamples,
                                       VkPipelineCache vkPipelineCache,
                                       VkPipeline& vkPipeline);
        bool CreateVkPipeline_Graphics(const VkGraphicsPipelineCreateInfo& createInfo,
                                       VkPipelineCache vkPipelineCache,
                                       VkPipeline& vkPipeline);
        bool CreateVkPipeline_Graphics(RHIVulkanShaderModule* pVulkanShaderVertex, 
                                       RHIVulkanShaderModule* pVulkanShaderPixel,
                                       RHIVulkanShaderModule* pVulkanShaderGeometry,
                                       RHIVulkanShaderModule* pVulkanShaderDomain,
                                       RHIVulkanShaderModule* pVulkanShaderHull,
                                       RHIVulkanPipelineLayout* pVulkanPipelineLayout,
                                       RHIVulkanPipelineCache* pVulkanPipelineCache,
                                       RHIVulkanRenderPass* pVulkanRenderPass,
                                       const RHIVertexState& sVertexState,
                                       const RHIPrimitiveState& sPrimitiveState,
                                       const RHITessellationState& sTessellationState,
                                       const RHIDepthStencilState& sDepthStencilState,
                                       const RHIMultiSampleState& sMultiSampleState,
                                       const RHIFragmentState& sFragmentState,
                                       VkPipeline& vkPipeline);

        bool CreateVkPipeline_Compute(VkShaderModule compShaderModule,
                                      const String& compMain,
                                      VkPipelineLayout vkPipelineLayout, 
                                      VkPipelineCreateFlags flags,
                                      VkPipelineCache vkPipelineCache,
                                      VkPipeline& vkPipeline);
        bool CreateVkPipeline_Compute(const VkPipelineShaderStageCreateInfo& shaderStageCreateInfo,
                                      VkPipelineLayout vkPipelineLayout, 
                                      VkPipelineCreateFlags flags,
                                      VkPipelineCache vkPipelineCache,
                                      VkPipeline& vkPipeline);
        bool CreateVkPipeline_Compute(RHIVulkanShaderModule* pVulkanComputeShader,
                                      RHIVulkanPipelineLayout* pVulkanPipelineLayout,
                                      RHIVulkanPipelineCache* pVulkanPipelineCache,
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
        void FreeVkDescriptorSets(VkDescriptorPool vkDescriptorPool, const VkDescriptorSet& vkDescriptorSet);
        void FreeVkDescriptorSets(VkDescriptorPool vkDescriptorPool, VkDescriptorSetVector& aDescriptorSets);

        VkDescriptorSetLayoutBinding CreateVkDescriptorSetLayoutBinding_Buffer(uint32_t binding,
                                                                               VkDescriptorType descriptorType,
                                                                               uint32_t descriptorCount,
                                                                               VkShaderStageFlags stageFlags);
        void CreateVkDescriptorSetLayoutBinding_Buffer(uint32_t binding,
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
        
        void PushVkDescriptorSet_Buffer(VkWriteDescriptorSetVector& aWriteDescriptorSets,
                                        VkDescriptorSet dstSet,
                                        uint32_t dstBinding,
                                        uint32_t dstArrayElement,
                                        uint32_t descriptorCount,
                                        VkDescriptorType descriptorType,
                                        VkDescriptorBufferInfo& bufferInfo);
        void PushVkDescriptorSet_Image(VkWriteDescriptorSetVector& aWriteDescriptorSets,
                                       VkDescriptorSet dstSet,
                                       uint32_t dstBinding,
                                       uint32_t dstArrayElement,
                                       uint32_t descriptorCount,
                                       VkDescriptorType descriptorType,
                                       VkDescriptorImageInfo& imageInfo);
        void UpdateVkDescriptorSets(VkWriteDescriptorSetVector& aWriteDescriptorSets);

        void CreateVkPushConstantRange(VkShaderStageFlags stageFlags,
                                       uint32_t offset,
                                       uint32_t size,
                                       VkPushConstantRange& pushConstantRange);

    public:
        //////////////////// VkCommandBuffer ////////////////
        VkCommandBuffer BeginSingleTimeCommands();
        void EndSingleTimeCommands(VkCommandBuffer& vkCommandBuffer);

    };
    
}; //LostPeterPluginRHIVulkan

#endif