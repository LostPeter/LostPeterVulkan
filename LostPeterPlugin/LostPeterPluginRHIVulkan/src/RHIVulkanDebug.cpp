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

#include "../include/RHIVulkanDebug.h"
#include "../include/RHIVulkanInstance.h"

namespace LostPeterPluginRHIVulkan
{
    static VKAPI_ATTR VkBool32 VKAPI_CALL g_DebugReportCallback(VkDebugReportFlagsEXT msgFlags,
                                                                VkDebugReportObjectTypeEXT objectType, 
                                                                uint64_t object, 
                                                                size_t location,
                                                                int32_t msgCode, 
                                                                const char* pLayerPrefix, 
                                                                const char* pMessage, 
                                                                void* pUserData) 
    {
        String msg;

        if (msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
            msg += "Error: ";
        else if (msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
            msg += "Warning: ";
        else if (msgFlags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
            msg += "Performance Warning: ";
        else if (msgFlags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
            msg += "Info: ";
        else if (msgFlags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
            msg += "Debug: ";

        if (msgCode == 2) 
            return VK_FALSE;
        if (msgCode == 15) 
            return VK_FALSE;

#if F_PLATFORM == F_PLATFORM_ANDROID
        if (msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
        {
            DUMP_CALLSTACK("Vulkan validation error");
        }
#endif
        F_LogInfo("$$$$$$$$$$$$$$$$$$$$$$$ Vulkan Debug: %s [%s, %d]: %s !", msg.c_str(), pLayerPrefix, msgCode, pMessage);

        return VK_FALSE;
    }


    RHIVulkanDebug::RHIVulkanDebug(RHIVulkanInstance* pInstance)
        : m_pInstance(pInstance)
        , m_vkDebugReport(VK_NULL_HANDLE)
        , m_vkSetDebugUtilsObjectNameEXT(VK_NULL_HANDLE)
    {
        F_Assert(m_pInstance && "RHIVulkanDebug::RHIVulkanDebug")
        createDebugReport();
        if (m_vkDebugReport == VK_NULL_HANDLE)
        {
            F_LogError("*********************** RHIVulkanDebug::RHIVulkanDebug: Failed to create VkDebugReportCallbackEXT !");
        }
    }

    RHIVulkanDebug::~RHIVulkanDebug()
    {
        destroyDebugReport();
    }

    void RHIVulkanDebug::SetDebugObject(VkDevice vkDevice, VkObjectType objectType, uint64_t objectHandle, const char* objectName)
    {
        if (m_vkSetDebugUtilsObjectNameEXT == VK_NULL_HANDLE)
            return;

        VkDebugUtilsObjectNameInfoEXT info = { VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT };
        info.objectType    = objectType;
        info.objectHandle  = objectHandle;
        info.pObjectName   = objectName;
        m_vkSetDebugUtilsObjectNameEXT(vkDevice, &info);

        F_LogInfo("RHIVulkanDebug::SetDebugObject: Success to call VkDebugUtilsObjectNameInfoEXT, obj: [%s] !", objectName);
    }

    void RHIVulkanDebug::createDebugReport()
    {
        //1> get vkCreateDebugReportCallbackEXT func
        PFN_vkCreateDebugReportCallbackEXT pFunc = (PFN_vkCreateDebugReportCallbackEXT)(vkGetInstanceProcAddr(m_pInstance->GetVkInstance(), "vkCreateDebugReportCallbackEXT"));
        if (pFunc == nullptr)
        {
            F_LogError("*********************** RHIVulkanDebug::createDebugReport: Failed to get vkCreateDebugReportCallbackEXT func !");
            return;
        }

        //2> vkCreateDebugReportCallbackEXT
        VkDebugReportCallbackCreateInfoEXT createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
        createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT |
                           VK_DEBUG_REPORT_WARNING_BIT_EXT |
                           VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
        createInfo.pUserData = reinterpret_cast<void*>(this);
        createInfo.pfnCallback = &g_DebugReportCallback;
        if (!RHI_CheckVkResult(pFunc(m_pInstance->GetVkInstance(), &createInfo, nullptr, &m_vkDebugReport), "g_CreateDebugReportCallback"))
        {
            F_LogError("*********************** RHIVulkanDebug::createDebugReport: Failed call vkCreateDebugReportCallbackEXT !");
            return;
        }
        F_LogInfo("RHIVulkanDebug::createDebugReport: Success to call vkCreateDebugReportCallbackEXT !");

        //3> vkSetDebugUtilsObjectNameEXT
        m_vkSetDebugUtilsObjectNameEXT = reinterpret_cast<PFN_vkSetDebugUtilsObjectNameEXT>(vkGetInstanceProcAddr(m_pInstance->GetVkInstance(), "vkSetDebugUtilsObjectNameEXT"));
    }

    void RHIVulkanDebug::destroyDebugReport()
    {
        if (m_vkDebugReport == VK_NULL_HANDLE)
            return;

        //1> get vkDestroyDebugReportCallbackEXT func
        PFN_vkDestroyDebugReportCallbackEXT pFunc = (PFN_vkDestroyDebugReportCallbackEXT)(vkGetInstanceProcAddr(m_pInstance->GetVkInstance(), "vkDestroyDebugReportCallbackEXT"));
        if (pFunc == nullptr)
        {
            F_LogError("*********************** RHIVulkanDebug::destroyDebugReport: Failed to get vkDestroyDebugReportCallbackEXT func !");
            return;
        }

        //2> vkDestroyDebugReportCallbackEXT
        pFunc(m_pInstance->GetVkInstance(), m_vkDebugReport, nullptr);
        m_vkDebugReport = VK_NULL_HANDLE;
        F_LogInfo("RHIVulkanDebug::destroyDebugReport: Success to call vkDestroyDebugReportCallbackEXT !");

        //3> vkSetDebugUtilsObjectNameEXT
        m_vkSetDebugUtilsObjectNameEXT = VK_NULL_HANDLE;
    }

}; //LostPeterPluginRHIVulkan