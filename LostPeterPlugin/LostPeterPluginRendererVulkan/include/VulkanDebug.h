/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-19
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_DEBUG_H_
#define _VULKAN_DEBUG_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanDebug
    {
    public:
        VulkanDebug(VulkanInstance* pInstance);
        ~VulkanDebug();

    public:
    protected:
        VulkanInstance* m_pInstance;
        VkDebugReportCallbackEXT m_vkDebugReport;
    
    public:
        F_FORCEINLINE const VkDebugReportCallbackEXT& GetVkDebugReportCallbackEXT() const { return m_vkDebugReport; }

    public:
    protected:
        void createDebugReport();
        void destroyDebugReport();
    };

}; //LostPeterPluginRendererVulkan

#endif