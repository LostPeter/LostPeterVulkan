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
    RHIVulkanCommandPool::RHIVulkanCommandPool(RHIVulkanDevice* pVulkanDevice)
        : RHICommandPool(pVulkanDevice)
        , m_pVulkanDevice(pVulkanDevice)
        , m_nFamilyIndex(-1)
        , m_vkCommandPool(VK_NULL_HANDLE)
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
        return true;
    }

}; //LostPeterPluginRHIVulkan