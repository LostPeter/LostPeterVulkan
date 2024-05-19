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
        virtual RHISampler* CreateSampler(const RHISamplerCreateInfo& createInfo);
        virtual RHIBindGroupLayout* CreateBindGroupLayout(const RHIBindGroupLayoutCreateInfo& createInfo);
        virtual RHIBindGroup* CreateBindGroup(const RHIBindGroupCreateInfo& createInfo);
        virtual RHIPipelineLayout* CreatePipelineLayout(const RHIPipelineLayoutCreateInfo& createInfo);
        virtual RHIShaderModule* CreateShaderModule(const RHIShaderModuleCreateInfo& createInfo);
        virtual RHIPipelineCompute* CreatePipelineCompute(const RHIPipelineComputeCreateInfo& createInfo);
        virtual RHIPipelineGraphics* CreatePipelineGraphics(const RHIPipelineGraphicsCreateInfo& createInfo);
        virtual RHICommandPool* CreateCommandPool();
        virtual RHICommandBuffer* CreateCommandBuffer();
        virtual RHIFence* CreateFence();
        virtual RHIFence* CreateFence(bool bIsSignaled);

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

    public:
        //////////////////// VkCommandBuffer ////////////////
        VkCommandBuffer BeginSingleTimeCommands();
        void EndSingleTimeCommands(VkCommandBuffer& vkCommandBuffer);

    };
    
}; //LostPeterPluginRHIVulkan

#endif