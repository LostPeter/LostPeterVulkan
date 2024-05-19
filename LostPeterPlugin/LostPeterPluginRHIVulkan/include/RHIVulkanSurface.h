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

#ifndef _RHI_VULKAN_SURFACE_H_
#define _RHI_VULKAN_SURFACE_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanSurface : public RHISurface
    {
    public:
        RHIVulkanSurface(RHIVulkanDevice* pVulkanDevice, const RHISurfaceCreateInfo& createInfo);
        virtual ~RHIVulkanSurface();

    public:
    protected:
        RHIVulkanDevice* m_pVulkanDevice;
        VkSurfaceKHR m_vkSurface;
        GLFWwindow* m_pWindow;

    public:
        F_FORCEINLINE RHIVulkanDevice* GetVulkanDevice() { return m_pVulkanDevice; }
        F_FORCEINLINE VkSurfaceKHR& GetDummySurface() { return m_vkSurface; }
        F_FORCEINLINE GLFWwindow* GetWindow() const { return m_pWindow; }

            
    public:
        virtual void Destroy();

    protected:
        void createVkSurfaceKHR();
    };
    
}; //LostPeterPluginRHIVulkan

#endif