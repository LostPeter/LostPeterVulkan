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

#include "../include/RHIDummyInstance.h"
#include "../include/RHIDummyPhysicalDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyInstance::RHIDummyInstance()
        : m_pDummyPhysicalDevice(nullptr)
    {
        
    }

    RHIDummyInstance::~RHIDummyInstance()
    {
        Destroy();
    }

    void RHIDummyInstance::Destroy()
    {
        F_DELETE(m_pDummyPhysicalDevice)
    }

    RHIType RHIDummyInstance::GetRHIType()
    {
        return RHIType::RHI_Dummy;
    }

    uint32_t RHIDummyInstance::GetPhysicalDeviceCount()
    {
        return 1;
    }

    RHIPhysicalDevice* RHIDummyInstance::GetPhysicalDevice(uint32 nIndex)
    {
        F_Assert("RHIDummyInstance::GetPhysicalDevice" && nIndex == 0);
        if (m_pDummyPhysicalDevice == nullptr)
        {
            m_pDummyPhysicalDevice = new RHIDummyPhysicalDevice(this);
            m_pPhysicalDevice = m_pDummyPhysicalDevice;
        }
        return m_pDummyPhysicalDevice;
    }

    RHIDevice* RHIDummyInstance::RequestDevice(const RHIDeviceCreateInfo& createInfo)
    {
        return GetPhysicalDevice(0)->RequestDevice(createInfo);
    }

}; //LostPeterPluginRHIDummy

