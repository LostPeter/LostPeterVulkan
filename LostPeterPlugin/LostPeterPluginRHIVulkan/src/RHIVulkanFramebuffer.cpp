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

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanFrameBuffer::RHIVulkanFrameBuffer(RHIVulkanDevice* pVulkanDevice, const RHIFrameBufferCreateInfo& createInfo)
        : RHIFrameBuffer(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)
        , m_vkFramebuffer(VK_NULL_HANDLE)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanFrameBuffer::RHIVulkanFrameBuffer")

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
    
    void RHIVulkanFrameBuffer::createVkFramebuffer()
    {
        

        if (RHI_IsDebug())
        {
            if (m_strDebugName.empty())
                m_strDebugName = m_strName;
            m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_FRAMEBUFFER, reinterpret_cast<uint64_t>(m_vkFramebuffer), m_strDebugName.c_str());
            F_LogInfo("RHIVulkanFrameBuffer::createVkFramebuffer: Create VkFramebuffer success, Name: [%s] !", m_strDebugName.c_str());
        }
    }

}; //LostPeterPluginRHIVulkan