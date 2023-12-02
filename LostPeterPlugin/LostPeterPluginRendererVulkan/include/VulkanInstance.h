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
    class VulkanInstance
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

        FPixelFormatType m_eSwapChainImagePixelFormat;

    public:
        E_FORCEINLINE const VkInstance& GetVkInstance() const { return m_vkInstance; }
        E_FORCEINLINE const VkDebugReportCallbackEXT& GetVkDebugReportCallbackEXT() const { return m_vkDebugReport; }
        E_FORCEINLINE const ConstCharPtrVector& GetInstanceLayers() const { return m_aInstanceLayers; }
        E_FORCEINLINE const ConstCharPtrVector& GetInstanceExtensions() const { return m_aInstanceExtensions; }
        E_FORCEINLINE const ConstCharPtrVector& GetAppInstanceExtensions() const { return m_aAppInstanceExtensions; }
        E_FORCEINLINE const ConstCharPtrVector& GetAppDeviceExtensions() const { return m_aAppDeviceExtensions; }
        E_FORCEINLINE void AddAppInstanceExtensions(const char* szName) { m_aAppInstanceExtensions.push_back(szName); }
        E_FORCEINLINE void AddAppDeviceExtensions(const char* szName) { m_aAppDeviceExtensions.push_back(szName); }
        E_FORCEINLINE int32 GetPreferredVendorID() const { return m_nPreferredVendorID; }
        E_FORCEINLINE void SetPreferredVendorID(int32 vendorID) { m_nPreferredVendorID = vendorID; }
        E_FORCEINLINE VkPhysicalDeviceFeatures2* GetVkPhysicalDeviceFeatures2() const { return m_pVkPhysicalDeviceFeatures2; }
        E_FORCEINLINE void SetVkPhysicalDeviceFeatures2(VkPhysicalDeviceFeatures2* p) { m_pVkPhysicalDeviceFeatures2 = p; }
        E_FORCEINLINE uint32 GetDesiredNumSwapChainImages() const { return m_nDesiredNumSwapChainImages; }
        E_FORCEINLINE void SetDesiredNumSwapChainImages(uint32 num) { m_nDesiredNumSwapChainImages = num; }
        
        E_FORCEINLINE bool IsEnableValidationLayers() const { return m_bIsEnableValidationLayers; }
        E_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }

        E_FORCEINLINE FPixelFormatType GetVkSwapChainImagePixelFormatType() const { return m_eSwapChainImagePixelFormat; }

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