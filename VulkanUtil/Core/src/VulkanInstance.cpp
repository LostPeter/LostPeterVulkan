/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-12-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/PreInclude.h"
#include "../include/VulkanInstance.h"
#include "../include/VulkanDevice.h"

namespace LostPeter
{
    VkResult g_CreateDebugReportCallback(VkInstance instance,
                                         const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
                                         const VkAllocationCallbacks* pAllocator,
                                         VkDebugReportCallbackEXT* pCallback) 
    {
        PFN_vkCreateDebugReportCallbackEXT func = (PFN_vkCreateDebugReportCallbackEXT)(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));
        if (func != nullptr)
        {
            return func(instance, pCreateInfo, pAllocator, pCallback);
        }

        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    void g_DestroyDebugReportCallback(VkInstance instance,
                                      VkDebugReportCallbackEXT callback,
                                      const VkAllocationCallbacks* pAllocator) 
    {
        PFN_vkDestroyDebugReportCallbackEXT func = (PFN_vkDestroyDebugReportCallbackEXT)(vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT"));
        if (func != nullptr)
        {
            func(instance, callback, pAllocator);
        }
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL g_DebugReportCallback(VkDebugReportFlagsEXT flags,
                                                                VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location,
                                                                int32_t code, const char* layerPrefix, const char* message, void* userData) 
    {
        Util_LogError("*********************** g_DebugReportCallback: %s", message);

        return VK_FALSE;
    }


    VulkanInstance::VulkanInstance()
        : m_vkInstance(VK_NULL_HANDLE)
        , m_vkDebugReport(VK_NULL_HANDLE)
        , m_nPreferredVendorID(-1)
        , m_pVkPhysicalDeviceFeatures2(nullptr)
        , m_nDesiredNumSwapChainImages(3)
        , m_pDevice(nullptr)
        , m_eSwapChainImagePixelFormat(Vulkan_PixelFormat_BYTE_A8R8G8B8_UNORM)
    {
    #if UTIL_DEBUG == 1
        m_bIsEnableValidationLayers = false;
    #else
        m_bIsEnableValidationLayers = true;
    #endif
    }

    VulkanInstance::~VulkanInstance()
    {
        Destroy();
    }

    void VulkanInstance::Destroy()
    {
        destroyReportCallbackInfo();
        UTIL_DELETE(m_pDevice)
        if (m_vkInstance != VK_NULL_HANDLE)
        {
            vkDestroyInstance(m_vkInstance, nullptr);
        }
        m_vkInstance = VK_NULL_HANDLE;
        
        Util_LogInfo("VulkanInstance::Destroy: Destroy success !");
    }

    bool VulkanInstance::Init()
    {

        return true;
    }

    bool VulkanInstance::createInstance()
    {
        //1> LoadVulkanLibrary
        //if (!VulkanLauncher::GetPlatform()->LoadVulkanLibrary())
        // {
        //     Util_LogError("*********************** VulkanInstance::Init: 1> LoadVulkanLibrary failed !");
        //     return false;
        // }
        Util_LogInfo("VulkanInstance::Init: 1> LoadVulkanLibrary success !");

        //2> createInstance
        if (!createInstance())
        {
            Util_LogError("*********************** VulkanInstance::Init: 2> createInstance failed !");
            return false;
        }
        Util_LogInfo("VulkanInstance::Init: 2> createInstance success !");

        //3> createDebugReport
        if (!createDebugReport())
        {
            Util_LogError("*********************** VulkanInstance::Init: 3> createDebugReport failed !");
            return false;
        }
        Util_LogInfo("VulkanInstance::Init: 3> createDebugReport success, Enable Debug: [%s] ", m_bIsEnableValidationLayers ? "true" : "false");

        //4> createDevice
        if (!createDevice())
        {
            Util_LogError("*********************** VulkanInstance::Init: 4> createDevice failed !");
            return false;
        }
        Util_LogInfo("VulkanInstance::Init: 4> createDevice success !");

        return true;
    }

    bool VulkanInstance::createDebugReport()
    {
        if (!m_bIsEnableValidationLayers)
            return true;

        VkDebugReportCallbackCreateInfoEXT createInfo = createReportCallbackInfo();
        if (!Util_CheckVkResult(g_CreateDebugReportCallback(m_vkInstance, &createInfo, nullptr, &m_vkDebugReport), "g_CreateDebugReportCallback"))
        {
            Util_LogError("*********************** VulkanInstance::createDebugReport: g_CreateDebugReportCallback failed !");
            return false;
        }

        return true;
    }

    bool VulkanInstance::createDevice()
    {

        return true;
    }

    void VulkanInstance::getInstanceLayersAndExtensions(bool bIsEnableValidationLayers,
                                                        ConstCharPtrVector& outInstanceLayers, 
                                                        ConstCharPtrVector& outInstanceExtensions)
    {

    }

    VkDebugReportCallbackCreateInfoEXT VulkanInstance::createReportCallbackInfo()
    {
        VkDebugReportCallbackCreateInfoEXT createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
        createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT |
                           VK_DEBUG_REPORT_WARNING_BIT_EXT |
                           VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
        createInfo.pUserData = reinterpret_cast<void *>(this);
        createInfo.pfnCallback = g_DebugReportCallback;

        return createInfo;
    }

    void VulkanInstance::destroyReportCallbackInfo()
    {
        if (m_bIsEnableValidationLayers &&
            m_vkDebugReport != VK_NULL_HANDLE)
        {
            g_DestroyDebugReportCallback(m_vkInstance, m_vkDebugReport, nullptr);
        }
        m_vkDebugReport = VK_NULL_HANDLE;
    }

}; //LostPeter