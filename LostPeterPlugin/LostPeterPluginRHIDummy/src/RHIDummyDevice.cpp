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
#include "../include/RHIDummyBindGroup.h"
#include "../include/RHIDummyPipelineLayout.h"
#include "../include/RHIDummyShaderModule.h"
#include "../include/RHIDummyPipelineCompute.h"
#include "../include/RHIDummyPipelineGraphics.h"
#include "../include/RHIDummyCommandPool.h"
#include "../include/RHIDummyCommandBuffer.h"
#include "../include/RHIDummyFence.h"

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
        return new RHIDummySurface(createInfo);
    }

    RHISwapChain* RHIDummyDevice::CreateSwapChain(const RHISwapChainCreateInfo& createInfo)
    {
        return new RHIDummySwapChain(createInfo);
    }

    RHIBuffer* RHIDummyDevice::CreateBuffer(const RHIBufferCreateInfo& createInfo)
    {
        return new RHIDummyBuffer(this, createInfo);
    }

    RHITexture* RHIDummyDevice::CreateTexture(const RHITextureCreateInfo& createInfo)
    {
        return new RHIDummyTexture(createInfo);
    }

    RHISampler* RHIDummyDevice::CreateSampler(const RHISamplerCreateInfo& createInfo)
    {
        return new RHIDummySampler(createInfo);
    }

    RHIBindGroupLayout* RHIDummyDevice::CreateBindGroupLayout(const RHIBindGroupLayoutCreateInfo& createInfo)
    {
        return new RHIDummyBindGroupLayout(createInfo);
    }

    RHIBindGroup* RHIDummyDevice::CreateBindGroup(const RHIBindGroupCreateInfo& createInfo)
    {
        return new RHIDummyBindGroup(createInfo);
    }

    RHIPipelineLayout* RHIDummyDevice::CreatePipelineLayout(const RHIPipelineLayoutCreateInfo& createInfo)
    {
        return new RHIDummyPipelineLayout(createInfo);
    }

    RHIShaderModule* RHIDummyDevice::CreateShaderModule(const RHIShaderModuleCreateInfo& createInfo)
    {
        return new RHIDummyShaderModule(createInfo);
    }

    RHIPipelineCompute* RHIDummyDevice::CreatePipelineCompute(const RHIPipelineComputeCreateInfo& createInfo)
    {
        return new RHIDummyPipelineCompute(createInfo);
    }

    RHIPipelineGraphics* RHIDummyDevice::CreatePipelineGraphics(const RHIPipelineGraphicsCreateInfo& createInfo)
    {
        return new RHIDummyPipelineGraphics(createInfo);
    }

    RHICommandPool* RHIDummyDevice::CreateCommandPool()
    {
        return new RHIDummyCommandPool(this);
    }

    RHICommandBuffer* RHIDummyDevice::CreateCommandBuffer()
    {
        return new RHIDummyCommandBuffer(this);
    }

    RHIFence* RHIDummyDevice::CreateFence()
    {
        return new RHIDummyFence(this, false);
    }
    RHIFence* RHIDummyDevice::CreateFence(bool bIsSignaled)
    {
        return new RHIDummyFence(this, bIsSignaled);
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