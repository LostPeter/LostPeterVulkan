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
    RHIVulkanPhysicalDevice::RHIVulkanPhysicalDevice(RHIVulkanInstance* pVulkanInstance, VkPhysicalDevice vkPhysicalDevice, int32 nPhysicalDeviceIndex)
        : RHIPhysicalDevice(pVulkanInstance)
        , m_pVulkanInstance(pVulkanInstance)
        , m_vkPhysicalDevice(vkPhysicalDevice)
        , m_nPhysicalDeviceIndex(nPhysicalDeviceIndex)
        , m_pVkPhysicalDeviceFeatures2(nullptr)
        , m_strPhysicalDeviceType("")
        , m_bIsDiscrete(false)
        , m_vkMaxMSAASamples()
        , m_pVulkanDevice(nullptr)
    {
        F_Assert(m_pVulkanInstance && m_vkPhysicalDevice && "RHIVulkanPhysicalDevice::RHIVulkanPhysicalDevice")
        init();
    }

    RHIVulkanPhysicalDevice::~RHIVulkanPhysicalDevice()
    {
        destroy();
    }

    void RHIVulkanPhysicalDevice::destroy()
    {
        F_DELETE(m_pVulkanDevice)
    }
    void RHIVulkanPhysicalDevice::init()
    {
        vkGetPhysicalDeviceProperties(m_vkPhysicalDevice, &m_vkPhysicalDeviceProperties);
        vkGetPhysicalDeviceFeatures(m_vkPhysicalDevice, &m_vkPhysicalDeviceFeatures);
        vkGetPhysicalDeviceMemoryProperties(m_vkPhysicalDevice, &m_vkPhysicalDeviceMemProperties);

        m_strPhysicalDeviceType = GetPhysicalDeviceType(m_vkPhysicalDeviceProperties, m_bIsDiscrete);
        m_vkMaxMSAASamples = GetMaxUsableSampleCount(m_vkPhysicalDeviceProperties);

        F_LogInfo("RHIVulkanPhysicalDevice::init: Device index: [%d], name: [%s]", m_nPhysicalDeviceIndex, m_vkPhysicalDeviceProperties.deviceName);
        F_LogInfo("RHIVulkanPhysicalDevice::init: API: [%d.%d.%d(0x%x)], Driver: [0x%x VendorId 0x%x]", VK_VERSION_MAJOR(m_vkPhysicalDeviceProperties.apiVersion), VK_VERSION_MINOR(m_vkPhysicalDeviceProperties.apiVersion), VK_VERSION_PATCH(m_vkPhysicalDeviceProperties.apiVersion), m_vkPhysicalDeviceProperties.apiVersion, m_vkPhysicalDeviceProperties.driverVersion, m_vkPhysicalDeviceProperties.vendorID);
        F_LogInfo("RHIVulkanPhysicalDevice::init: DeviceID: [0x%x], Type: [%s]", m_vkPhysicalDeviceProperties.deviceID, m_strPhysicalDeviceType.c_str());
        F_LogInfo("RHIVulkanPhysicalDevice::init: Max Descriptor Sets Bound: [%d], Timestamps: [%d]", m_vkPhysicalDeviceProperties.limits.maxBoundDescriptorSets, m_vkPhysicalDeviceProperties.limits.timestampComputeAndGraphics);
        F_LogInfo("RHIVulkanPhysicalDevice::init: Max MSAA Samples: [%d]", (int32)m_vkMaxMSAASamples);

        uint32 queueCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(m_vkPhysicalDevice, &queueCount, nullptr);
        m_aVkQueueFamilyProperties.resize(queueCount);
        vkGetPhysicalDeviceQueueFamilyProperties(m_vkPhysicalDevice, &queueCount, m_aVkQueueFamilyProperties.data());
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
        m_pVulkanDevice = new RHIVulkanDevice(this, createInfo);
        return m_pVulkanDevice;
    }

    uint32_t RHIVulkanPhysicalDevice::FindMemoryType(uint32_t filter, VkMemoryPropertyFlags propertyFlag) const
    {
        for (uint32_t i = 0; i < m_vkPhysicalDeviceMemProperties.memoryTypeCount; i++) 
        {
            if ((filter & (1 << i)) && (m_vkPhysicalDeviceMemProperties.memoryTypes[i].propertyFlags & propertyFlag) == propertyFlag) 
            {
                return i;
            }
        }
        F_Assert(false && "RHIVulkanPhysicalDevice::FindMemoryType: Failed to found suitable memory type !");
        return -1;
    }

    String RHIVulkanPhysicalDevice::GetPhysicalDeviceType(const VkPhysicalDeviceProperties& deviceProperties, bool& isDiscrete)
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
    VkSampleCountFlagBits RHIVulkanPhysicalDevice::GetMaxUsableSampleCount(const VkPhysicalDeviceProperties& deviceProperties)
    {
        VkSampleCountFlags counts = deviceProperties.limits.framebufferColorSampleCounts & deviceProperties.limits.framebufferDepthSampleCounts;
        if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
        if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
        if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
        if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
        if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
        if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

        return VK_SAMPLE_COUNT_1_BIT;
    }

}; //LostPeterPluginRHIVulkan