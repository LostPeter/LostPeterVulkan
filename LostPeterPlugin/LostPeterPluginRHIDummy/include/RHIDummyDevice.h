/****************************************************************************
* LostPeterPluginRHIDummy - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_DUMMY_DEVICE_H_
#define _RHI_DUMMY_DEVICE_H_

#include "RHIDummyPreDefine.h"

namespace LostPeterPluginRHIDummy
{
    class rhiDummyExport RHIDummyDevice : public RHIDevice
    {
    public:
        RHIDummyDevice(RHIDummyPhysicalDevice* pDummyPhysicalDevice, const RHIDeviceCreateInfo& createInfo);
        virtual ~RHIDummyDevice();

    public:
    protected:
        RHIDummyPhysicalDevice* m_pDummyPhysicalDevice;
        RHIDummyQueue* m_pDummyQueue;
        RHIDummyCommandPool* m_pDummyCommandPool;

        RHIDummyObjectManager* m_pDummyObjectManager;

        RHIDummyBindGroupLayoutCache* m_pDummyBindGroupLayoutCache;
        RHIDummyBindGroupCache* m_pDummyBindGroupCache;
        RHIDummyShaderModuleCache* m_pDummyShaderModuleCache;
        RHIDummyPipelineLayoutCache* m_pDummyPipelineLayoutCache;
        RHIDummyPipelineCache* m_pDummyPipelineCache;
        RHIDummyRenderPassCache* m_pDummyRenderPassCache;

    public:
        F_FORCEINLINE RHIDummyPhysicalDevice* GetDummyPhysicalDevice() { return m_pDummyPhysicalDevice; }
        F_FORCEINLINE RHIDummyObjectManager* GetDummyObjectManager() { return m_pDummyObjectManager; }
        
        F_FORCEINLINE RHIDummyBindGroupLayoutCache* GetDummyBindGroupLayoutCache() const { return m_pDummyBindGroupLayoutCache; }
        F_FORCEINLINE RHIDummyBindGroupCache* GetDummyBindGroupCache() const { return m_pDummyBindGroupCache; }
        F_FORCEINLINE RHIDummyShaderModuleCache* GetDummyShaderModuleCache() const { return m_pDummyShaderModuleCache; }
        F_FORCEINLINE RHIDummyPipelineLayoutCache* GetDummyPipelineLayoutCache() const { return m_pDummyPipelineLayoutCache; }
        F_FORCEINLINE RHIDummyPipelineCache* GetDummyPipelineCache() const { return m_pDummyPipelineCache; }
        F_FORCEINLINE RHIDummyRenderPassCache* GetDummyRenderPassCache() const { return m_pDummyRenderPassCache; }

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
        virtual RHICommandPool* CreateCommandPool();
        virtual RHICommandBuffer* CreateCommandBuffer();

        virtual bool CheckSwapChainFormatSupport(RHISurface* pSurface, RHIPixelFormatType ePixelFormat);
        virtual bool IsPixelFormatSupported(RHIPixelFormatType ePixelFormat);
    };
    
}; //LostPeterPluginRHIDummy

#endif