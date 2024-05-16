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

#ifndef _RHI_VULKAN_INSTANCE_H_
#define _RHI_VULKAN_INSTANCE_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanInstance : public RHIInstance
    {
    public:
        RHIVulkanInstance();
        virtual ~RHIVulkanInstance();

    public:
    protected:
        VkInstance m_vkInstance;
        ConstCharPtrVector m_aInstanceLayers;
        ConstCharPtrVector m_aInstanceExtensions;
        ConstCharPtrVector m_aAppInstanceExtensions;
        ConstCharPtrVector m_aAppDeviceExtensions;
        int32 m_nPreferredVendorID;

        RHIVulkanVolk* m_pVolk;
        bool m_bIsEnableValidationLayers;
        RHIVulkanDebug* m_pDebug;

        RHIVulkanPhysicalDevicePtrVector m_aPhysicalDevices;
        RHIVulkanPhysicalDevice* m_pPhysicalDevice;
        RHIVulkanDevice* m_pDevice;

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

        F_FORCEINLINE RHIVulkanVolk* GetVolk() const { return m_pVolk; }
        F_FORCEINLINE bool IsEnableValidationLayers() const { return m_bIsEnableValidationLayers; }
        F_FORCEINLINE RHIVulkanDebug* GetDebug() const { return m_pDebug; }

        F_FORCEINLINE const RHIVulkanPhysicalDevicePtrVector& GetPhysicalDevices() const { return m_aPhysicalDevices; }
        F_FORCEINLINE RHIVulkanPhysicalDevice* GetPhysicalDevice() const { return m_pPhysicalDevice; }
        F_FORCEINLINE RHIVulkanDevice* GetDevice() const { return m_pDevice; }

    public:
        virtual void Destroy();
        bool Init();

        virtual RHIType GetRHIType();
        virtual uint32 GetPhysicalDeviceCount();
        virtual RHIPhysicalDevice* GetPhysicalDevice(uint32 nIndex);
        virtual RHIDevice* RequestDevice(const RHIDeviceCreateInfo& createInfo);

    public:


    protected:
        void destroyDebug();
        void destroyVolk();
        void destroyPhysicalDevice();

        bool createInstance();
        bool createVolk();
        bool createDebug();
        bool createPhysicalDevice();

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

}; //LostPeterPluginRHIVulkan

#endif

