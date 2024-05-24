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

#include "../include/RHIVulkanQueue.h"
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanQueue::RHIVulkanQueue(RHIVulkanDevice* pVulkanDevice, const RHIQueueCreateInfo& createInfo)
        : RHIQueue(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)
        , m_vkQueue(VK_NULL_HANDLE)
        , m_nFamilyIndex(createInfo.nFamilyIndex)
        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanQueue::RHIVulkanQueue")

        createVkQueue();
    }

    RHIVulkanQueue::~RHIVulkanQueue()
    {
        
    }

    void RHIVulkanQueue::Destroy()
    {
        m_vkQueue = VK_NULL_HANDLE;
    }

    void RHIVulkanQueue::createVkQueue()
    {
        m_vkQueue = m_pVulkanDevice->GetVkQueue(m_nFamilyIndex, 0);
        if (m_vkQueue == VK_NULL_HANDLE)
        {
            F_LogError("*********************** RHIVulkanQueue::createVkQueue: GetVkQueue failed, Name: [%s], nFamilyIndex: [%u] !", m_strName.c_str(), m_nFamilyIndex);
            return;
        }

        if (RHI_IsDebug())
        {
            if (m_strDebugName.empty())
                m_strDebugName = m_strName;
            m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_QUEUE, reinterpret_cast<uint64_t>(m_vkQueue), m_strDebugName.c_str());
        }
        F_LogInfo("RHIVulkanQueue::createVkQueue: Create VkQueue success, Name: [%s], nFamilyIndex: [%u] !", m_strDebugName.c_str(), m_nFamilyIndex);
    }

    void RHIVulkanQueue::Submit(RHICommandBuffer* pCommandBuffer, RHIFence* pFenceToSignal)
    {

    }

    void RHIVulkanQueue::Wait(RHIFence* pFenceToSignal)
    {

    }
    
}; //LostPeterPluginRHIVulkan