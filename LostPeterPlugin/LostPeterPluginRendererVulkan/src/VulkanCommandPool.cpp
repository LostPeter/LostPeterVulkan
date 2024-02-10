/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-10
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanCommandPool.h"
#include "../include/VulkanDevice.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanCommandPool::VulkanCommandPool(const String& strNameDescriptorPool, VulkanDevice* pDevice)
        : Base(strNameDescriptorPool)
        , m_pDevice(pDevice)
        , m_vkCommandPool(VK_NULL_HANDLE)
    {
        F_Assert(m_pDevice && "VulkanCommandPool::VulkanCommandPool")
    }

    VulkanCommandPool::~VulkanCommandPool()
    {
        Destroy();
    }

    void VulkanCommandPool::Destroy()
    {
        if (this->m_vkCommandPool != VK_NULL_HANDLE)
        {
            m_pDevice->DestroyVkCommandPool(this->m_vkCommandPool);
        }
        this->m_vkCommandPool = VK_NULL_HANDLE;
    }

    bool VulkanCommandPool::Init(VkCommandPoolCreateFlags flags,
                                 uint32_t queueFamilyIndex)
    {
        Destroy();
        if (!m_pDevice->CreateVkCommandPool(flags,
                                            queueFamilyIndex,
                                            this->m_vkCommandPool))
        {
            F_LogError("*********************** VulkanCommandPool::Init: Failed to CreateVkCommandPool, nameCommandPool: [%s] !", GetName().c_str());
            return false;
        }

        return true;
    }

}; //LostPeterPluginRendererVulkan