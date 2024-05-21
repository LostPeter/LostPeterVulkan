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

#include "../include/RHIVulkanBindGroupLayout.h"
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanBindGroupLayout::RHIVulkanBindGroupLayout(RHIVulkanDevice* pVulkanDevice, const RHIBindGroupLayoutCreateInfo& createInfo)
        : RHIBindGroupLayout(pVulkanDevice, createInfo)
        , m_pVulkanDevice(pVulkanDevice)
        , m_vkDescriptorSetLayout(VK_NULL_HANDLE)
        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanBindGroupLayout::RHIVulkanBindGroupLayout")

        createVkDescriptorSetLayout();
    }   

    RHIVulkanBindGroupLayout::~RHIVulkanBindGroupLayout()
    {
        Destroy();
    }

    void RHIVulkanBindGroupLayout::Destroy()
    {
        if (m_vkDescriptorSetLayout != VK_NULL_HANDLE)
        {
            m_pVulkanDevice->DestroyVkDescriptorSetLayout(m_vkDescriptorSetLayout);
        }
        m_vkDescriptorSetLayout = VK_NULL_HANDLE;
    }
    
    void RHIVulkanBindGroupLayout::createVkDescriptorSetLayout()
    {


        if (RHI_IsDebug())
        {
            if (!m_strDebugName.empty())
            {
                m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, reinterpret_cast<uint64_t>(m_vkDescriptorSetLayout), m_strDebugName.c_str());
            }
        }
    }

}; //LostPeterPluginRHIVulkan