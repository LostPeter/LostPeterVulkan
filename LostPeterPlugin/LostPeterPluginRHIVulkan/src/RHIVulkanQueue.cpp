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
    uint32 RHIVulkanQueue::ms_nID = 0;
    RHIVulkanQueue::RHIVulkanQueue(RHIVulkanDevice* pVulkanDevice, uint32 nFamilyIndex)
        : RHIQueue(pVulkanDevice)
        , m_pVulkanDevice(pVulkanDevice)
        , m_nFamilyIndex(nFamilyIndex)
        , m_vkQueue(VK_NULL_HANDLE)
        , m_nID(++ms_nID)
        , m_strDebugName("")
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanQueue::RHIVulkanQueue")
    }

    RHIVulkanQueue::~RHIVulkanQueue()
    {
        
    }

    void RHIVulkanQueue::Destroy()
    {
        m_vkQueue = VK_NULL_HANDLE;
    }

    bool RHIVulkanQueue::Init()
    {
        m_vkQueue = m_pVulkanDevice->GetVkQueue(m_nFamilyIndex, 0);
        if (m_vkQueue == VK_NULL_HANDLE)
        {
            F_LogError("*********************** RHIVulkanQueue::Init: GetVkQueue failed !");
            return false;
        }

        if (RHI_IsDebug())
        {
            m_strDebugName = "VulkanQueue-" + FUtilString::SaveUInt(m_nFamilyIndex) + "-" + FUtilString::SaveUInt(m_nID);
            m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_QUEUE, reinterpret_cast<uint64_t>(m_vkQueue), m_strDebugName.c_str());
        }
        F_LogInfo("RHIVulkanQueue::Init: Create Queue success, nFamilyIndex: [%u], ID: [%u] !", m_nFamilyIndex, m_nID);
        return true;
    }

    void RHIVulkanQueue::Submit(RHICommandBuffer* pCommandBuffer, RHIFence* pFenceToSignal)
    {

    }

    void RHIVulkanQueue::Wait(RHIFence* pFenceToSignal)
    {

    }
    
}; //LostPeterPluginRHIVulkan