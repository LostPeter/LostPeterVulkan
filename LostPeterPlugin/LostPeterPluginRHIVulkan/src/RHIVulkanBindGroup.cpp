/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-12
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIVulkanBindGroup.h"
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanBindGroup::RHIVulkanBindGroup(RHIVulkanDevice* pVulkanDevice, const RHIBindGroupCreateInfo& createInfo)
        : RHIBindGroup(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)
        , m_vkDescriptorSet(VK_NULL_HANDLE)
        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanBindGroup::RHIVulkanBindGroup")

        createVkDescriptorSet();
    }

    RHIVulkanBindGroup::~RHIVulkanBindGroup()
    {
        Destroy();
    }

    void RHIVulkanBindGroup::Destroy()
    {

    }
    
    void RHIVulkanBindGroup::createVkDescriptorSet()
    {   


        if (RHI_IsDebug())
        {
            if (m_strDebugName.empty())
                m_strDebugName = m_strName;
            m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_DESCRIPTOR_SET, reinterpret_cast<uint64_t>(m_vkDescriptorSet), m_strDebugName.c_str());
            F_LogInfo("RHIVulkanBindGroup::createVkDescriptorSet: Create VkDescriptorSet success, Name: [%s] !", m_strDebugName.c_str());
        }
    }

}; //LostPeterPluginRHIVulkan