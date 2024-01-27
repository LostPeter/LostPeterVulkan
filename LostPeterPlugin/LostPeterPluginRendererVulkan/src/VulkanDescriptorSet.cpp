/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-27
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanDescriptorSet.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanDescriptorSetManager.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanDescriptorSet::VulkanDescriptorSet(const String& strNameDescriptorSet, VulkanDescriptorSetManager* pDescriptorSetManager)
        : Base(strNameDescriptorSet)
        , m_pDescriptorSetManager(pDescriptorSetManager)
        , m_vkDescriptorSet(VK_NULL_HANDLE)
    {
        F_Assert(m_pDescriptorSetManager != nullptr && "VulkanDescriptorSet::VulkanDescriptorSet")
    }

    VulkanDescriptorSet::~VulkanDescriptorSet()
    {
        Destroy();
    }

    void VulkanDescriptorSet::Destroy()
    {
        
    }

}; //LostPeterPluginRendererVulkan