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

#ifndef _VULKAN_INSTANCE_H_
#define _VULKAN_INSTANCE_H_

#include "VulkanPreDefine.h"

namespace LostPeter
{
    class lpVulkanExport VulkanInstance
    {
    public:
        VulkanInstance();
        ~VulkanInstance();

    public:
    protected:
        VkInstance m_vkInstance;
        VkDebugReportCallbackEXT m_vkDebugReport;
        ConstCharPtrVector m_aInstanceLayers;
        ConstCharPtrVector m_aInstanceExtensions;
        ConstCharPtrVector m_aAppInstanceExtensions;
        ConstCharPtrVector m_aAppDeviceExtensions;
        int32 m_nPreferredVendorID;
        VkPhysicalDeviceFeatures2* m_pVkPhysicalDeviceFeatures2;
        uint32 m_nDesiredNumSwapChainImages;

        bool m_bIsEnableValidationLayers;
        VulkanDevice* m_pDevice;

        VulkanPixelFormatType m_eSwapChainImagePixelFormat;

    public:
        UTIL_FORCEINLINE const VkInstance& GetVkInstance() const { return m_vkInstance; }
        UTIL_FORCEINLINE const VkDebugReportCallbackEXT& GetVkDebugReportCallbackEXT() const { return m_vkDebugReport; }
        UTIL_FORCEINLINE const ConstCharPtrVector& GetInstanceLayers() const { return m_aInstanceLayers; }
        UTIL_FORCEINLINE const ConstCharPtrVector& GetInstanceExtensions() const { return m_aInstanceExtensions; }
        UTIL_FORCEINLINE const ConstCharPtrVector& GetAppInstanceExtensions() const { return m_aAppInstanceExtensions; }
        UTIL_FORCEINLINE const ConstCharPtrVector& GetAppDeviceExtensions() const { return m_aAppDeviceExtensions; }
        UTIL_FORCEINLINE void AddAppInstanceExtensions(const char* szName) { m_aAppInstanceExtensions.push_back(szName); }
        UTIL_FORCEINLINE void AddAppDeviceExtensions(const char* szName) { m_aAppDeviceExtensions.push_back(szName); }
        UTIL_FORCEINLINE int32 GetPreferredVendorID() const { return m_nPreferredVendorID; }
        UTIL_FORCEINLINE void SetPreferredVendorID(int32 vendorID) { m_nPreferredVendorID = vendorID; }
        UTIL_FORCEINLINE VkPhysicalDeviceFeatures2* GetVkPhysicalDeviceFeatures2() const { return m_pVkPhysicalDeviceFeatures2; }
        UTIL_FORCEINLINE void SetVkPhysicalDeviceFeatures2(VkPhysicalDeviceFeatures2* p) { m_pVkPhysicalDeviceFeatures2 = p; }
        UTIL_FORCEINLINE uint32 GetDesiredNumSwapChainImages() const { return m_nDesiredNumSwapChainImages; }
        UTIL_FORCEINLINE void SetDesiredNumSwapChainImages(uint32 num) { m_nDesiredNumSwapChainImages = num; }
        
        UTIL_FORCEINLINE bool IsEnableValidationLayers() const { return m_bIsEnableValidationLayers; }
        UTIL_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }

        UTIL_FORCEINLINE VulkanPixelFormatType GetVkSwapChainImagePixelFormatType() const { return m_eSwapChainImagePixelFormat; }

    public:
        void Destroy();
        bool Init();

    public:
    protected:
        bool createInstance();
        bool createDebugReport();
        bool createDevice();

    private:
        void getInstanceLayersAndExtensions(bool bIsEnableValidationLayers,
                                            ConstCharPtrVector& outInstanceLayers, 
                                            ConstCharPtrVector& outInstanceExtensions);

        VkDebugReportCallbackCreateInfoEXT createReportCallbackInfo();
        void destroyReportCallbackInfo();
    };

}; //LostPeter

#endif