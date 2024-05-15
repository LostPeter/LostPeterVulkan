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
#include "../include/RHIVulkanConverter.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanPhysicalDevice::RHIVulkanPhysicalDevice(RHIVulkanInstance* pInstance, VkPhysicalDevice vkPhysicalDevice)
        : m_pInstance(pInstance)
        , m_vkPhysicalDevice(vkPhysicalDevice)
        , m_pVkPhysicalDeviceFeatures2(nullptr)
    {
        F_Assert(m_pInstance && m_vkPhysicalDevice && "RHIVulkanPhysicalDevice::RHIVulkanPhysicalDevice")

        vkGetPhysicalDeviceProperties(m_vkPhysicalDevice, &m_vkPhysicalDeviceProperties);
    }

    RHIVulkanPhysicalDevice::~RHIVulkanPhysicalDevice()
    {

    }

    RHIPhysicalDeviceProperty RHIVulkanPhysicalDevice::GetPhysicalDeviceProperty()
    {
        RHIPhysicalDeviceProperty property = {};
        property.nVendorID = m_vkPhysicalDeviceProperties.vendorID;
        property.nDeviceID = m_vkPhysicalDeviceProperties.deviceID;
        property.ePhysicalDevice = RHIVulkanConverter::TransformFromVkPhysicalDeviceType(m_vkPhysicalDeviceProperties.deviceType);
        return property;
    }

    RHIDevice* RHIVulkanPhysicalDevice::RequestDevice(const RHIDeviceCreateInfo& createInfo)
    {
        return new RHIVulkanDevice(this, createInfo);
    }

    String RHIVulkanPhysicalDevice::GetDeviceType(const VkPhysicalDeviceProperties& deviceProperties, bool& isDiscrete)
    {
        isDiscrete = false;
        String info;
		switch (deviceProperties.deviceType)
		{
		case  VK_PHYSICAL_DEVICE_TYPE_OTHER:
			info = ("Other");
			break;
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
			info = ("Integrated GPU");
			break;
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			info = ("Discrete GPU");
			isDiscrete = true;
			break;
		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
			info = ("Virtual GPU");
			break;
		case VK_PHYSICAL_DEVICE_TYPE_CPU:
			info = ("CPU");
			break;
		default:
			info = ("Unknown");
			break;
		}
		return info;
    }
    String RHIVulkanPhysicalDevice::GetQueueType(const VkQueueFamilyProperties& props)
    {
        String info;
        if ((props.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT) 
        {
            info += " Graphics";
        }
        if ((props.queueFlags & VK_QUEUE_COMPUTE_BIT) == VK_QUEUE_COMPUTE_BIT) 
        {
            info += " Compute";
        }
        if ((props.queueFlags & VK_QUEUE_TRANSFER_BIT) == VK_QUEUE_TRANSFER_BIT) 
        {
            info += " Transfer";
        }
        if ((props.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) == VK_QUEUE_SPARSE_BINDING_BIT) 
        {
            info += " Sparse";
        }
        return info;
    }
    
}; //LostPeterPluginRHIVulkan