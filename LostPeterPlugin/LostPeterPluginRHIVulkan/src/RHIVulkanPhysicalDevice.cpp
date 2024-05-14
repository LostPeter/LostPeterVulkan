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
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanPhysicalDevice::RHIVulkanPhysicalDevice()
    {

    }

    RHIVulkanPhysicalDevice::~RHIVulkanPhysicalDevice()
    {

    }

    RHIPhysicalDeviceProperty RHIVulkanPhysicalDevice::GetPhysicalDeviceProperty()
    {
        return { };
    }

    RHIDevice* RHIVulkanPhysicalDevice::RequestDevice(const RHIDeviceCreateInfo& createInfo)
    {
        return new RHIVulkanDevice(createInfo);
    }
    
}; //LostPeterPluginRHIVulkan