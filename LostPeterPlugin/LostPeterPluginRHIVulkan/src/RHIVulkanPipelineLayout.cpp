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

#include "../include/RHIVulkanPipelineLayout.h"
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanPipelineLayout::RHIVulkanPipelineLayout(RHIVulkanDevice* pVulkanDevice, const RHIPipelineLayoutCreateInfo& createInfo)
        : RHIPipelineLayout(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)
        , m_vkPipelineLayout(VK_NULL_HANDLE)

        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanPipelineLayout::RHIVulkanPipelineLayout")

        createVkPipelineLayout();
    }

    RHIVulkanPipelineLayout::~RHIVulkanPipelineLayout()
    {
        Destroy();
    }

    void RHIVulkanPipelineLayout::Destroy()
    {
        if (m_vkPipelineLayout != VK_NULL_HANDLE)
        {
            m_pVulkanDevice->DestroyVkPipelineLayout(m_vkPipelineLayout);
        }
        m_vkPipelineLayout = VK_NULL_HANDLE;
    }
    
    void RHIVulkanPipelineLayout::createVkPipelineLayout()
    {
        

        if (RHI_IsDebug())
        {
            if (!m_strDebugName.empty())
            {
                m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_PIPELINE_LAYOUT, reinterpret_cast<uint64_t>(m_vkPipelineLayout), m_strDebugName.c_str());
            }
        }
    }

}; //LostPeterPluginRHIVulkan