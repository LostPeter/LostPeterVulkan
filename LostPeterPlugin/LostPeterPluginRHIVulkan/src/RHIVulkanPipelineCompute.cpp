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

#include "../include/RHIVulkanPipelineCompute.h"
#include "../include/RHIVulkanDevice.h"
#include "../include/RHIVulkanShaderModule.h"
#include "../include/RHIVulkanPipelineLayout.h"
#include "../include/RHIVulkanPipelineCache.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanPipelineCompute::RHIVulkanPipelineCompute(RHIVulkanDevice* pVulkanDevice, const RHIPipelineComputeCreateInfo& createInfo)
        : RHIPipelineCompute(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)
        , m_pVulkanComputeShader((RHIVulkanShaderModule*)createInfo.pComputeShader)
        , m_pVulkanPipelineLayout((RHIVulkanPipelineLayout*)createInfo.pPipelineLayout)
        , m_pVulkanPipelineCache((RHIVulkanPipelineCache*)createInfo.pPipelineCache)
        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && m_pVulkanComputeShader && m_pVulkanPipelineLayout && m_pVulkanPipelineCache && "RHIVulkanPipelineCompute::RHIVulkanPipelineCompute")
    
        createVkPipelineCompute();
    }

    RHIVulkanPipelineCompute::~RHIVulkanPipelineCompute()
    {
        Destroy();
    }
    
    void RHIVulkanPipelineCompute::Destroy()
    {
        destroyVkPipeline(m_pVulkanDevice);
    }

    void RHIVulkanPipelineCompute::createVkPipelineCompute()
    {
        if (!m_pVulkanDevice->CreateVkPipeline_Compute(m_pVulkanComputeShader,
                                                       m_pVulkanPipelineLayout,
                                                       m_pVulkanPipelineCache,
                                                       m_vkPipeline))
        {
            setIsError(true);
            F_LogError("*********************** RHIVulkanPipelineCompute::createVkPipelineCompute: CreateVkPipeline_Compute failed, Name: [%s] !", m_strName.c_str());
            return;
        }

        if (RHI_IsDebug())
        {
            if (m_strDebugName.empty())
                m_strDebugName = m_strName;
            m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_PIPELINE, reinterpret_cast<uint64_t>(m_vkPipeline), m_strDebugName.c_str());
            F_LogInfo("RHIVulkanPipelineCompute::createVkPipelineCompute: Create VkPipeline Compute success, Name: [%s] !", m_strDebugName.c_str());
        }
    }

}; //LostPeterPluginRHIVulkan