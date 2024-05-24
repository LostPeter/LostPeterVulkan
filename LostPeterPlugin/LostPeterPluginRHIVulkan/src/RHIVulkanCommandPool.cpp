/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-18
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIVulkanCommandPool.h"
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanCommandPool::RHIVulkanCommandPool(RHIVulkanDevice* pVulkanDevice, const RHICommandPoolCreateInfo& createInfo)
        : RHICommandPool(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)
        , m_vkCommandPool(VK_NULL_HANDLE)
        , m_nFamilyIndex(createInfo.nFamilyIndex)
        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanCommandPool::RHIVulkanCommandPool")

        createVkCommandPool();
    }

    RHIVulkanCommandPool::~RHIVulkanCommandPool()
    {
        Destroy();
    }  
    
    void RHIVulkanCommandPool::Destroy()
    {
        if (m_vkCommandPool != VK_NULL_HANDLE)
        {
            m_pVulkanDevice->DestroyVkCommandPool(m_vkCommandPool);
        }
        m_vkCommandPool = VK_NULL_HANDLE;
    }

    bool RHIVulkanCommandPool::AllocateCommandBuffers(VkCommandBufferLevel level, const void* pNext, uint32_t commandBufferCount, VkCommandBuffer* pCommandBuffers)
    {
        if (m_vkCommandPool != VK_NULL_HANDLE)
        {
            return m_pVulkanDevice->AllocateVkCommandBuffers(m_vkCommandPool, level, pNext, commandBufferCount, pCommandBuffers);
        }
        return false;
    }

    void RHIVulkanCommandPool::FreeCommandBuffers(uint32_t commandBufferCount, VkCommandBuffer* pCommandBuffers)
    {
        if (m_vkCommandPool != VK_NULL_HANDLE)
        {
            m_pVulkanDevice->FreeVkCommandBuffers(m_vkCommandPool, commandBufferCount, pCommandBuffers);
        }
    }

    void RHIVulkanCommandPool::createVkCommandPool()
    {
        if (!m_pVulkanDevice->CreateVkCommandPool(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, 
                                                  (uint32_t)m_nFamilyIndex, 
                                                  m_vkCommandPool))
        {
            F_LogError("*********************** RHIVulkanCommandPool::createVkCommandPool: CreateVkCommandPool failed, Name: [%s] !", m_strName.c_str());
            return;
        }

        if (RHI_IsDebug())
        {
            if (m_strDebugName.empty())
                m_strDebugName = m_strName;
            m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_COMMAND_POOL, reinterpret_cast<uint64_t>(m_vkCommandPool), m_strDebugName.c_str());
        }
        F_LogInfo("RHIVulkanCommandPool::createVkCommandPool: Create VkCommandPool success, Name: [%s] , nFamilyIndex: [%u] !", m_strDebugName.c_str(), m_nFamilyIndex);
    }

}; //LostPeterPluginRHIVulkan