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
#include "../include/RHIDummyCommandBuffer.h"
#include "../include/RHIDummyFence.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyDevice::RHIDummyDevice(const RHIDeviceCreateInfo& createInfo)
        : RHIDevice(createInfo)
        , m_pQueue(nullptr)
    {

    }

    RHIDummyDevice::~RHIDummyDevice()
    {
        
    }
    
    void RHIDummyDevice::Destroy()
    {
        F_DELETE(m_pQueue)
    }

    uint32 RHIDummyDevice::GetQueueCount(RHIQueueType eQueue)
    {
        return eQueue == RHIQueueType::RHI_Queue_Graphics ? 1 : 0;
    }

    RHIQueue* RHIDummyDevice::GetQueue(RHIQueueType eQueue)
    {
        F_Assert("RHIDummyDevice::GetQueue" && eQueue == RHIQueueType::RHI_Queue_Graphics);
        if (m_pQueue == nullptr)
        {
            m_pQueue = new RHIDummyQueue();
        }
        return m_pQueue;
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
        return new RHIDummyBuffer(createInfo);
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

    RHICommandBuffer* RHIDummyDevice::CreateCommandBuffer()
    {
        return new RHIDummyCommandBuffer();
    }

    RHIFence* RHIDummyDevice::CreateFence()
    {
        return new RHIDummyFence(*this);
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