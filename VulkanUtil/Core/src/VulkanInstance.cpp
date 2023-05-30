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

    bool VulkanInstance::createInstance()
    {
        getInstanceLayersAndExtensions(m_bIsEnableValidationLayers, 
                                       m_aInstanceLayers, 
                                       m_aInstanceExtensions);

        int countAppInstanceExtensions = (int)m_aAppInstanceExtensions.size();
        if (countAppInstanceExtensions > 0)
        {
            Util_LogInfo("VulkanInstance::createInstance: Using app instance extensions count: %d", countAppInstanceExtensions);
            for (int32 i = 0; i < countAppInstanceExtensions; ++i)
            {
                m_aInstanceExtensions.push_back(m_aAppInstanceExtensions[i]);
                Util_LogInfo("VulkanInstance::createInstance: Using app instance extension: %s", m_aAppInstanceExtensions[i]);
            }
        }

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "LostPeterVulkan";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "LostPeter Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    #if UTIL_PLATFORM == UTIL_PLATFORM_IOS || UTIL_PLATFORM == UTIL_PLATFORM_ANDROID
        appInfo.apiVersion         = VK_API_VERSION_1_0;
    #else
        appInfo.apiVersion         = VK_API_VERSION_1_1;
    #endif

        VkInstanceCreateInfo instanceCreateInfo;
        Util_ZeroStruct(instanceCreateInfo, VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO);
        instanceCreateInfo.pApplicationInfo = &appInfo;
        instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_aInstanceExtensions.size());
        instanceCreateInfo.ppEnabledExtensionNames = m_aInstanceExtensions.size() > 0 ? m_aInstanceExtensions.data() : nullptr;
        instanceCreateInfo.enabledLayerCount = uint32_t(m_aInstanceLayers.size());
	    instanceCreateInfo.ppEnabledLayerNames = m_aInstanceLayers.size() > 0 ? m_aInstanceLayers.data() : nullptr;

        VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_vkInstance);
        if (result == VK_ERROR_INCOMPATIBLE_DRIVER) 
        {
            Util_LogError("*********************** VulkanInstance::createInstance: Can not find a compatible Vulkan driver (ICD) !");
        }
        else if (result == VK_ERROR_EXTENSION_NOT_PRESENT)
        {
            String missingExtensions;
            uint32 propertyCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, nullptr);
            std::vector<VkExtensionProperties> properties(propertyCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, properties.data());

            for (const char* extension : m_aInstanceExtensions)
            {
                bool found = false;
                for (uint32 i = 0; i < propertyCount; ++i)
                {
                    const char* propExtension = properties[i].extensionName;
                    if (strcmp(propExtension, extension) == 0) 
                    {
                        found = true;
                        break;
                    }
                }
                if (!found) 
                {
                    String extensionStr(extension);
                    missingExtensions += extensionStr + "\n";
                }
            }

            Util_LogError("*********************** VulkanInstance::createInstance: Vulkan driver doesn't contain specified extensions: %s !", missingExtensions.c_str());
        }
        else if (result != VK_SUCCESS) 
        {
            Util_LogError("*********************** VulkanInstance::createInstance: Create vulkan instance failed !");
        }
        else 
        {
            Util_LogInfo("VulkanInstance::createInstance: Create vulkan instance success !");
        }
        
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
        uint32_t physicalDevicesCount = 0;
        VkResult result = vkEnumeratePhysicalDevices(m_vkInstance, &physicalDevicesCount, nullptr);
        if (result == VK_ERROR_INITIALIZATION_FAILED)
        {
            Util_LogError("*********************** VulkanInstance::createDevice: Can not find a compatible Vulkan device or driver !");
            return false;
        }
        if (physicalDevicesCount == 0)
        {
            Util_LogError("*********************** VulkanInstance::createDevice: Can not enumerate physical devices, count is 0 !");
            return false;
        }

        std::vector<VkPhysicalDevice> physicalDevices(physicalDevicesCount);
        vkEnumeratePhysicalDevices(m_vkInstance, &physicalDevicesCount, physicalDevices.data());

        struct DeviceInfo
        {
            VulkanDevice* pDevice;
            int32 deviceIndex;
        };
        std::vector<DeviceInfo> aDevices_Discrete;
	    std::vector<DeviceInfo> aDevices_Integrated;
        std::vector<VulkanDevice*> aDevices;

        for (uint32 i = 0; i < physicalDevicesCount; i++)
        {
            VulkanDevice* pDevice = new VulkanDevice(this, physicalDevices[i]);
            bool isDiscrete = pDevice->QueryGPU(i);
            if (isDiscrete) 
            {
                aDevices_Discrete.push_back({pDevice, (int32)i});
            }
            else 
            {
                aDevices_Integrated.push_back({pDevice, (int32)i});
            }
            aDevices.push_back(pDevice);
        }

        for (size_t i = 0; i < aDevices_Integrated.size(); ++i) 
        {
            aDevices_Discrete.push_back(aDevices_Integrated[i]);
        }

        int32 deviceIndex = -1;
        int32 countDevice = (int)aDevices_Discrete.size();
        if (countDevice > 0)
        {
            if (countDevice > 1 && m_nPreferredVendorID != -1)
            {
                for (int32 i = 0; i < countDevice; ++i)
                {
                    if (aDevices_Discrete[i].pDevice->GetVkPhysicalDeviceProperties().vendorID == (uint32_t)m_nPreferredVendorID)
                    {
                        m_pDevice = aDevices_Discrete[i].pDevice;
                        deviceIndex = aDevices_Discrete[i].deviceIndex;
                        break;
                    }
                }
            }
            if (deviceIndex == -1)
            {
                m_pDevice = aDevices_Discrete[0].pDevice;
                deviceIndex = aDevices_Discrete[0].deviceIndex;
            }

            size_t count_dev = aDevices.size();
            for (size_t i = 0; i < count_dev; i++)
            {
                if (aDevices[i] == m_pDevice)
                    continue;
                UTIL_DELETE(aDevices[i])
            }
            aDevices.clear();
        }
        else
        {
            Util_LogError("*********************** VulkanInstance::createDevice: Can not find device !");
            deviceIndex = -1;
            return false;
        }

        size_t count_extension = m_aAppDeviceExtensions.size(); 
        for (size_t i = 0; i < count_extension; ++i)
        {
            m_pDevice->AddAppDeviceExtensions(m_aAppDeviceExtensions[i]);
        }

        m_pDevice->SetVkPhysicalDeviceFeatures2(m_pVkPhysicalDeviceFeatures2);
        return m_pDevice->Init(deviceIndex, m_bIsEnableValidationLayers);
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