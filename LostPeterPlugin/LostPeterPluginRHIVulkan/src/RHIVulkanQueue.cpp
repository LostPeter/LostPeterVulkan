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
    RHIVulkanQueue::RHIVulkanQueue(RHIVulkanDevice* pVulkanDevice, uint32 nFamilyIndex)
        : RHIQueue(pVulkanDevice)
        , m_pVulkanDevice(pVulkanDevice)
        , m_nFamilyIndex(nFamilyIndex)
        , m_vkQueue(VK_NULL_HANDLE)
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
        return true;
    }

    void RHIVulkanQueue::Submit(RHICommandBuffer* pCommandBuffer, RHIFence* pFenceToSignal)
    {

    }

    void RHIVulkanQueue::Wait(RHIFence* pFenceToSignal)
    {

    }
    
}; //LostPeterPluginRHIVulkan