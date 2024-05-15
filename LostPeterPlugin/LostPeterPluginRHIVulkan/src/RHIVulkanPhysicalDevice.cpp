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

#include "../include/RHIVulkanPhysicalDevice.h"
#include "../include/RHIVulkanInstance.h"
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanPhysicalDevice::RHIVulkanPhysicalDevice(RHIVulkanInstance* pInstance, VkPhysicalDevice vkPhysicalDevice)
        : m_pInstance(pInstance)
        , m_vkPhysicalDevice(vkPhysicalDevice)
    {
        F_Assert(m_pInstance && m_vkPhysicalDevice && "RHIVulkanPhysicalDevice::RHIVulkanPhysicalDevice")
    }

    RHIVulkanPhysicalDevice::~RHIVulkanPhysicalDevice()
    {

    }

    RHIPhysicalDeviceProperty RHIVulkanPhysicalDevice::GetPhysicalDeviceProperty()
    {
        VkPhysicalDeviceProperties vkPhysicalDeviceProperties;
        vkGetPhysicalDeviceProperties(m_vkPhysicalDevice, &vkPhysicalDeviceProperties);

        RHIPhysicalDeviceProperty property = {};
        property.nVendorID = vkPhysicalDeviceProperties.vendorID;
        property.nDeviceID = vkPhysicalDeviceProperties.deviceID;
        //property.ePhysicalDevice = ;
        return property;
    }

    RHIDevice* RHIVulkanPhysicalDevice::RequestDevice(const RHIDeviceCreateInfo& createInfo)
    {
        return new RHIVulkanDevice(this, createInfo);
    }
    
}; //LostPeterPluginRHIVulkan