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

#include "../include/PreInclude.h"
#include "../include/VulkanFence.h"
#include "../include/VulkanFenceManager.h"
#include "../include/VulkanDevice.h"

namespace LostPeter
{
    VulkanFence::VulkanFence(VulkanDevice* pDevice, VulkanFenceManager* pFenceManager, bool bCreateSignaled)
        : m_vkFence(VK_NULL_HANDLE)
        , m_eFenceState(bCreateSignaled ? Vulkan_FenceState_Signaled : Vulkan_FenceState_NotReady)
        , m_pFenceManager(pFenceManager)
    {
        VkFenceCreateInfo createInfo;
        Util_ZeroStruct(createInfo, VK_STRUCTURE_TYPE_FENCE_CREATE_INFO);
        createInfo.flags = bCreateSignaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
        vkCreateFence(pDevice->GetVkDevice(), &createInfo, UTIL_CPU_ALLOCATOR, &m_vkFence);
    }

    VulkanFence::~VulkanFence()
    {
        Destroy();
    }

    void VulkanFence::Destroy()
    {
        if (m_vkFence != VK_NULL_HANDLE)
        {
            vkDestroyFence(m_pFenceManager->GetDevice()->GetVkDevice(), m_vkFence, UTIL_CPU_ALLOCATOR);
        }
        m_vkFence = VK_NULL_HANDLE;
    }

}; //LostPeter