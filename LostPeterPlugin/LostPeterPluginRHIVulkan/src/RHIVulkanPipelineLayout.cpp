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
#include "../include/RHIVulkanBindGroupLayout.h"
#include "../include/RHIVulkanConverter.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanPipelineLayout::RHIVulkanPipelineLayout(RHIVulkanDevice* pVulkanDevice, const RHIPipelineLayoutCreateInfo& createInfo)
        : RHIPipelineLayout(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)
        , m_vkPipelineLayout(VK_NULL_HANDLE)
        , m_aBindGroupLayout(createInfo.aBindGroupLayout)
        , m_aPipelineConstantLayout(createInfo.aPipelineConstantLayout)
        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanPipelineLayout::RHIVulkanPipelineLayout")

        refreshParam();
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
    
    void RHIVulkanPipelineLayout::refreshParam()
    {
        //VkDescriptorSetLayoutVector
        m_aVkDescriptorSetLayout.clear();
        uint32 count = (uint32)m_aBindGroupLayout.size();
        for (uint32 i = 0; i < count; i++)
        {
            RHIVulkanBindGroupLayout* pVulkanBindGroupLayout = (RHIVulkanBindGroupLayout*)m_aBindGroupLayout[i];
            F_Assert(pVulkanBindGroupLayout && "RHIVulkanPipelineLayout::refreshParam")
            m_aVkDescriptorSetLayout.push_back(pVulkanBindGroupLayout->GetVkDescriptorSetLayout());
        }

        //VkPushConstantRangeVector
        m_aVkPushConstantRange.clear();
        count = (uint32)m_aPipelineConstantLayout.size();
        for (uint32 i = 0; i < count; i++)
        {
            RHIPipelineConstantLayout& pipelineConstantLayout = m_aPipelineConstantLayout[i];

            VkPushConstantRange vkPushConstantRange = {};
            VkShaderStageFlags stageFlags = RHIVulkanConverter::TransformToVkShaderStageFlagsFromShaderStagelags(pipelineConstantLayout.flagsShaderStage);
            m_pVulkanDevice->CreateVkPushConstantRange(stageFlags,
                                                       (uint32_t)pipelineConstantLayout.nOffset,
                                                       (uint32_t)pipelineConstantLayout.nSize,
                                                       vkPushConstantRange);
            m_aVkPushConstantRange.push_back(vkPushConstantRange);
        }
    }

    void RHIVulkanPipelineLayout::createVkPipelineLayout()
    {
        if (m_aVkDescriptorSetLayout.empty() ||
            !m_pVulkanDevice->CreateVkPipelineLayout(m_aVkDescriptorSetLayout, 
                                                     m_aVkPushConstantRange, 
                                                     m_vkPipelineLayout))
        {
            setIsError(true);
            F_LogError("*********************** RHIVulkanBindGroupLayout::createVkDescriptorSetLayout: Wrong CreateVkDescriptorSetLayout failed, Name: [%s] !", m_strName.c_str());
            return;
        }

        if (RHI_IsDebug())
        {
            if (m_strDebugName.empty())
                m_strDebugName = m_strName;
            m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_PIPELINE_LAYOUT, reinterpret_cast<uint64_t>(m_vkPipelineLayout), m_strDebugName.c_str());
            F_LogInfo("RHIVulkanPipelineLayout::createVkPipelineLayout: Create VkPipelineLayout success, Name: [%s] !", m_strDebugName.c_str());
        }
    }

}; //LostPeterPluginRHIVulkan