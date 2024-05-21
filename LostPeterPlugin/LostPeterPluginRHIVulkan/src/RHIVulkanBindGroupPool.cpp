/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIVulkanBindGroupPool.h"
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanBindGroupPool::RHIVulkanBindGroupPool(RHIVulkanDevice* pVulkanDevice, const RHIBindGroupPoolCreateInfo& createInfo)
        : RHIBindGroupPool(pVulkanDevice, createInfo)
        , m_pVulkanDevice(pVulkanDevice)
        , m_vkDescriptorPool(VK_NULL_HANDLE)
        , m_nDescriptorCount(createInfo.nPoolCount)
        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanBindGroupPool::RHIVulkanBindGroupPool")

        createVkDescriptorPool();
    }

    RHIVulkanBindGroupPool::~RHIVulkanBindGroupPool()
    {
        Destroy();
    }

    void RHIVulkanBindGroupPool::Destroy()
    {
        if (m_vkDescriptorPool != VK_NULL_HANDLE)
        {
            m_pVulkanDevice->DestroyVkDescriptorPool(m_vkDescriptorPool);
        }
        m_vkDescriptorPool = VK_NULL_HANDLE;
    }
    
    void RHIVulkanBindGroupPool::createVkDescriptorPool()
    {   
        if (!m_pVulkanDevice->CreateVkDescriptorPool(m_nDescriptorCount,
                                                     m_vkDescriptorPool))
        {
            F_LogError("*********************** RHIVulkanBindGroupPool::createVkDescriptorPool: CreateVkDescriptorPool failed, name: [%s] !", m_strDebugName.c_str());
        }

        if (RHI_IsDebug())
        {
            if (!m_strDebugName.empty())
            {
                m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_DESCRIPTOR_POOL, reinterpret_cast<uint64_t>(m_vkDescriptorPool), m_strDebugName.c_str());
            }
        }
    }

}; //LostPeterPluginRHIVulkan