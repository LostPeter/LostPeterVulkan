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
#include "RHIVulkanObject.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanSurface : public RHISurface
                                           , public RHIVulkanObject
    {   
        friend class RHIVulkanDevice;

    protected:
        RHIVulkanSurface(RHIVulkanDevice* pVulkanDevice, const RHISurfaceCreateInfo& createInfo);
    public:
        virtual ~RHIVulkanSurface();

    public:
    protected:
        VkSurfaceKHR m_vkSurface;

        GLFWwindow* m_pWindow;
        String m_strDebugName;

    public:
        F_FORCEINLINE VkSurfaceKHR& GetVulkanSurface() { return m_vkSurface; }

        F_FORCEINLINE GLFWwindow* GetWindow() const { return m_pWindow; }
        F_FORCEINLINE const String& GetDebugName() const { return m_strDebugName; }
            
    public:
        virtual void Destroy();

    protected:
        void createVkSurfaceKHR();
    };
    
}; //LostPeterPluginRHIVulkan

#endif