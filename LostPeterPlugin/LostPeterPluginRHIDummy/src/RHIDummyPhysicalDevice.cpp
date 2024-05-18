/****************************************************************************
* LostPeterPluginRHIDummy - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIDummyPhysicalDevice.h"
#include "../include/RHIDummyInstance.h"
#include "../include/RHIDummyDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyPhysicalDevice::RHIDummyPhysicalDevice(RHIDummyInstance* pDummyInstance)
        : RHIPhysicalDevice(pDummyInstance)
        , m_pDummyInstance(pDummyInstance)
    {
        F_Assert(m_pDummyInstance && "RHIDummyPhysicalDevice::RHIDummyPhysicalDevice")
    }

    RHIDummyPhysicalDevice::~RHIDummyPhysicalDevice()
    {
        
    }
    
    RHIPhysicalDeviceProperty RHIDummyPhysicalDevice::GetPhysicalDeviceProperty()
    {
        return { };
    }   

    RHIDevice* RHIDummyPhysicalDevice::RequestDevice(const RHIDeviceCreateInfo& createInfo)
    {
        return new RHIDummyDevice(createInfo);
    }

}; //LostPeterPluginRHIDummy