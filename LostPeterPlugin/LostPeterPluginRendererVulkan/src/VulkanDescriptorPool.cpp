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

#include "../include/VulkanDescriptorPool.h"
#include "../include/VulkanDevice.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanDescriptorPool::VulkanDescriptorPool(const String& strNameDescriptorPool, VulkanDevice* pDevice)
        : Base(strNameDescriptorPool)
        , m_pDevice(pDevice)
        , m_vkDescriptorPool(VK_NULL_HANDLE)
    {
        F_Assert(m_pDevice && "VulkanDescriptorPool::VulkanDescriptorPool")
    }

    VulkanDescriptorPool::~VulkanDescriptorPool()
    {
        Destroy();
    }

    void VulkanDescriptorPool::Destroy()
    {
        if (this->m_vkDescriptorPool != VK_NULL_HANDLE)
        {
            m_pDevice->DestroyVkDescriptorPool(this->m_vkDescriptorPool);
        }
        this->m_vkDescriptorPool = VK_NULL_HANDLE;
    }

    bool VulkanDescriptorPool::Init(uint32_t descriptorCount)
    {
        Destroy();
        if (!m_pDevice->CreateVkDescriptorPool(descriptorCount,
                                               m_vkDescriptorPool))
        {
            F_LogError("*********************** VulkanDescriptorPool::Init: Failed to CreateVkDescriptorPool, nameDescriptorPool: [%s] !", GetName().c_str());
            return false;
        }

        return true;
    }

}; //LostPeterPluginRendererVulkan