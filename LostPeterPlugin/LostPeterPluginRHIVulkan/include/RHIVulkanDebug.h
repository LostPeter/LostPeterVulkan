/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_VULKAN_DEBUG_H_
#define _RHI_VULKAN_DEBUG_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class RHIVulkanDebug
    {
    public:
        RHIVulkanDebug(RHIVulkanInstance* pInstance);
        ~RHIVulkanDebug();

    public:
    protected:
        RHIVulkanInstance* m_pInstance;
        VkDebugReportCallbackEXT m_vkDebugReport;
    
    public:
        F_FORCEINLINE const VkDebugReportCallbackEXT& GetVkDebugReportCallbackEXT() const { return m_vkDebugReport; }

    public:
    protected:
        void createDebugReport();
        void destroyDebugReport();
    };

}; //LostPeterPluginRHIVulkan

#endif