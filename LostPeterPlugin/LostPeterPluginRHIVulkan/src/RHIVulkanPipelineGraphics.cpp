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
#include "../include/RHIVulkanShaderModule.h"
#include "../include/RHIVulkanPipelineCache.h"
#include "../include/RHIVulkanPipelineLayout.h"
#include "../include/RHIVulkanRenderPass.h"
#include "../include/RHIVulkanConverter.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanPipelineGraphics::RHIVulkanPipelineGraphics(RHIVulkanDevice* pVulkanDevice, const RHIPipelineGraphicsCreateInfo& createInfo)
        : RHIPipelineGraphics(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)
        , m_pVulkanShaderVertex((RHIVulkanShaderModule*)createInfo.pShaderVertex)
        , m_pVulkanShaderPixel((RHIVulkanShaderModule*)createInfo.pShaderPixel)
        , m_pVulkanShaderGeometry((RHIVulkanShaderModule*)createInfo.pShaderGeometry)
        , m_pVulkanShaderDomain((RHIVulkanShaderModule*)createInfo.pShaderDomain)
        , m_pVulkanShaderHull((RHIVulkanShaderModule*)createInfo.pShaderHull)
        , m_pVulkanPipelineLayout((RHIVulkanPipelineLayout*)createInfo.pPipelineLayout)
        , m_pVulkanPipelineCache((RHIVulkanPipelineCache*)createInfo.pPipelineCache)
        , m_pVulkanRenderPass((RHIVulkanRenderPass*)createInfo.pRenderPass)
        , m_sVertexState(createInfo.sVertexState)
        , m_sPrimitiveState(createInfo.sPrimitiveState)
        , m_sTessellationState(createInfo.sTessellationState)
        , m_sDepthStencilState(createInfo.sDepthStencilState)
        , m_sMultiSampleState(createInfo.sMultiSampleState)
        , m_sFragmentState(createInfo.sFragmentState)
        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && m_pVulkanShaderVertex && m_pVulkanShaderPixel && m_pVulkanPipelineCache && m_pVulkanPipelineLayout && m_pVulkanRenderPass && "RHIVulkanPipelineGraphics::RHIVulkanPipelineGraphics")

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
        if (!m_pVulkanDevice->CreateVkPipeline_Graphics(m_pVulkanShaderVertex,
                                                        m_pVulkanShaderPixel,
                                                        m_pVulkanShaderGeometry,
                                                        m_pVulkanShaderDomain,
                                                        m_pVulkanShaderHull,
                                                        m_pVulkanPipelineLayout,
                                                        m_pVulkanPipelineCache,
                                                        m_pVulkanRenderPass,
                                                        m_sVertexState,
                                                        m_sPrimitiveState,
                                                        m_sTessellationState,
                                                        m_sDepthStencilState,
                                                        m_sMultiSampleState,
                                                        m_sFragmentState,
                                                        m_vkPipeline))
        {
            setIsError(true);
            F_LogError("*********************** RHIVulkanPipelineGraphics::createVkPipelineGraphics: CreateVkPipeline_Graphics failed, Name: [%s] !", m_strName.c_str());
            return;
        }

        if (RHI_IsDebug())
        {
            if (m_strDebugName.empty())
                m_strDebugName = m_strName;
            m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_PIPELINE, reinterpret_cast<uint64_t>(m_vkPipeline), m_strDebugName.c_str());
            F_LogInfo("RHIVulkanPipelineGraphics::createVkPipelineGraphics: Create VkPipeline Graphics success, Name: [%s] !", m_strDebugName.c_str());
        }
    }

}; //LostPeterPluginRHIVulkan