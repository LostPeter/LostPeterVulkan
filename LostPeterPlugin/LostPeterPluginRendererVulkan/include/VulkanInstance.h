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

namespace LostPeterPluginRendererVulkan
{
    class VulkanInstance
    {
    public:
        VulkanInstance();
        ~VulkanInstance();

    public:
    protected:
        VkInstance m_vkInstance;
        ConstCharPtrVector m_aInstanceLayers;
        ConstCharPtrVector m_aInstanceExtensions;
        ConstCharPtrVector m_aAppInstanceExtensions;
        ConstCharPtrVector m_aAppDeviceExtensions;
        int32 m_nPreferredVendorID;
        VkPhysicalDeviceFeatures2* m_pVkPhysicalDeviceFeatures2;

        VulkanVolk* m_pVolk;
        bool m_bIsEnableValidationLayers;
        VulkanDebug* m_pDebug;
        VulkanDevice* m_pDevice;

    public:
        F_FORCEINLINE const VkInstance& GetVkInstance() const { return m_vkInstance; }
        F_FORCEINLINE const ConstCharPtrVector& GetInstanceLayers() const { return m_aInstanceLayers; }
        F_FORCEINLINE const ConstCharPtrVector& GetInstanceExtensions() const { return m_aInstanceExtensions; }
        F_FORCEINLINE const ConstCharPtrVector& GetAppInstanceExtensions() const { return m_aAppInstanceExtensions; }
        F_FORCEINLINE const ConstCharPtrVector& GetAppDeviceExtensions() const { return m_aAppDeviceExtensions; }
        F_FORCEINLINE void AddAppInstanceExtensions(const char* szName) { m_aAppInstanceExtensions.push_back(szName); }
        F_FORCEINLINE void AddAppDeviceExtensions(const char* szName) { m_aAppDeviceExtensions.push_back(szName); }
        F_FORCEINLINE int32 GetPreferredVendorID() const { return m_nPreferredVendorID; }
        F_FORCEINLINE void SetPreferredVendorID(int32 vendorID) { m_nPreferredVendorID = vendorID; }
        F_FORCEINLINE VkPhysicalDeviceFeatures2* GetVkPhysicalDeviceFeatures2() const { return m_pVkPhysicalDeviceFeatures2; }
        F_FORCEINLINE void SetVkPhysicalDeviceFeatures2(VkPhysicalDeviceFeatures2* p) { m_pVkPhysicalDeviceFeatures2 = p; }
        
        F_FORCEINLINE VulkanVolk* GetVolk() const { return m_pVolk; }
        F_FORCEINLINE bool IsEnableValidationLayers() const { return m_bIsEnableValidationLayers; }
        F_FORCEINLINE VulkanDebug* GetDebug() const { return m_pDebug; }
        F_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }

    public:
        void Destroy();
        bool Init();

    public:
    protected:
        void destroyDebug();
        void destroyVolk();

        bool createInstance();
        bool createVolk();
        bool createDebug();
        bool createDevice();

    private:
        void getInstanceLayersAndExtensions(bool bIsEnableValidationLayers,
                                            ConstCharPtrVector& outInstanceLayers, 
                                            ConstCharPtrVector& outInstanceExtensions);

    /////////////////////////////////////// Vulkan Function Wrapper ///////////////////////////////////////
    public:
        //////////////////// VkInstance /////////////////////
        void DestroyVkInstance(const VkInstance& vkInstance);

        //////////////////// VkSurfaceKHR ///////////////////
        bool CreateVkSurfaceKHR(GLFWwindow* pWindow,
                                VkSurfaceKHR& vkSurfaceKHR);
        void DestroyVkSurfaceKHR(const VkSurfaceKHR& vkSurfaceKHR);

    /////////////////////////////////////// Vulkan Function Wrapper ///////////////////////////////////////
    };

}; //LostPeterPluginRendererVulkan

#endif