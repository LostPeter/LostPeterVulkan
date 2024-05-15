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

#ifndef _RHI_VULKAN_PHYSICAL_DEVICE_H_
#define _RHI_VULKAN_PHYSICAL_DEVICE_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanPhysicalDevice : public RHIPhysicalDevice
    {
    public:
        RHIVulkanPhysicalDevice(RHIVulkanInstance* pInstance, VkPhysicalDevice vkPhysicalDevice);
        virtual ~RHIVulkanPhysicalDevice();

    public:
    protected:
        RHIVulkanInstance* m_pInstance;
        VkPhysicalDevice m_vkPhysicalDevice;
        VkPhysicalDeviceProperties m_vkPhysicalDeviceProperties;
        VkPhysicalDeviceFeatures m_vkPhysicalDeviceFeatures;
        VkPhysicalDeviceFeatures2* m_pVkPhysicalDeviceFeatures2;
        VkQueueFamilyPropertiesVector m_aVkQueueFamilyProperties;

    public:
        F_FORCEINLINE VkPhysicalDevice& GetVkPhysicalDevice() { return m_vkPhysicalDevice; }
        F_FORCEINLINE const VkPhysicalDevice& GetVkPhysicalDevice() const { return m_vkPhysicalDevice; }
        F_FORCEINLINE VkPhysicalDeviceProperties* GetVkPhysicalDevicePropertiesPtr() { return &m_vkPhysicalDeviceProperties; }
        F_FORCEINLINE VkPhysicalDeviceProperties& GetVkPhysicalDeviceProperties() { return m_vkPhysicalDeviceProperties; }
        F_FORCEINLINE VkPhysicalDeviceFeatures& GetVkPhysicalDeviceFeatures() { return m_vkPhysicalDeviceFeatures; }
        F_FORCEINLINE const VkPhysicalDeviceFeatures& GetVkPhysicalDeviceFeatures() const { return m_vkPhysicalDeviceFeatures; }
        F_FORCEINLINE const VkPhysicalDeviceLimits& GetVkPhysicalDeviceLimits() const { return m_vkPhysicalDeviceProperties.limits; }
        F_FORCEINLINE VkPhysicalDeviceFeatures2* GetVkPhysicalDeviceFeatures2() const { return m_pVkPhysicalDeviceFeatures2; }
        F_FORCEINLINE void SetVkPhysicalDeviceFeatures2(VkPhysicalDeviceFeatures2* pPhysicalDeviceFeatures2) { m_pVkPhysicalDeviceFeatures2 = pPhysicalDeviceFeatures2; }

    public:
        virtual RHIPhysicalDeviceProperty GetPhysicalDeviceProperty();
        virtual RHIDevice* RequestDevice(const RHIDeviceCreateInfo& createInfo);

    public:
        static String GetDeviceType(const VkPhysicalDeviceProperties& deviceProperties, bool& isDiscrete);
        static String GetQueueType(const VkQueueFamilyProperties& props);
    };
    
}; //LostPeterPluginRHIVulkan

#endif