/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-13
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIVulkanPipelineGraphics.h"
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanPipelineGraphics::RHIVulkanPipelineGraphics(RHIVulkanDevice* pVulkanDevice, const RHIPipelineGraphicsCreateInfo& createInfo)
        : RHIPipelineGraphics(pVulkanDevice, createInfo)
        , m_pVulkanDevice(pVulkanDevice)
        , m_vkPipeline(VK_NULL_HANDLE)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanPipelineGraphics::RHIVulkanPipelineGraphics")

        createVkPipeline();
    }

    RHIVulkanPipelineGraphics::~RHIVulkanPipelineGraphics()
    {
        Destroy();
    }
    
    void RHIVulkanPipelineGraphics::Destroy()
    {
        if (m_vkPipeline != VK_NULL_HANDLE)
        {
            m_pVulkanDevice->DestroyVkPipeline(m_vkPipeline);
        }
        m_vkPipeline = VK_NULL_HANDLE;
    }

    void RHIVulkanPipelineGraphics::createVkPipeline()
    {
        

        if (RHI_IsDebug())
        {
            if (!m_strDebugName.empty())
            {
                m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_PIPELINE, reinterpret_cast<uint64_t>(m_vkPipeline), m_strDebugName.c_str());
            }
        }
    }

}; //LostPeterPluginRHIVulkan