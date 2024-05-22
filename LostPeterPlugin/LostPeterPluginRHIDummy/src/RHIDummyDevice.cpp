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

#include "../include/RHIDummyDevice.h"
#include "../include/RHIDummyPhysicalDevice.h"
#include "../include/RHIDummyQueue.h"
#include "../include/RHIDummySurface.h"
#include "../include/RHIDummySwapChain.h"
#include "../include/RHIDummyBuffer.h"
#include "../include/RHIDummyTexture.h"
#include "../include/RHIDummySampler.h"
#include "../include/RHIDummyBindGroupLayout.h"
#include "../include/RHIDummyBindGroupPool.h"
#include "../include/RHIDummyBindGroupCache.h"
#include "../include/RHIDummyBindGroup.h"
#include "../include/RHIDummyShaderModule.h"
#include "../include/RHIDummyPipelineLayout.h"
#include "../include/RHIDummyPipelineCache.h"
#include "../include/RHIDummyPipelineCompute.h"
#include "../include/RHIDummyPipelineGraphics.h"
#include "../include/RHIDummyRenderPassCache.h"
#include "../include/RHIDummyRenderPass.h"
#include "../include/RHIDummyFrameBuffer.h"
#include "../include/RHIDummyCommandPool.h"
#include "../include/RHIDummyCommandBuffer.h"
#include "../include/RHIDummyFence.h"
#include "../include/RHIDummySemaphore.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyDevice::RHIDummyDevice(RHIDummyPhysicalDevice* pDummyPhysicalDevice, const RHIDeviceCreateInfo& createInfo)
        : RHIDevice(pDummyPhysicalDevice, createInfo)
        , m_pDummyPhysicalDevice(pDummyPhysicalDevice)
        , m_pDummyQueue(nullptr)
        , m_pDummyCommandPool(nullptr)
    {
        F_Assert(m_pDummyPhysicalDevice && "RHIDummyDevice::RHIDummyDevice")
    }

    RHIDummyDevice::~RHIDummyDevice()
    {
        
    }
    
    void RHIDummyDevice::Destroy()
    {
        F_DELETE(m_pDummyQueue)
        F_DELETE(m_pDummyCommandPool)
    }

    uint32 RHIDummyDevice::GetQueueCount(RHIQueueType eQueue)
    {
        return eQueue == RHIQueueType::RHI_Queue_Graphics ? 1 : 0;
    }

    RHIQueue* RHIDummyDevice::GetQueue(RHIQueueType eQueue)
    {
        F_Assert("RHIDummyDevice::GetQueue" && eQueue == RHIQueueType::RHI_Queue_Graphics);
        if (m_pDummyQueue == nullptr)
        {
            m_pDummyQueue = new RHIDummyQueue(this);
        }
        return m_pDummyQueue;
    }

    RHICommandPool* RHIDummyDevice::GetCommandPool(RHIQueueType eQueue)
    {
        F_Assert("RHIDummyDevice::GetCommandPool" && eQueue == RHIQueueType::RHI_Queue_Graphics);
        if (m_pDummyCommandPool == nullptr)
        {
            m_pDummyCommandPool = new RHIDummyCommandPool(this);
        }
        return m_pDummyCommandPool;
        
    }

    RHISurface* RHIDummyDevice::CreateSurface(const RHISurfaceCreateInfo& createInfo)
    {
        return new RHIDummySurface(this, createInfo);
    }

    RHISwapChain* RHIDummyDevice::CreateSwapChain(const RHISwapChainCreateInfo& createInfo)
    {
        return new RHIDummySwapChain(this, createInfo);
    }

    RHIBuffer* RHIDummyDevice::CreateBuffer(const RHIBufferCreateInfo& createInfo)
    {
        return new RHIDummyBuffer(this, createInfo);
    }

    RHITexture* RHIDummyDevice::CreateTexture(const RHITextureCreateInfo& createInfo)
    {
        return new RHIDummyTexture(this, createInfo);
    }

    RHISampler* RHIDummyDevice::CreateSampler(const RHISamplerCreateInfo& createInfo)
    {
        return new RHIDummySampler(this, createInfo);
    }

    RHIBindGroupLayout* RHIDummyDevice::CreateBindGroupLayout(const RHIBindGroupLayoutCreateInfo& createInfo)
    {
        return new RHIDummyBindGroupLayout(this, createInfo);
    }

    RHIBindGroupPool* RHIDummyDevice::CreateBindGroupPool(const RHIBindGroupPoolCreateInfo& createInfo)
    {
        return new RHIDummyBindGroupPool(this, createInfo);
    }

    RHIBindGroupCache* RHIDummyDevice::CreateBindGroupCache(const RHIBindGroupCacheCreateInfo& createInfo)
    {
        return new RHIDummyBindGroupCache(this, createInfo);
    }

    RHIBindGroup* RHIDummyDevice::CreateBindGroup(const RHIBindGroupCreateInfo& createInfo)
    {
        return new RHIDummyBindGroup(this, createInfo);
    }

    RHIShaderModule* RHIDummyDevice::CreateShaderModule(const RHIShaderModuleCreateInfo& createInfo)
    {
        return new RHIDummyShaderModule(this, createInfo);
    }

    RHIPipelineLayout* RHIDummyDevice::CreatePipelineLayout(const RHIPipelineLayoutCreateInfo& createInfo)
    {
        return new RHIDummyPipelineLayout(this, createInfo);
    }

    RHIPipelineCache* RHIDummyDevice::CreatePipelineCache(const RHIPipelineCacheCreateInfo& createInfo)
    {
        return new RHIDummyPipelineCache(this, createInfo);
    }

    RHIPipelineCompute* RHIDummyDevice::CreatePipelineCompute(const RHIPipelineComputeCreateInfo& createInfo)
    {
        return new RHIDummyPipelineCompute(this, createInfo);
    }

    RHIPipelineGraphics* RHIDummyDevice::CreatePipelineGraphics(const RHIPipelineGraphicsCreateInfo& createInfo)
    {
        return new RHIDummyPipelineGraphics(this, createInfo);
    }

    RHIRenderPassCache* RHIDummyDevice::CreateRenderPassCache(const RHIRenderPassCacheCreateInfo& createInfo)
    {
        return new RHIDummyRenderPassCache(this, createInfo);
    }

    RHIRenderPass* RHIDummyDevice::CreateRenderPass(const RHIRenderPassCreateInfo& createInfo)
    {
        return new RHIDummyRenderPass(this, createInfo);
    }

    RHIFrameBuffer* RHIDummyDevice::CreateFrameBuffer(const RHIFrameBufferCreateInfo& createInfo)
    {
        return new RHIDummyFrameBuffer(this, createInfo);
    }

    RHIFence* RHIDummyDevice::CreateFence(const RHIFenceCreateInfo& createInfo)
    {
        return new RHIDummyFence(this, createInfo);
    }

    RHISemaphore* RHIDummyDevice::CreateSemaphore(const RHISemaphoreCreateInfo& createInfo)
    {
        return new RHIDummySemaphore(this, createInfo);
    }   

    RHICommandPool* RHIDummyDevice::CreateCommandPool()
    {
        return new RHIDummyCommandPool(this);
    }

    RHICommandBuffer* RHIDummyDevice::CreateCommandBuffer()
    {
        return new RHIDummyCommandBuffer(this);
    }

    bool RHIDummyDevice::CheckSwapChainFormatSupport(RHISurface* pSurface, RHIPixelFormatType ePixelFormat)
    {
        return true;
    }
    bool RHIDummyDevice::IsPixelFormatSupported(RHIPixelFormatType ePixelFormat)
    {
        return true;
    }

}; //LostPeterPluginRHIDummy