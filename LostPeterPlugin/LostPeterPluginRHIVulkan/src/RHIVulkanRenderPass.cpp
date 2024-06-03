/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIVulkanRenderPass.h"
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanRenderPass::RHIVulkanRenderPass(RHIVulkanDevice* pVulkanDevice, const RHIRenderPassCreateInfo& createInfo)
        : RHIRenderPass(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)
        , m_vkRenderPass(VK_NULL_HANDLE)
        , m_aColorAttachment(createInfo.aColorAttachment)
        , m_aDepthStencilAttachment(createInfo.aDepthStencilAttachment)
        , m_aSubpassDescription(createInfo.aSubpassDescription)
        , m_aSubpassDependency(createInfo.aSubpassDependency)
        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanRenderPass::RHIVulkanRenderPass")

        createVkRenderPass();    
    }

    RHIVulkanRenderPass::~RHIVulkanRenderPass()
    {
        Destroy();
    }

    void RHIVulkanRenderPass::Destroy()
    {
        if (m_vkRenderPass != VK_NULL_HANDLE)
        {
            m_pVulkanDevice->DestroyVkRenderPass(m_vkRenderPass);
        }
        m_vkRenderPass = VK_NULL_HANDLE;
    }
    
    void RHIVulkanRenderPass::createVkRenderPass()
    {
        if (!m_pVulkanDevice->CreateVkRenderPass(m_strName,
                                                 m_aColorAttachment,
                                                 m_aDepthStencilAttachment,
                                                 m_aSubpassDescription,
                                                 m_aSubpassDependency,
                                                 m_vkRenderPass))
        {
            setIsError(true);
            F_LogError("*********************** RHIVulkanRenderPass::createVkRenderPass: CreateVkRenderPass failed, Name: [%s] !", m_strName.c_str());
            return;
        }

        if (RHI_IsDebug())
        {
            if (m_strDebugName.empty())
                m_strDebugName = m_strName;
            m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_RENDER_PASS, reinterpret_cast<uint64_t>(m_vkRenderPass), m_strDebugName.c_str());
            F_LogInfo("RHIVulkanRenderPass::createVkRenderPass: Create VkRenderPass success, Name: [%s] !", m_strDebugName.c_str());
        }
    }

}; //LostPeterPluginRHIVulkan