/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIVulkanShaderModule.h"
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanShaderModule::RHIVulkanShaderModule(RHIVulkanDevice* pVulkanDevice, const RHIShaderModuleCreateInfo& createInfo)
        : RHIShaderModule(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)
        , m_vkShaderModule(VK_NULL_HANDLE)
        , m_eShaderStageBits(createInfo.eShaderStageBits)
        , m_strPath(createInfo.strPath)
        , m_strContent(createInfo.strByteCode)
        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanShaderModule::RHIVulkanShaderModule")

        createVkShaderModule();
    }

    RHIVulkanShaderModule::~RHIVulkanShaderModule()
    {
        Destroy();
    }

    void RHIVulkanShaderModule::Destroy()
    {
        if (m_vkShaderModule != VK_NULL_HANDLE)
        {
            m_pVulkanDevice->DestroyVkShaderModule(m_vkShaderModule);
        }
        m_vkShaderModule = VK_NULL_HANDLE;
    }

    void RHIVulkanShaderModule::createVkShaderModule()
    {   
        if (!m_strContent.empty())
        {
            if (!m_pVulkanDevice->CreateVkShaderModule(m_strContent.size(),
                                                       m_strContent.data(),
                                                       m_vkShaderModule))
            {
                F_LogError("*********************** RHIVulkanShaderModule::createVkShaderModule: CreateVkShaderModule failed, name: [%s], type: [%s] !", m_strDebugName.c_str(), RHI_GetShaderStageBitsTypeName(m_eShaderStageBits).c_str());
                return;
            }
        }
        else if (!m_strPath.empty())
        {
            if (!m_pVulkanDevice->CreateVkShaderModule(m_eShaderStageBits,
                                                       m_strPath,
                                                       m_vkShaderModule))
            {
                F_LogError("*********************** RHIVulkanShaderModule::createVkShaderModule: CreateVkShaderModule failed, name: [%s], type: [%s] !", m_strDebugName.c_str(), RHI_GetShaderStageBitsTypeName(m_eShaderStageBits).c_str());
                return;
            }
        }

        if (RHI_IsDebug())
        {
            if (!m_strDebugName.empty())
            {
                m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_SHADER_MODULE, reinterpret_cast<uint64_t>(m_vkShaderModule), m_strDebugName.c_str());
            }
        }
    }

}; //LostPeterPluginRHIVulkan