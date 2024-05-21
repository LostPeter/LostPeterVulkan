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
    uint32 RHIVulkanCommandPool::ms_nID = 0;
    RHIVulkanCommandPool::RHIVulkanCommandPool(RHIVulkanDevice* pVulkanDevice)
        : RHICommandPool(pVulkanDevice)
        , m_pVulkanDevice(pVulkanDevice)
        , m_nFamilyIndex(-1)
        , m_vkCommandPool(VK_NULL_HANDLE)
        , m_nID(++ms_nID)
        , m_strDebugName("")
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanCommandPool::RHIVulkanCommandPool")
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
    
    bool RHIVulkanCommandPool::Init(uint32 nFamilyIndex)
    {
        m_nFamilyIndex = nFamilyIndex;
        m_vkCommandPool = m_pVulkanDevice->CreateVkCommandPool(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, (uint32_t)nFamilyIndex);
        if (m_vkCommandPool == VK_NULL_HANDLE)
        {
            F_LogError("*********************** RHIVulkanCommandPool::Init: CreateVkCommandPool failed !");
            return false;
        }

        if (RHI_IsDebug())
        {
            m_strDebugName = "VulkanCommandPool-" + FUtilString::SaveUInt(m_nFamilyIndex) + "-" + FUtilString::SaveUInt(m_nID);
            m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_COMMAND_POOL, reinterpret_cast<uint64_t>(m_vkCommandPool), m_strDebugName.c_str());
        }
        F_LogInfo("RHIVulkanCommandPool::Init: Create CommandPool success, nFamilyIndex: [%u], ID: [%u] !", m_nFamilyIndex, m_nID);
        return true;
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

}; //LostPeterPluginRHIVulkan