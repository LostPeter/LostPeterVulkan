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
        RHIVulkanDebug(RHIVulkanInstance* pVulkanInstance);
        ~RHIVulkanDebug();

    public:
    protected:
        RHIVulkanInstance* m_pVulkanInstance;
        VkDebugReportCallbackEXT m_vkDebugReport;
        PFN_vkSetDebugUtilsObjectNameEXT m_vkSetDebugUtilsObjectNameEXT;
    
    public:
        F_FORCEINLINE const VkDebugReportCallbackEXT& GetVkDebugReportCallbackEXT() const { return m_vkDebugReport; }

    public:
        void SetDebugObject(VkDevice vkDevice, VkObjectType objectType, uint64_t objectHandle, const char* objectName);

    protected:
        void createDebugReport();
        void destroyDebugReport();
    };

}; //LostPeterPluginRHIVulkan

#endif