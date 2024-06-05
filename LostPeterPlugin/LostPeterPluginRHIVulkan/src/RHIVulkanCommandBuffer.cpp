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
#include "../include/RHIVulkanConverter.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanCommandBuffer::RHIVulkanCommandBuffer(RHIVulkanDevice* pVulkanDevice, RHIVulkanCommandPool* pVulkanCommandPool, const RHICommandBufferCreateInfo& createInfo)
        : RHICommandBuffer(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)
        , m_pVulkanCommandPool(pVulkanCommandPool)
        , m_vkCommandBuffer(VK_NULL_HANDLE)
        , m_eQueue(createInfo.eQueue)
        , m_eCommandBufferLevel(createInfo.eCommandBufferLevel)
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
        m_vkCommandBufferLevel = RHIVulkanConverter::TransformToVkCommandBufferLevel(m_eCommandBufferLevel);

        if (!m_pVulkanDevice->AllocateVkCommandBuffer(m_pVulkanCommandPool->GetVkCommandPool(),
                                                      m_vkCommandBufferLevel,
                                                      m_vkCommandBuffer))
        {
            setIsError(true);
            F_LogError("*********************** RHIVulkanCommandBuffer::createVkCommandBuffer: AllocateVkCommandBuffer failed, Name: [%s] !", m_strName.c_str());
            return;
        }

        if (RHI_IsDebug())
        {
            if (m_strDebugName.empty())
                m_strDebugName = m_strName;
            m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_COMMAND_BUFFER, reinterpret_cast<uint64_t>(m_vkCommandBuffer), m_strDebugName.c_str());
            F_LogInfo("RHIVulkanCommandBuffer::createVkCommandBuffer: Create VkCommandBuffer success, Name: [%s] !", m_strDebugName.c_str());
        }
    }

}; //LostPeterPluginRHIVulkan