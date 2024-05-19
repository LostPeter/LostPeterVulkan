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
        , m_pVulkanDevice(pVulkanDevice)
        , m_vkSurface(VK_NULL_HANDLE)
        , m_pWindow((GLFWwindow*)createInfo.pWindow)
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
            F_LogError("*********************** RHIVulkanSurface::createVkSurfaceKHR: CreateVkSurfaceKHR failed !");
            return;
        }

        F_LogInfo("*********************** RHIVulkanSurface::createVkSurfaceKHR: Create Surface success !");
    }
    
}; //LostPeterPluginRHIVulkan