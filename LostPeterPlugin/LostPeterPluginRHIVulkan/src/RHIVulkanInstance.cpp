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

#include "../include/RHIVulkanInstance.h"
#include "../include/RHIVulkanDebug.h"
#include "../include/RHIVulkanVolk.h"
#include "../include/RHIVulkanPhysicalDevice.h"
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanInstance::RHIVulkanInstance()
        : m_vkInstance(VK_NULL_HANDLE)
        , m_nPreferredVendorID(-1)
        , m_pVolk(nullptr)
        , m_pDebug(nullptr)
        , m_pVulkanPhysicalDevice(nullptr)
        , m_pVulkanDevice(nullptr)
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

    RHIVulkanInstance::~RHIVulkanInstance()
    {
        Destroy();
    }

    void RHIVulkanInstance::Destroy()
    {
        destroyPhysicalDevice();
        destroyDebug();
        DestroyVkInstance(m_vkInstance);
        m_vkInstance = VK_NULL_HANDLE;
        destroyVolk();
        
        F_LogInfo("RHIVulkanInstance::Destroy: Destroy success !");
    }
    void RHIVulkanInstance::destroyDebug()
    {
        F_DELETE(m_pDebug)
    }
    void RHIVulkanInstance::destroyVolk()
    {
        F_DELETE(m_pVolk)
    }
    void RHIVulkanInstance::destroyPhysicalDevice()
    {
        size_t count = m_aVulkanPhysicalDevices.size();
        for (size_t i = 0; i < count; i++)
        {
            F_DELETE(m_aVulkanPhysicalDevices[i])
        }
        m_aVulkanPhysicalDevices.clear();
    }

    bool RHIVulkanInstance::Init()
    {
        //1> createVolk
        if (!createVolk())
        {
            F_LogError("*********************** RHIVulkanInstance::Init: 1> createVolk failed !");
            return false;
        }
        F_LogInfo("RHIVulkanInstance::Init: 1> createVolk success !");

        //2> createInstance
        if (!createInstance())
        {
            F_LogError("*********************** RHIVulkanInstance::Init: 2> createInstance failed !");
            return false;
        }
        F_LogInfo("RHIVulkanInstance::Init: 2> createInstance success !");

        //3> createDebug
        if (!createDebug())
        {
            F_LogError("*********************** RHIVulkanInstance::Init: 3> createDebug failed !");
            return false;
        }
        F_LogInfo("RHIVulkanInstance::Init: 3> createDebug success, Enable Debug: [%s] ", m_bIsEnableValidationLayers ? "true" : "false");

        //4> createPhysicalDevice
        if (!createPhysicalDevice())
        {
            F_LogError("*********************** RHIVulkanInstance::Init: 4> createPhysicalDevice failed !");
            return false;
        }
        F_LogInfo("RHIVulkanInstance::Init: 4> createPhysicalDevice success !");

        return true;
    }
    bool RHIVulkanInstance::createVolk()
    {
        m_pVolk = new RHIVulkanVolk;
        if (!m_pVolk->VolkInitialize())
        {
            return false;
        }
        return true;
    }
    bool RHIVulkanInstance::createInstance()
    {
        getInstanceLayersAndExtensions(m_bIsEnableValidationLayers, 
                                       m_aInstanceLayers, 
                                       m_aInstanceExtensions);

        int countAppInstanceExtensions = (int)m_aAppInstanceExtensions.size();
        if (countAppInstanceExtensions > 0)
        {
            F_LogInfo("RHIVulkanInstance::createInstance: Using app instance extensions count: %d", countAppInstanceExtensions);
            for (int32 i = 0; i < countAppInstanceExtensions; ++i)
            {
                m_aInstanceExtensions.push_back(m_aAppInstanceExtensions[i]);
                F_LogInfo("RHIVulkanInstance::createInstance: Using app instance extension: %s", m_aAppInstanceExtensions[i]);
            }
        }

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "LostPeterPluginRHIVulkan";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "LostPeter Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    #if F_PLATFORM == F_PLATFORM_iOS || F_PLATFORM == F_PLATFORM_ANDROID
        appInfo.apiVersion         = VK_API_VERSION_1_0;
    #else
        appInfo.apiVersion         = VK_API_VERSION_1_1;
    #endif

        VkInstanceCreateInfo instanceCreateInfo;
        RHI_ZeroStruct(instanceCreateInfo, VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO);
        instanceCreateInfo.pApplicationInfo = &appInfo;
        instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_aInstanceExtensions.size());
        instanceCreateInfo.ppEnabledExtensionNames = m_aInstanceExtensions.size() > 0 ? m_aInstanceExtensions.data() : nullptr;
        instanceCreateInfo.enabledLayerCount = uint32_t(m_aInstanceLayers.size());
	    instanceCreateInfo.ppEnabledLayerNames = m_aInstanceLayers.size() > 0 ? m_aInstanceLayers.data() : nullptr;

        VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_vkInstance);
        if (result == VK_ERROR_INCOMPATIBLE_DRIVER) 
        {
            F_LogError("*********************** RHIVulkanInstance::createInstance: Can not find a compatible Vulkan driver (ICD) !");
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

            F_LogError("*********************** RHIVulkanInstance::createInstance: Vulkan driver doesn't contain specified extensions: %s !", missingExtensions.c_str());
        }
        else if (result != VK_SUCCESS) 
        {
            F_LogError("*********************** RHIVulkanInstance::createInstance: Create vulkan instance failed !");
            return false;
        }
        else 
        {
            F_LogInfo("RHIVulkanInstance::createInstance: Create vulkan instance success !");
        }
        
        m_pVolk->VolkLoadInstanceOnly(m_vkInstance);
        return true;
    }
    bool RHIVulkanInstance::createDebug()
    {
        if (!m_bIsEnableValidationLayers)
            return true;

        m_pDebug = new RHIVulkanDebug(this); 
        return true;
    }
    bool RHIVulkanInstance::createPhysicalDevice()
    {
        destroyPhysicalDevice();

        uint32_t physicalDevicesCount = 0;
        VkResult result = vkEnumeratePhysicalDevices(m_vkInstance, &physicalDevicesCount, nullptr);
        if (result == VK_ERROR_INITIALIZATION_FAILED)
        {
            F_LogError("*********************** RHIVulkanInstance::createPhysicalDevice: Can not find a compatible Vulkan device or driver !");
            return false;
        }
        if (physicalDevicesCount == 0)
        {
            F_LogError("*********************** RHIVulkanInstance::createPhysicalDevice: Can not enumerate physical devices, count is 0 !");
            return false;
        }

        std::vector<VkPhysicalDevice> physicalDevices(physicalDevicesCount);
        vkEnumeratePhysicalDevices(m_vkInstance, &physicalDevicesCount, physicalDevices.data());

        for (uint32 i = 0; i < physicalDevicesCount; i++)
        {
            RHIVulkanPhysicalDevice* pVulkanPhysicalDevice = new RHIVulkanPhysicalDevice(this, physicalDevices[i], i);
            m_aVulkanPhysicalDevices.push_back(pVulkanPhysicalDevice);
        }
        
        return m_aVulkanPhysicalDevices.size() > 0;
    }

    RHIType RHIVulkanInstance::GetRHIType()
    {
        return RHIType::RHI_Vulkan;
    }

    uint32 RHIVulkanInstance::GetPhysicalDeviceCount()
    {
        return (uint32)m_aVulkanPhysicalDevices.size();
    }

    RHIPhysicalDevice* RHIVulkanInstance::GetPhysicalDevice(uint32 nIndex)
    {
        F_Assert("RHIVulkanInstance::GetPhysicalDevice" && nIndex >= 0 && nIndex < (uint32)(m_aVulkanPhysicalDevices.size()));
        return m_aVulkanPhysicalDevices[nIndex];
    }

    RHIDevice* RHIVulkanInstance::RequestDevice(const RHIDeviceCreateInfo& createInfo)
    {
        SetPreferredVendorID(createInfo.nPreferredVendorID);

        m_pVulkanPhysicalDevice = nullptr;
        uint32 countDevice = (uint32)m_aVulkanPhysicalDevices.size();
        if (countDevice > 0)
        {
            if (countDevice > 1 && m_nPreferredVendorID != -1)
            {
                for (uint32 i = 0; i < countDevice; i++)
                {
                    RHIVulkanPhysicalDevice* pVulkanPhysicalDevice = m_aVulkanPhysicalDevices[i];
                    if (pVulkanPhysicalDevice->GetVkPhysicalDeviceProperties().vendorID == (uint32_t)m_nPreferredVendorID)
                    {
                        m_pVulkanPhysicalDevice = pVulkanPhysicalDevice;
                        break;
                    }
                }
            }
            if (m_pVulkanPhysicalDevice == nullptr)
            {
                m_pVulkanPhysicalDevice = m_aVulkanPhysicalDevices[0];
            }
        }
        else
        {
            F_LogError("*********************** RHIVulkanInstance::RequestDevice: Can not find PhysicalDevice !");
        }

        m_pPhysicalDevice = m_pVulkanPhysicalDevice;
        m_pVulkanDevice = nullptr;
        if (m_pVulkanPhysicalDevice != nullptr)
        {
            m_pVulkanDevice = (RHIVulkanDevice*)m_pVulkanPhysicalDevice->RequestDevice(createInfo);
        }
        return m_pVulkanDevice;
    }





    /////////////////////////////////////// Vulkan Function Wrapper ///////////////////////////////////////
    //////////////////// VkInstance /////////////////////
    void RHIVulkanInstance::DestroyVkInstance(const VkInstance& vkInstance)
    {
        if (vkInstance != VK_NULL_HANDLE)
        {
            vkDestroyInstance(vkInstance, nullptr);
        }
    }

    
    //////////////////// VkSurfaceKHR ///////////////////
    bool RHIVulkanInstance::CreateVkSurfaceKHR(GLFWwindow* pWindow,
                                               VkSurfaceKHR& vkSurfaceKHR)
    {
        VkResult result = glfwCreateWindowSurface(this->m_vkInstance, pWindow, nullptr, &vkSurfaceKHR);
        if (result != VK_SUCCESS)
        {
            F_LogError("*********************** RHIVulkanInstance::CreateVkSurfaceKHR: Failed to create VkSurfaceKHR !");
            return false;
        }
        return true;
    }
    void RHIVulkanInstance::DestroyVkSurfaceKHR(const VkSurfaceKHR& vkSurfaceKHR)
    {
        if (vkSurfaceKHR != VK_NULL_HANDLE)
        {
            vkDestroySurfaceKHR(this->m_vkInstance, vkSurfaceKHR, nullptr);
        }
    }


}; //LostPeterPluginRHIVulkan

