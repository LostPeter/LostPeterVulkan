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

#include "../include/RHIVulkanFrameBuffer.h"
#include "../include/RHIVulkanDevice.h"
#include "../include/RHIVulkanTextureView.h"
#include "../include/RHIVulkanRenderPass.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanFrameBuffer::RHIVulkanFrameBuffer(RHIVulkanDevice* pVulkanDevice, const RHIFrameBufferCreateInfo& createInfo)
        : RHIFrameBuffer(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)
        , m_vkFramebuffer(VK_NULL_HANDLE)
        , m_pVulkanRenderPass((RHIVulkanRenderPass*)createInfo.pRenderPass)
        , m_sExtent(createInfo.sExtent)
    {
        F_Assert(m_pVulkanDevice && m_pVulkanRenderPass&& "RHIVulkanFrameBuffer::RHIVulkanFrameBuffer")

        refreshParam(createInfo);
        createVkFramebuffer();
    }   

    RHIVulkanFrameBuffer::~RHIVulkanFrameBuffer()
    {
        Destroy();
    }

    void RHIVulkanFrameBuffer::Destroy()
    {
        if (m_vkFramebuffer != VK_NULL_HANDLE)
        {
            m_pVulkanDevice->DestroyVkFramebuffer(m_vkFramebuffer);
        }
        m_vkFramebuffer = VK_NULL_HANDLE;
    }
    
    void RHIVulkanFrameBuffer::refreshParam(const RHIFrameBufferCreateInfo& createInfo)
    {
        size_t count = createInfo.aTextureView.size();
        for (size_t i = 0; i < count; i++)
        {
            RHIVulkanTextureView* pTextureView = (RHIVulkanTextureView*)createInfo.aTextureView[i];
            m_aVulkanTextureView.push_back(pTextureView);
        }
    }
    void RHIVulkanFrameBuffer::createVkFramebuffer()
    {
        if (!m_pVulkanDevice->CreateVkFramebuffer(m_strName,
                                                  m_aVulkanTextureView,
                                                  m_pVulkanRenderPass,
                                                  m_sExtent,
                                                  m_vkFramebuffer))
        {
            setIsError(true);
            F_LogError("*********************** RHIVulkanFrameBuffer::createVkFramebuffer: CreateVkFramebuffer failed, Name: [%s] !", m_strName.c_str());
            return;
        }

        if (RHI_IsDebug())
        {
            if (m_strDebugName.empty())
                m_strDebugName = m_strName;
            m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_FRAMEBUFFER, reinterpret_cast<uint64_t>(m_vkFramebuffer), m_strDebugName.c_str());
            F_LogInfo("RHIVulkanFrameBuffer::createVkFramebuffer: Create VkFramebuffer success, Name: [%s] !", m_strDebugName.c_str());
        }
    }

}; //LostPeterPluginRHIVulkan