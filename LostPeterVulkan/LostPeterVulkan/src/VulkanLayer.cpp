/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-03-19
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/PreInclude.h"
#include "../include/VulkanDefine.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    struct VulkanLayerExtension
    {
        VkLayerProperties layerProps;
        std::vector<VkExtensionProperties> extensionProps;

        VulkanLayerExtension();

        void AddUniqueExtensionNames(StringVector& outExtensions);
        void AddUniqueExtensionNames(ConstCharPtrVector& outExtensions);
    };
    typedef std::vector<VulkanLayerExtension> VulkanLayerExtensionVector;

    VulkanLayerExtension::VulkanLayerExtension()
    {
        memset(&layerProps, 0, sizeof(VkLayerProperties));
    }
    void VulkanLayerExtension::AddUniqueExtensionNames(StringVector& outExtensions)
    {
        for (int32 i = 0; i < extensionProps.size(); ++i) 
        {
            FUtilString::AddUnique(outExtensions, extensionProps[i].extensionName);
        }
    }
    void VulkanLayerExtension::AddUniqueExtensionNames(ConstCharPtrVector& outExtensions)
    {
        for (int32 i = 0; i < extensionProps.size(); ++i) 
        {
            FUtilString::AddUnique(outExtensions, extensionProps[i].extensionName);
        }
    }


    static const char* g_szValidationLayersInstance[] = 
    {
        
        
    #if F_PLATFORM == F_PLATFORM_iOS
        
    #elif F_PLATFORM == F_PLATFORM_ANDROID
        
    #elif F_PLATFORM == F_PLATFORM_WINDOW
        "VK_LAYER_KHRONOS_validation",
        
    #elif F_PLATFORM == F_PLATFORM_MAC

    #elif F_PLATFORM == F_PLATFORM_LINUX
        
    #else
        #pragma error "UnKnown platform! Abort! Abort!"
    #endif
        nullptr,
    };

    static const char* g_szValidationLayersDevice[] = 
    {
    #if F_PLATFORM == F_PLATFORM_iOS
        "MoltenVK",
    #elif F_PLATFORM == F_PLATFORM_ANDROID

    #elif F_PLATFORM == F_PLATFORM_WINDOW
        
    #elif F_PLATFORM == F_PLATFORM_MAC
        
    #elif F_PLATFORM == F_PLATFORM_LINUX
        
    #else
        #pragma error "UnKnown platform! Abort! Abort!"
    #endif
        nullptr,
    };

    static const char* g_szInstanceExtensions[] =
    {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,

    #if F_PLATFORM == F_PLATFORM_iOS
        "VK_MVK_ios_surface",
        "MoltenVK",

    #elif F_PLATFORM == F_PLATFORM_ANDROID
        "VK_KHR_android_surface",

    #elif F_PLATFORM == F_PLATFORM_WINDOW
        "VK_KHR_win32_surface",
        
    #elif F_PLATFORM == F_PLATFORM_MAC
        "VK_MVK_macos_surface",
        VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
        
    #elif F_PLATFORM == F_PLATFORM_LINUX
        
    #else
        #pragma error "UnKnown platform! Abort! Abort!"
    #endif
        nullptr,
    };

    static const char* g_szDeviceExtensions[] =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_SAMPLER_MIRROR_CLAMP_TO_EDGE_EXTENSION_NAME,
        VK_KHR_MAINTENANCE1_EXTENSION_NAME,
        VK_KHR_MULTIVIEW_EXTENSION_NAME,
        VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME,
        VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME,
        VK_EXT_EXTENDED_DYNAMIC_STATE_3_EXTENSION_NAME,

    #if F_DEBUG == 1
        VK_EXT_DEBUG_MARKER_EXTENSION_NAME,
    #endif


    #if F_PLATFORM == F_PLATFORM_iOS
        
    #elif F_PLATFORM == F_PLATFORM_ANDROID

    #elif F_PLATFORM == F_PLATFORM_WINDOW
        
    #elif F_PLATFORM == F_PLATFORM_MAC

    #elif F_PLATFORM == F_PLATFORM_LINUX
        
    #else
        #pragma error "UnKnown platform! Abort! Abort!"
    #endif
        nullptr,
    };

    static void EnumerateInstanceExtensionProperties(const char* layerName, VulkanLayerExtension& outLayer)
    {
        uint32 count = 0;
        vkEnumerateInstanceExtensionProperties(layerName, &count, nullptr);
        outLayer.extensionProps.resize(count);
        vkEnumerateInstanceExtensionProperties(layerName, &count, outLayer.extensionProps.data());
    }

    static void EnumerateDeviceExtensionProperties(VkPhysicalDevice device, const char* layerName, VulkanLayerExtension& outLayer)
    {
        uint32 count = 0;
        vkEnumerateDeviceExtensionProperties(device, layerName, &count, nullptr);
        outLayer.extensionProps.resize(count);
        vkEnumerateDeviceExtensionProperties(device, layerName, &count, outLayer.extensionProps.data());
    }

    static int32 FindLayerIndexInList(const VulkanLayerExtensionVector& layers, const char* layerName)
    {
        for (int32 i = 0; i < layers.size(); ++i) 
        {
            F_LogInfo("VulkanWindow::FindLayerIndexInList: Compare: src: [%s], dst: [%s] !", layers[i].layerProps.layerName, layerName);
            if (strcmp(layers[i].layerProps.layerName, layerName) == 0) {
                return i;
            }
        }
        return -1;
    }

    static bool FindLayerInList(const VulkanLayerExtensionVector& layers, const char* layerName)
    {
        return FindLayerIndexInList(layers, layerName) != -1;
    }

    static bool FindLayerExtensionInList(const VulkanLayerExtensionVector& layers, const char* extensionName, const char*& foundLayer)
    {
        for (int32 i = 0; i < layers.size(); ++i) 
        {
            for (int32 j = 0; j < layers[i].extensionProps.size(); ++j)
            {
                if (strcmp(layers[i].extensionProps[j].extensionName, extensionName) == 0)
                {
                    foundLayer = layers[i].layerProps.layerName;
                    return true;
                }
            }
        }
        return false;
    }

    static bool FindLayerExtensionInList(const VulkanLayerExtensionVector& layers, const char* extensionName)
    {
        const char* dummy = nullptr;
        return FindLayerExtensionInList(layers, extensionName, dummy);
    }

    static void TrimDuplicates(ConstCharPtrVector& arr)
    {
        for (int32 i = (int32)arr.size() - 1; i >= 0; --i)
        {
            bool found = false;
            for (int32 j = i - 1; j >= 0; --j)
            {
                if (strcmp(arr[i], arr[j]) == 0) 
                {
                    found = true;
                    break;
                }
            }

            if (found) 
            {
                arr.erase(arr.begin() + i);
            }
        }
    }

    void VulkanWindow::getInstanceLayersAndExtensions(bool bIsEnableValidationLayers,
                                                      ConstCharPtrVector& outInstanceLayers, 
                                                      ConstCharPtrVector& outInstanceExtensions)
    {
        //1> Enum global layer extensions
        VulkanLayerExtensionVector globalLayerExtensions(1);
	    EnumerateInstanceExtensionProperties(nullptr, globalLayerExtensions[0]);

        //2> Found unique extensions
        StringVector foundUniqueExtensions;
        for (size_t i = 0; i < globalLayerExtensions[0].extensionProps.size(); ++i) 
        {
            FUtilString::AddUnique(foundUniqueExtensions, globalLayerExtensions[0].extensionProps[i].extensionName);
        }

        //3> Enum instance layer
        uint32 instanceLayerCount = 0;
        vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
        std::vector<VkLayerProperties> globalLayerProperties(instanceLayerCount);
        vkEnumerateInstanceLayerProperties(&instanceLayerCount, globalLayerProperties.data());

        //3> Found unique layer
        StringVector foundUniqueLayers;
        for (size_t i = 0; i < globalLayerProperties.size(); ++i) 
        {
            VulkanLayerExtension layer;
            layer.layerProps = globalLayerProperties[i];
            EnumerateInstanceExtensionProperties(globalLayerProperties[i].layerName, layer);
            layer.AddUniqueExtensionNames(foundUniqueExtensions);
            FUtilString::AddUnique(foundUniqueLayers, globalLayerProperties[i].layerName);
            globalLayerExtensions.push_back(layer);
        }

        //4> Unique layer
        int layerCount = (int)foundUniqueLayers.size();
        F_LogInfo("VulkanWindow::getInstanceLayersAndExtensions: Found unique layer count: [%d] !", layerCount);
        for (int i = 0; i < layerCount; i++)
        {
            const String& nameLayer = foundUniqueLayers[i];
            F_LogInfo("VulkanWindow::getInstanceLayersAndExtensions: Layer index: [%d], Layer name: [%s], Layer count: [%d] !", i, nameLayer.c_str(), layerCount);
        }

        //5> Unique extensions
        int extensionsCount = (int)foundUniqueExtensions.size();
        F_LogInfo("VulkanWindow::getInstanceLayersAndExtensions: Found unique instance extension count: [%d] !", extensionsCount);
        for (int i = 0; i < extensionsCount; i++) 
        {
            const String& nameExtension = foundUniqueExtensions[i];
            F_LogInfo("VulkanWindow::getInstanceLayersAndExtensions: Instance Extension index: [%d], Extension name: [%s], Extension count: [%d] !", i, nameExtension.c_str(), extensionsCount);
        }

        //6> bIsEnableValidationLayers
        if (bIsEnableValidationLayers)
        {
            for (int32 i = 0; g_szValidationLayersInstance[i] != nullptr; ++i) 
            {
                const char* currValidationLayer = g_szValidationLayersInstance[i];
                bool found = FindLayerInList(globalLayerExtensions, currValidationLayer);
                if (found) 
                {
                    outInstanceLayers.push_back(currValidationLayer);
                } 
                else 
                {
                    F_LogError("*********************** VulkanWindow::getInstanceLayersAndExtensions: Unable to find Vulkan instance validation layer [%s] !", currValidationLayer);
                }
            }

            if (FindLayerExtensionInList(globalLayerExtensions, VK_EXT_DEBUG_REPORT_EXTENSION_NAME)) 
            {
                outInstanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
            }
        }

        //7> outInstanceExtensions
        for (int32 i = 0; g_szInstanceExtensions[i] != nullptr; ++i) 
        {
            if (FindLayerExtensionInList(globalLayerExtensions, g_szInstanceExtensions[i])) 
            {
                outInstanceExtensions.push_back(g_szInstanceExtensions[i]);
            }
        }


        F_LogInfo("VulkanWindow::getInstanceLayersAndExtensions ***********************");
        {
            TrimDuplicates(outInstanceLayers);
            int countLayers = (int)outInstanceLayers.size();
            if (countLayers > 0) 
            {
                F_LogInfo("VulkanWindow::getInstanceLayersAndExtensions: Using instance layers count: [%d] !", (int32)countLayers);
                for (int i = 0; i < countLayers; i++)
                {
                    F_LogInfo("VulkanWindow::getInstanceLayersAndExtensions: Using instance layer index: [%d], name: [%s] !", i, outInstanceLayers[i]);
                }
            }
            else 
            {
                F_LogInfo("VulkanWindow::getInstanceLayersAndExtensions: Not using instance layers !");
            }

            TrimDuplicates(outInstanceExtensions);
            int countExtensions = (int)outInstanceExtensions.size();
            if (countExtensions > 0) 
            {
                F_LogInfo("VulkanWindow::getInstanceLayersAndExtensions: Using instance extensions count: [%d] !", countExtensions);
                for (int i = 0; i < countExtensions; i++)
                {
                    F_LogInfo("VulkanWindow::getInstanceLayersAndExtensions: Using instance extensions index: [%d], name: [%s] !", i, outInstanceExtensions[i]);
                }
            }
            else 
            {
                F_LogInfo("VulkanWindow::getInstanceLayersAndExtensions: Not using instance extensions !");
            }
        }
        F_LogInfo("VulkanWindow::getInstanceLayersAndExtensions ***********************");
    }

    void VulkanWindow::getDeviceLayersAndExtensions(bool bIsEnableValidationLayers,
                                                    ConstCharPtrVector& outDeviceLayers, 
                                                    ConstCharPtrVector& outDeviceExtensions)
    {
        uint32 count = 0;
        vkEnumerateDeviceLayerProperties(this->poPhysicalDevice, &count, nullptr);
        std::vector<VkLayerProperties> properties(count);
        vkEnumerateDeviceLayerProperties(this->poPhysicalDevice, &count, properties.data());

        std::vector<VulkanLayerExtension> deviceLayerExtensions(count + 1);
        for (size_t i = 1; i < deviceLayerExtensions.size(); ++i) 
        {
            deviceLayerExtensions[i].layerProps = properties[i - 1];
        }
        
        StringVector foundUniqueLayers;
        StringVector foundUniqueExtensions;
        for (size_t i = 0; i < deviceLayerExtensions.size(); ++i)
        {
            if (i == 0) 
            {
                EnumerateDeviceExtensionProperties(this->poPhysicalDevice, nullptr, deviceLayerExtensions[i]);
            }
            else 
            {
                FUtilString::AddUnique(foundUniqueLayers, deviceLayerExtensions[i].layerProps.layerName);
                EnumerateDeviceExtensionProperties(this->poPhysicalDevice, deviceLayerExtensions[i].layerProps.layerName, deviceLayerExtensions[i]);
            }
            
            deviceLayerExtensions[i].AddUniqueExtensionNames(foundUniqueExtensions);
        }

        int layerCount = (int)foundUniqueLayers.size();
        F_LogInfo("VulkanWindow::getDeviceLayersAndExtensions: Found unique layer count: %d", layerCount);
        for (int i = 0; i < layerCount; i++)
        {
            const String& nameLayer = foundUniqueLayers[i];
            F_LogInfo("VulkanWindow::getDeviceLayersAndExtensions: Layer index: %d, Layer name: %s, Layer count: %d", i, nameLayer.c_str(), layerCount);
        }

        int extensionsCount = (int)foundUniqueExtensions.size();
        F_LogInfo("VulkanWindow::getDeviceLayersAndExtensions: Found unique device extension count: %d", extensionsCount);
        for (int i = 0; i < extensionsCount; i++) 
        {
            const String& nameExtension = foundUniqueExtensions[i];
            F_LogInfo("VulkanWindow::getDeviceLayersAndExtensions: Device Extension index: %d, Extension name: %s, Extension count: %d", i, nameExtension.c_str(), extensionsCount);
        }

        if (bIsEnableValidationLayers)
        {
            for (uint32 i = 0; g_szValidationLayersDevice[i] != nullptr; ++i)
            {
                bool bValidationFound = false;
                const char* currValidationLayer = g_szValidationLayersDevice[i];
                for (int32 j = 1; j < deviceLayerExtensions.size(); ++j)
                {
                    if (strcmp(deviceLayerExtensions[j].layerProps.layerName, currValidationLayer) == 0)
                    {
                        bValidationFound = true;
                        outDeviceLayers.push_back(currValidationLayer);
                        break;
                    }
                }
                
                if (!bValidationFound) 
                {
                    F_LogError("*********************** VulkanWindow::getDeviceLayersAndExtensions: Unable to find Vulkan device validation layer '%s'", currValidationLayer);
                }
            }
        }

        ConstCharPtrVector availableExtensions;
        for (size_t i = 0; i < foundUniqueExtensions.size(); ++i) 
        {
            availableExtensions.push_back(foundUniqueExtensions[i].c_str());
        }

        for (size_t i = 0; i < outDeviceLayers.size(); ++i)
        {
            int32 findLayerIndex;
            for (findLayerIndex = 1; findLayerIndex < deviceLayerExtensions.size(); ++findLayerIndex)
            {
                if (strcmp(deviceLayerExtensions[findLayerIndex].layerProps.layerName, outDeviceLayers[i]) == 0) 
                {
                    break;
                }
            }
            
            if (findLayerIndex < deviceLayerExtensions.size()) 
            {
                deviceLayerExtensions[findLayerIndex].AddUniqueExtensionNames(availableExtensions);
            }
        }
        
        TrimDuplicates(availableExtensions);
        
        auto ListContains = [](const ConstCharPtrVector& arr, const char* name) -> bool
        {
            for (const char* element : arr)
            {
                if (strcmp(element, name) == 0) {
                    return true;
                }
            }
            return false;
        };
        
        for (uint32 i = 0; g_szDeviceExtensions[i] != nullptr; ++i)
        {
            if (ListContains(availableExtensions, g_szDeviceExtensions[i])) 
            {
                outDeviceExtensions.push_back(g_szDeviceExtensions[i]);
            }
        }
        
        F_LogInfo("VulkanWindow::getDeviceLayersAndExtensions ***********************");
        {
            size_t countLayers = outDeviceLayers.size();
            if (countLayers > 0)
            {
                F_LogInfo("VulkanWindow::getDeviceLayersAndExtensions: Using device layers count: %d", (int32)countLayers);
                for (int i = 0; i < countLayers; i++)
                {
                    F_LogInfo("VulkanWindow::getDeviceLayersAndExtensions: Using device layer index: %d, name: %s", i, outDeviceLayers[i]);
                }
            }
            else
            {
                F_LogInfo("VulkanWindow::getDeviceLayersAndExtensions: Not using device layers !");
            }

            size_t countExtensions = outDeviceExtensions.size();
            if (countExtensions > 0)
            {
                F_LogInfo("VulkanWindow::getDeviceLayersAndExtensions: Using device extensions count: %d", (int32)countExtensions);
                for (int i = 0; i < countExtensions; i++)
                {
                    F_LogInfo("VulkanWindow::getDeviceLayersAndExtensions: Using device extensions index: %d, name: %s", i, outDeviceExtensions[i]);
                }
            }
            else
            {
                F_LogInfo("VulkanWindow::getDeviceLayersAndExtensions: Not using device extensions !");
            }
        }
        F_LogInfo("VulkanWindow::getDeviceLayersAndExtensions ***********************");
    } 

}; //LostPeterVulkan