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
        RHIVulkanPhysicalDevice(RHIVulkanInstance* pVulkanInstance, VkPhysicalDevice vkPhysicalDevice, int32 nPhysicalDeviceIndex);
        virtual ~RHIVulkanPhysicalDevice();

    public:
    protected:
        RHIVulkanInstance* m_pVulkanInstance;
        VkPhysicalDevice m_vkPhysicalDevice;
        int32 m_nPhysicalDeviceIndex;
        VkPhysicalDeviceProperties m_vkPhysicalDeviceProperties;
        VkPhysicalDeviceMemoryProperties m_vkPhysicalDeviceMemProperties;
        VkPhysicalDeviceFeatures m_vkPhysicalDeviceFeatures;
        VkPhysicalDeviceFeatures2* m_pVkPhysicalDeviceFeatures2;
        VkQueueFamilyPropertiesVector m_aVkQueueFamilyProperties;

        String m_strPhysicalDeviceType;
        bool m_bIsDiscrete;
        VkSampleCountFlagBits m_vkMaxMSAASamples;

        RHIVulkanDevice* m_pVulkanDevice;

    public:
        F_FORCEINLINE RHIVulkanInstance* GetVulkanInstance() const { return m_pVulkanInstance; }
        F_FORCEINLINE VkPhysicalDevice& GetVkPhysicalDevice() { return m_vkPhysicalDevice; }
        F_FORCEINLINE const VkPhysicalDevice& GetVkPhysicalDevice() const { return m_vkPhysicalDevice; }
        F_FORCEINLINE int32 GetPhysicalDeviceIndex() const { return m_nPhysicalDeviceIndex; }
        F_FORCEINLINE VkPhysicalDeviceProperties* GetVkPhysicalDevicePropertiesPtr() { return &m_vkPhysicalDeviceProperties; }
        F_FORCEINLINE VkPhysicalDeviceProperties& GetVkPhysicalDeviceProperties() { return m_vkPhysicalDeviceProperties; }
        F_FORCEINLINE VkPhysicalDeviceFeatures& GetVkPhysicalDeviceFeatures() { return m_vkPhysicalDeviceFeatures; }
        F_FORCEINLINE const VkPhysicalDeviceFeatures& GetVkPhysicalDeviceFeatures() const { return m_vkPhysicalDeviceFeatures; }
        F_FORCEINLINE const VkPhysicalDeviceLimits& GetVkPhysicalDeviceLimits() const { return m_vkPhysicalDeviceProperties.limits; }
        F_FORCEINLINE VkPhysicalDeviceFeatures2* GetVkPhysicalDeviceFeatures2() const { return m_pVkPhysicalDeviceFeatures2; }
        F_FORCEINLINE void SetVkPhysicalDeviceFeatures2(VkPhysicalDeviceFeatures2* pPhysicalDeviceFeatures2) { m_pVkPhysicalDeviceFeatures2 = pPhysicalDeviceFeatures2; }
        F_FORCEINLINE const VkQueueFamilyPropertiesVector& GetQueueFamilyProperties() const { return m_aVkQueueFamilyProperties; }

        F_FORCEINLINE const String& GetPhysicalDeviceType() const { return m_strPhysicalDeviceType; }
        F_FORCEINLINE bool IsDiscrete() const { return m_bIsDiscrete; }
        F_FORCEINLINE VkSampleCountFlagBits GetMaxSampleCountFlagBits() const { return m_vkMaxMSAASamples; }

        F_FORCEINLINE RHIVulkanDevice* GetVulkanDevice() const { return m_pVulkanDevice; }

    public:
        virtual RHIPhysicalDeviceProperty GetPhysicalDeviceProperty();
        virtual RHIDevice* RequestDevice(const RHIDeviceCreateInfo& createInfo);

    public:
        uint32_t FindMemoryType(uint32_t filter, VkMemoryPropertyFlags propertyFlag) const;

    protected:
        void destroy();
        void init();

    public:
        static String GetPhysicalDeviceType(const VkPhysicalDeviceProperties& deviceProperties, bool& isDiscrete);
        static String GetQueueType(const VkQueueFamilyProperties& props);
        static VkSampleCountFlagBits GetMaxUsableSampleCount(const VkPhysicalDeviceProperties& deviceProperties);
    };
    
}; //LostPeterPluginRHIVulkan

#endif