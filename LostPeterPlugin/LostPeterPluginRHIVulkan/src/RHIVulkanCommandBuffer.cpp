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

#include "../include/RHIVulkanCommandBuffer.h"
#include "../include/RHIVulkanDevice.h"
#include "../include/RHIVulkanCommandPool.h"
#include "../include/RHIVulkanCommandEncoder.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanCommandBuffer::RHIVulkanCommandBuffer(RHIVulkanDevice* pVulkanDevice, RHIVulkanCommandPool* pVulkanCommandPool, const RHICommandBufferCreateInfo& createInfo)
        : RHICommandBuffer(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)
        , m_pVulkanCommandPool(pVulkanCommandPool)
        , m_vkCommandBuffer(VK_NULL_HANDLE)
    {
        F_Assert(m_pVulkanDevice && m_pVulkanCommandPool && "RHIVulkanCommandBuffer::RHIVulkanCommandBuffer")

        createVkCommandBuffer();
    }

    RHIVulkanCommandBuffer::~RHIVulkanCommandBuffer()
    {
        Destroy();
    }

    void RHIVulkanCommandBuffer::Destroy()
    {
        if (m_vkCommandBuffer != VK_NULL_HANDLE)
        {
            m_pVulkanDevice->FreeVkCommandBuffers(m_pVulkanCommandPool->GetVkCommandPool(), 1, &m_vkCommandBuffer);
        }
        m_vkCommandBuffer = VK_NULL_HANDLE;
    }

    RHICommandEncoder* RHIVulkanCommandBuffer::Begin()
    {
        return new RHIVulkanCommandEncoder(this);
    }
    
    void RHIVulkanCommandBuffer::createVkCommandBuffer()
    {
        
    }

}; //LostPeterPluginRHIVulkan