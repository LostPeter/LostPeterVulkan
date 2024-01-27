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

#include "../include/VulkanDescriptorSetLayout.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanDescriptorSetLayoutManager.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(const String& strNameDescriptorSetLayout, VulkanDescriptorSetLayoutManager* pDescriptorSetLayoutManager)
        : Base(strNameDescriptorSetLayout)
        , m_pDescriptorSetLayoutManager(pDescriptorSetLayoutManager)
        , m_vkDescriptorSetLayout(VK_NULL_HANDLE)
    {
        F_Assert(m_pDescriptorSetLayoutManager != nullptr && "VulkanDescriptorSetLayout::VulkanDescriptorSetLayout")
    }

    VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
    {
        Destroy();
    }

    void VulkanDescriptorSetLayout::Destroy()
    {
        if (m_vkDescriptorSetLayout != VK_NULL_HANDLE)
        {
            m_pDescriptorSetLayoutManager->GetDevice()->DestroyVkDescriptorSetLayout(m_vkDescriptorSetLayout);
        }
        m_vkDescriptorSetLayout = VK_NULL_HANDLE;
    }

}; //LostPeterPluginRendererVulkan