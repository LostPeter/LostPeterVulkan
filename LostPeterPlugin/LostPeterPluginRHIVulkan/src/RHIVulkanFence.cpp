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

#include "../include/RHIVulkanFence.h"
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanFence::RHIVulkanFence(RHIVulkanDevice* pDevice, bool bIsSignaled)
        : m_pDevice(pDevice)
        , m_vkFence(VK_NULL_HANDLE)
        , m_bIsSignaled(bIsSignaled)
    {
        F_Assert(m_pDevice && "RHIVulkanBuffer::RHIVulkanBuffer")

        m_pDevice->CreateVkFence(bIsSignaled, m_vkFence);
    }   

    RHIVulkanFence::~RHIVulkanFence()
    {
        Destroy();
    }

    void RHIVulkanFence::Destroy()
    {
        if (m_vkFence != VK_NULL_HANDLE)
        {
            m_pDevice->DestroyVkFence(m_vkFence);
        }
        m_vkFence = VK_NULL_HANDLE;
    }

    RHIFenceStatusType RHIVulkanFence::GetStatus()
    {
        return m_bIsSignaled ? RHIFenceStatusType::RHI_FenceStatus_Signaled : RHIFenceStatusType::RHI_FenceStatus_NotReady;
    }

    void RHIVulkanFence::Reset()
    {
        m_pDevice->ResetVkFence(m_vkFence);
        m_bIsSignaled = false;
    }

    void RHIVulkanFence::Wait()
    {
        if (m_bIsSignaled) 
            return;
        
        m_pDevice->WaitVkFence(m_vkFence);
    }
    
}; //LostPeterPluginRHIVulkan