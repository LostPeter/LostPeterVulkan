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

#include "../include/RHIVulkanDevice.h"
#include "../include/RHIVulkanPhysicalDevice.h"
#include "../include/RHIVulkanQueue.h"
#include "../include/RHIVulkanSurface.h"
#include "../include/RHIVulkanSwapChain.h"
#include "../include/RHIVulkanBuffer.h"
#include "../include/RHIVulkanTexture.h"
#include "../include/RHIVulkanSampler.h"
#include "../include/RHIVulkanBindGroupLayout.h"
#include "../include/RHIVulkanBindGroup.h"
#include "../include/RHIVulkanPipelineLayout.h"
#include "../include/RHIVulkanShaderModule.h"
#include "../include/RHIVulkanPipelineCompute.h"
#include "../include/RHIVulkanPipelineGraphics.h"
#include "../include/RHIVulkanCommandBuffer.h"
#include "../include/RHIVulkanFence.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanDevice::RHIVulkanDevice(RHIVulkanPhysicalDevice* pPhysicalDevice, const RHIDeviceCreateInfo& createInfo)
        : RHIDevice(createInfo)
        , m_pPhysicalDevice(pPhysicalDevice)
        , m_pQueue(nullptr)
    {
        F_Assert(m_pPhysicalDevice && "RHIVulkanDevice::RHIVulkanDevice")
    }

    RHIVulkanDevice::~RHIVulkanDevice()
    {

    }

    void RHIVulkanDevice::Destroy()
    {
        F_DELETE(m_pQueue)
        m_pPhysicalDevice = nullptr;
    }

    uint32 RHIVulkanDevice::GetQueueCount(RHIQueueType eQueue)
    {
        return eQueue == RHIQueueType::RHI_Queue_Graphics ? 1 : 0;
    }

    RHIQueue* RHIVulkanDevice::GetQueue(RHIQueueType eQueue, uint32 nIndex)
    {
        F_Assert("RHIVulkanDevice::GetQueue" && eQueue == RHIQueueType::RHI_Queue_Graphics && nIndex == 0);
        if (m_pQueue == nullptr)
        {
            m_pQueue = new RHIVulkanQueue();
        }
        return m_pQueue;
    }

    RHISurface* RHIVulkanDevice::CreateSurface(const RHISurfaceCreateInfo& createInfo)
    {
        return new RHIVulkanSurface(createInfo);
    }

    RHISwapChain* RHIVulkanDevice::CreateSwapChain(const RHISwapChainCreateInfo& createInfo)
    {
        return new RHIVulkanSwapChain(createInfo);
    }

    RHIBuffer* RHIVulkanDevice::CreateBuffer(const RHIBufferCreateInfo& createInfo)
    {
        return new RHIVulkanBuffer(createInfo);
    }

    RHITexture* RHIVulkanDevice::CreateTexture(const RHITextureCreateInfo& createInfo)
    {
        return new RHIVulkanTexture(createInfo);
    }

    RHISampler* RHIVulkanDevice::CreateSampler(const RHISamplerCreateInfo& createInfo)
    {
        return new RHIVulkanSampler(createInfo);
    }

    RHIBindGroupLayout* RHIVulkanDevice::CreateBindGroupLayout(const RHIBindGroupLayoutCreateInfo& createInfo)
    {
        return new RHIVulkanBindGroupLayout(createInfo);
    }

    RHIBindGroup* RHIVulkanDevice::CreateBindGroup(const RHIBindGroupCreateInfo& createInfo)
    {
        return new RHIVulkanBindGroup(createInfo);
    }

    RHIPipelineLayout* RHIVulkanDevice::CreatePipelineLayout(const RHIPipelineLayoutCreateInfo& createInfo)
    {
        return new RHIVulkanPipelineLayout(createInfo);
    }

    RHIShaderModule* RHIVulkanDevice::CreateShaderModule(const RHIShaderModuleCreateInfo& createInfo)
    {
        return new RHIVulkanShaderModule(createInfo);
    }

    RHIPipelineCompute* RHIVulkanDevice::CreatePipelineCompute(const RHIPipelineComputeCreateInfo& createInfo)
    {
        return new RHIVulkanPipelineCompute(createInfo);
    }

    RHIPipelineGraphics* RHIVulkanDevice::CreatePipelineGraphics(const RHIPipelineGraphicsCreateInfo& createInfo)
    {
        return new RHIVulkanPipelineGraphics(createInfo);
    }

    RHICommandBuffer* RHIVulkanDevice::CreateCommandBuffer()
    {
        return new RHIVulkanCommandBuffer();
    }

    RHIFence* RHIVulkanDevice::CreateFence()
    {
        return new RHIVulkanFence(*this);
    }

    bool RHIVulkanDevice::CheckSwapChainFormatSupport(RHISurface* pSurface, RHIPixelFormatType ePixelFormat)
    {
        return true;
    }
    
}; //LostPeterPluginRHIVulkan