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
        , RHIVulkanObject(pVulkanDevice)
        , m_pVulkanPipelineCache((RHIVulkanPipelineCache*)createInfo.pPipelineCache)
        , m_pVulkanPipelineLayout((RHIVulkanPipelineLayout*)createInfo.pPipelineLayout)
        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanPipelineGraphics::RHIVulkanPipelineGraphics")

        createVkPipelineGraphics();
    }

    RHIVulkanPipelineGraphics::~RHIVulkanPipelineGraphics()
    {
        Destroy();
    }
    
    void RHIVulkanPipelineGraphics::Destroy()
    {
        destroyVkPipeline(m_pVulkanDevice);
    }

    void RHIVulkanPipelineGraphics::createVkPipelineGraphics()
    {
        

        if (RHI_IsDebug())
        {
            if (m_strDebugName.empty())
                m_strDebugName = m_strName;
            m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_PIPELINE, reinterpret_cast<uint64_t>(m_vkPipeline), m_strDebugName.c_str());
            F_LogInfo("RHIVulkanPipelineGraphics::createVkPipelineGraphics: Create VkPipeline Graphics success, Name: [%s] !", m_strDebugName.c_str());
        }
    }

}; //LostPeterPluginRHIVulkan