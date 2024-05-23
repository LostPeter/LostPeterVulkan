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

#include "../include/RHIVulkanSemaphore.h"
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanSemaphore::RHIVulkanSemaphore(RHIVulkanDevice* pVulkanDevice, const RHISemaphoreCreateInfo& createInfo)
        : RHISemaphore(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)
        , m_vkSemaphore(VK_NULL_HANDLE)
        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanSemaphore::RHIVulkanSemaphore")

        createVkSemaphore();
    }   

    RHIVulkanSemaphore::~RHIVulkanSemaphore()
    {
        Destroy();
    }

    void RHIVulkanSemaphore::Destroy()
    {
        if (m_vkSemaphore != VK_NULL_HANDLE)
        {
            m_pVulkanDevice->DestroyVkSemaphore(m_vkSemaphore);
        }
        m_vkSemaphore = VK_NULL_HANDLE;
    }

    void RHIVulkanSemaphore::createVkSemaphore()
    {
        if (!m_pVulkanDevice->CreateVkSemaphore(m_vkSemaphore))
        {
            F_LogError("*********************** RHIVulkanSemaphore::createVkSemaphore: CreateVkSemaphore failed, name: [%s] !", m_strDebugName.c_str());
        }

        if (RHI_IsDebug())
        {
            if (!m_strDebugName.empty())
            {
                m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_SEMAPHORE, reinterpret_cast<uint64_t>(m_vkSemaphore), m_strDebugName.c_str());
            }
        }
    }
    
}; //LostPeterPluginRHIVulkan