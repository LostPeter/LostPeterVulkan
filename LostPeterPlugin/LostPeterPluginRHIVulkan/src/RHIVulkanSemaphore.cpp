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
    RHIVulkanSemaphore::RHIVulkanSemaphore(RHIVulkanDevice* pVulkanDevice)
        : m_pVulkanDevice(pVulkanDevice)
        , m_vkSemaphore(VK_NULL_HANDLE)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanSemaphore::RHIVulkanSemaphore")

        m_vkSemaphore = m_pVulkanDevice->CreateVkSemaphore();
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

    
}; //LostPeterPluginRHIVulkan