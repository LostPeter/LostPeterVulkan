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

    public:
        F_FORCEINLINE RHIDummyPhysicalDevice* GetDummyPhysicalDevice() { return m_pDummyPhysicalDevice; }

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
    };
    
}; //LostPeterPluginRHIDummy

#endif