/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-19
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanInstance.h"
#include "../include/VulkanDebug.h"
#include "../include/VulkanVolk.h"
#include "../include/VulkanDevice.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanInstance::VulkanInstance()
        : m_vkInstance(VK_NULL_HANDLE)
        , m_nPreferredVendorID(-1)
        , m_pVkPhysicalDeviceFeatures2(nullptr)
        , m_pVolk(nullptr)
        , m_pDebug(nullptr)
        , m_pDevice(nullptr)
    {
    #if F_DEBUG == 1
        m_bIsEnableValidationLayers = true;
    #else
        m_bIsEnableValidationLayers = false;
    #endif

        //glfw Extensions
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = nullptr;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        if (glfwExtensions != nullptr)
        {
            ConstCharPtrVector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
            size_t count_glfw = extensions.size();
            for (size_t i = 0; i < count_glfw; i++)
            {
                AddAppInstanceExtensions(extensions[i]);
            }
        }
    }

    VulkanInstance::~VulkanInstance()
    {
        Destroy();
    }

    void VulkanInstance::Destroy()
    {
        destroyDebug();
        F_DELETE(m_pDevice)
        DestroyVkInstance(m_vkInstance);
        m_vkInstance = VK_NULL_HANDLE;
        destroyVolk();
        
        F_LogInfo("VulkanInstance::Destroy: Destroy success !");
    }
    void VulkanInstance::destroyDebug()
    {
        F_DELETE(m_pDebug)
    }
    void VulkanInstance::destroyVolk()
    {
        F_DELETE(m_pVolk)
    }

    bool VulkanInstance::Init()
    {
        //1> createVolk
        if (!createVolk())
        {
            F_LogError("*********************** VulkanInstance::Init: 1> createVolk failed !");
            return false;
        }
        F_LogInfo("VulkanInstance::Init: 1> createVolk success !");

        //2> createInstance
        if (!createInstance())
        {
            F_LogError("*********************** VulkanInstance::Init: 2> createInstance failed !");
            return false;
        }
        F_LogInfo("VulkanInstance::Init: 2> createInstance success !");

        //3> createDebug
        if (!createDebug())
        {
            F_LogError("*********************** VulkanInstance::Init: 3> createDebug failed !");
            return false;
        }
        F_LogInfo("VulkanInstance::Init: 3> createDebug success, Enable Debug: [%s] ", m_bIsEnableValidationLayers ? "true" : "false");

        //4> createDevice
        if (!createDevice())
        {
            F_LogError("*********************** VulkanInstance::Init: 4> createDevice failed !");
            return false;
        }
        F_LogInfo("VulkanInstance::Init: 4> createDevice success !");

        return true;
    }
    bool VulkanInstance::createVolk()
    {
        m_pVolk = new VulkanVolk;
        if (!m_pVolk->VolkInitialize())
        {
            return false;
        }
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
            F_LogInfo("VulkanInstance::createInstance: Using app instance extensions count: %d", countAppInstanceExtensions);
            for (int32 i = 0; i < countAppInstanceExtensions; ++i)
            {
                m_aInstanceExtensions.push_back(m_aAppInstanceExtensions[i]);
                F_LogInfo("VulkanInstance::createInstance: Using app instance extension: %s", m_aAppInstanceExtensions[i]);
            }
        }

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "LostPeterPluginRendererVulkan";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "LostPeter Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    #if F_PLATFORM == F_PLATFORM_iOS || F_PLATFORM == F_PLATFORM_ANDROID
        appInfo.apiVersion         = VK_API_VERSION_1_0;
    #else
        appInfo.apiVersion         = VK_API_VERSION_1_1;
    #endif

        VkInstanceCreateInfo instanceCreateInfo;
        E_ZeroStruct(instanceCreateInfo, VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO);
        instanceCreateInfo.pApplicationInfo = &appInfo;
        instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_aInstanceExtensions.size());
        instanceCreateInfo.ppEnabledExtensionNames = m_aInstanceExtensions.size() > 0 ? m_aInstanceExtensions.data() : nullptr;
        instanceCreateInfo.enabledLayerCount = uint32_t(m_aInstanceLayers.size());
	    instanceCreateInfo.ppEnabledLayerNames = m_aInstanceLayers.size() > 0 ? m_aInstanceLayers.data() : nullptr;

        VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_vkInstance);
        if (result == VK_ERROR_INCOMPATIBLE_DRIVER) 
        {
            F_LogError("*********************** VulkanInstance::createInstance: Can not find a compatible Vulkan driver (ICD) !");
            return false;
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

            F_LogError("*********************** VulkanInstance::createInstance: Vulkan driver doesn't contain specified extensions: %s !", missingExtensions.c_str());
        }
        else if (result != VK_SUCCESS) 
        {
            F_LogError("*********************** VulkanInstance::createInstance: Create vulkan instance failed !");
            return false;
        }
        else 
        {
            F_LogInfo("VulkanInstance::createInstance: Create vulkan instance success !");
        }
        
        m_pVolk->VolkLoadInstanceOnly(m_vkInstance);
        return true;
    }
    bool VulkanInstance::createDebug()
    {
        if (!m_bIsEnableValidationLayers)
            return true;

        m_pDebug = new VulkanDebug(this); 
        return true;
    }
    bool VulkanInstance::createDevice()
    {
        uint32_t physicalDevicesCount = 0;
        VkResult result = vkEnumeratePhysicalDevices(m_vkInstance, &physicalDevicesCount, nullptr);
        if (result == VK_ERROR_INITIALIZATION_FAILED)
        {
            F_LogError("*********************** VulkanInstance::createDevice: Can not find a compatible Vulkan device or driver !");
            return false;
        }
        if (physicalDevicesCount == 0)
        {
            F_LogError("*********************** VulkanInstance::createDevice: Can not enumerate physical devices, count is 0 !");
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
                F_DELETE(aDevices[i])
            }
            aDevices.clear();
        }
        else
        {
            F_LogError("*********************** VulkanInstance::createDevice: Can not find device !");
            deviceIndex = -1;
            return false;
        }

        size_t count_extension = m_aAppDeviceExtensions.size(); 
        for (size_t i = 0; i < count_extension; ++i)
        {
            m_pDevice->AddAppDeviceExtensions(m_aAppDeviceExtensions[i]);
        }

        m_pDevice->SetVkPhysicalDeviceFeatures2(m_pVkPhysicalDeviceFeatures2);
        if (!m_pDevice->Init(deviceIndex, m_bIsEnableValidationLayers))
        {
            F_LogError("*********************** VulkanInstance::createDevice: m_pDevice->Init failed !");
            return false;
        }
        
        return true; 
    }
    

    /////////////////////////////////////// Vulkan Function Wrapper ///////////////////////////////////////
    //////////////////// VkInstance /////////////////////
    void VulkanInstance::DestroyVkInstance(const VkInstance& vkInstance)
    {
        if (vkInstance != VK_NULL_HANDLE)
        {
            vkDestroyInstance(vkInstance, nullptr);
        }
    }

    
    //////////////////// VkSurfaceKHR ///////////////////
    bool VulkanInstance::CreateVkSurfaceKHR(GLFWwindow* pWindow,
                                            VkSurfaceKHR& vkSurfaceKHR)
    {
        VkResult result = glfwCreateWindowSurface(this->m_vkInstance, pWindow, nullptr, &vkSurfaceKHR);
        if (result != VK_SUCCESS)
        {
            F_LogError("*********************** VulkanInstance::CreateVkSurfaceKHR: Failed to create VkSurfaceKHR !");
            return false;
        }
        return true;
    }
    void VulkanInstance::DestroyVkSurfaceKHR(const VkSurfaceKHR& vkSurfaceKHR)
    {
        if (vkSurfaceKHR != VK_NULL_HANDLE)
        {
            vkDestroySurfaceKHR(this->m_vkInstance, vkSurfaceKHR, nullptr);
        }
    }


}; //LostPeterPluginRendererVulkan