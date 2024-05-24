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

#include "../include/RHIVulkanSurface.h"
#include "../include/RHIVulkanDevice.h"
#include "../include/RHIVulkanPhysicalDevice.h"
#include "../include/RHIVulkanInstance.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanSurface::RHIVulkanSurface(RHIVulkanDevice* pVulkanDevice, const RHISurfaceCreateInfo& createInfo)
        : RHISurface(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)
        , m_vkSurface(VK_NULL_HANDLE)
        , m_pWindow((GLFWwindow*)createInfo.pWindow)
        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanSurface::RHIVulkanSurface")

        createVkSurfaceKHR();
    }

    RHIVulkanSurface::~RHIVulkanSurface()
    {
        Destroy();
    }

    void RHIVulkanSurface::Destroy()
    {
        if (m_vkSurface != VK_NULL_HANDLE)
        {
            m_pVulkanDevice->GetVulkanPhysicalDevice()->GetVulkanInstance()->DestroyVkSurfaceKHR(m_vkSurface);
        }
        m_vkSurface = VK_NULL_HANDLE;
    }

    void RHIVulkanSurface::createVkSurfaceKHR()
    {
        if (!m_pVulkanDevice->GetVulkanPhysicalDevice()->GetVulkanInstance()->CreateVkSurfaceKHR(m_pWindow, m_vkSurface))
        {
            F_LogError("*********************** RHIVulkanSurface::createVkSurfaceKHR: CreateVkSurfaceKHR failed, Name: [%s] !", m_strName.c_str());
            return;
        }

        if (RHI_IsDebug())
        {
            if (m_strDebugName.empty())
                m_strDebugName = m_strName;
            m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_SURFACE_KHR, reinterpret_cast<uint64_t>(m_vkSurface), m_strDebugName.c_str());
        }
        F_LogInfo("RHIVulkanSurface::createVkSurfaceKHR: Create VkSurfaceKHR success, Name: [%s] !", m_strName.c_str());
    }
    
}; //LostPeterPluginRHIVulkan