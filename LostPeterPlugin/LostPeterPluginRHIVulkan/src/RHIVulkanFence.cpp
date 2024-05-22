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
    RHIVulkanFence::RHIVulkanFence(RHIVulkanDevice* pVulkanDevice, const RHIFenceCreateInfo& createInfo)
        : RHIFence(pVulkanDevice, createInfo)
        , m_pVulkanDevice(pVulkanDevice)
        , m_vkFence(VK_NULL_HANDLE)
        , m_bIsSignaled(createInfo.bIsSignaled)
        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanFence::RHIVulkanFence")

        createVkFence();
    }   

    RHIVulkanFence::~RHIVulkanFence()
    {
        Destroy();
    }

    void RHIVulkanFence::Destroy()
    {
        if (m_vkFence != VK_NULL_HANDLE)
        {
            m_pVulkanDevice->DestroyVkFence(m_vkFence);
        }
        m_vkFence = VK_NULL_HANDLE;
    }

    RHIFenceStatusType RHIVulkanFence::GetStatus()
    {
        return m_bIsSignaled ? RHIFenceStatusType::RHI_FenceStatus_Signaled : RHIFenceStatusType::RHI_FenceStatus_NotReady;
    }

    void RHIVulkanFence::Reset()
    {
        m_pVulkanDevice->ResetVkFence(m_vkFence);
        m_bIsSignaled = false;
    }

    void RHIVulkanFence::Wait()
    {
        if (m_bIsSignaled) 
            return;
        
        m_pVulkanDevice->WaitVkFence(m_vkFence);
    }

    void RHIVulkanFence::createVkFence()
    {
        if (!m_pVulkanDevice->CreateVkFence(m_bIsSignaled, m_vkFence))
        {
            F_LogError("*********************** RHIVulkanFence::createVkFence: CreateVkFence failed, name: [%s] !", m_strDebugName.c_str());
        }

        if (RHI_IsDebug())
        {
            if (!m_strDebugName.empty())
            {
                m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_FENCE, reinterpret_cast<uint64_t>(m_vkFence), m_strDebugName.c_str());
            }
        }
    }
    
}; //LostPeterPluginRHIVulkan