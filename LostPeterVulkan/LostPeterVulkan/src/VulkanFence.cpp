/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-05-19
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanFence.h"
#include "../include/VulkanFenceManager.h"
#include "../include/VulkanDevice.h"

namespace LostPeter
{
    VulkanFence::VulkanFence(VulkanFenceManager* pFenceManager, bool bCreateSignaled)
        : m_pFenceManager(pFenceManager)
        , m_vkFence(VK_NULL_HANDLE)
        , m_eFenceState(bCreateSignaled ? Vulkan_FenceState_Signaled : Vulkan_FenceState_NotReady)
    {
        assert(m_pFenceManager != nullptr && "VulkanFence::VulkanFence");
        m_vkFence = m_pFenceManager->GetDevice()->CreateVkFence(bCreateSignaled);
        assert(m_vkFence != VK_NULL_HANDLE && "VulkanFence::VulkanFence");
    }

    VulkanFence::~VulkanFence()
    {
        Destroy();
    }

    void VulkanFence::Destroy()
    {
        m_pFenceManager->GetDevice()->DestroyVkFence(m_vkFence);
        m_vkFence = VK_NULL_HANDLE;
    }

}; //LostPeter