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

#include "../include/RHIVulkanInstance.h"
#include "../include/RHIVulkanPhysicalDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanInstance::RHIVulkanInstance()
        : m_pPhysicalDevice(nullptr)
    {

    }

    RHIVulkanInstance::~RHIVulkanInstance()
    {
        Destroy();
    }

    void RHIVulkanInstance::Destroy()
    {
        F_DELETE(m_pPhysicalDevice)
    }

    RHIType RHIVulkanInstance::GetRHIType()
    {
        return RHIType::RHI_Vulkan;
    }

    uint32_t RHIVulkanInstance::GetPhysicalDeviceCount()
    {
        return 1;
    }

    RHIPhysicalDevice* RHIVulkanInstance::GetPhysicalDevice(uint32 nIndex)
    {
        F_Assert("RHIVulkanInstance::GetPhysicalDevice" && nIndex == 0);
        if (m_pPhysicalDevice == nullptr)
        {
            m_pPhysicalDevice = new RHIVulkanPhysicalDevice();
        }
        return m_pPhysicalDevice;
    }

}; //LostPeterPluginRHIVulkan

