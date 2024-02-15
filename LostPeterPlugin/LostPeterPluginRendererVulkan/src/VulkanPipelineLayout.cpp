/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanPipelineLayout.h"
#include "../include/VulkanDevice.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanPipelineLayout::VulkanPipelineLayout(const String& namePipelineLayout, VulkanDevice* pDevice)
        : Base(namePipelineLayout)
        , m_pDevice(pDevice)
        , m_vkPipelineLayout(VK_NULL_HANDLE)
    {
        F_Assert(m_pDevice && "VulkanPipelineLayout::VulkanPipelineLayout")
    }

    VulkanPipelineLayout::~VulkanPipelineLayout()
    {
        Destroy();
    }

    void VulkanPipelineLayout::Destroy()
    {
        if (this->m_vkPipelineLayout != VK_NULL_HANDLE)
        {
            m_pDevice->DestroyVkPipelineLayout(this->m_vkPipelineLayout);
        }
        this->m_vkPipelineLayout = VK_NULL_HANDLE;
    }

    bool VulkanPipelineLayout::Init(const VkDescriptorSetLayoutVector& aDescriptorSetLayout)
    {
        Destroy();
        if (!m_pDevice->CreateVkPipelineLayout(aDescriptorSetLayout,
                                               this->m_vkPipelineLayout))
        {
            F_LogError("*********************** VulkanPipelineLayout::Init: Failed to CreateVkPipelineLayout, name: [%s] !", GetName().c_str());
            return false;
        }

        return true;
    }

}; //LostPeterPluginRendererVulkan