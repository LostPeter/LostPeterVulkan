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

    public:
        F_FORCEINLINE VkPhysicalDevice& GetVkPhysicalDevice() { return m_vkPhysicalDevice; }
        F_FORCEINLINE const VkPhysicalDevice& GetVkPhysicalDevice() const { return m_vkPhysicalDevice; }

    public:
        virtual RHIPhysicalDeviceProperty GetPhysicalDeviceProperty();
        virtual RHIDevice* RequestDevice(const RHIDeviceCreateInfo& createInfo);
    };
    
}; //LostPeterPluginRHIVulkan

#endif