/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIVulkanConverter.h"

namespace LostPeterPluginRHIVulkan
{
    ////////////////////// TransformFromXXXX //////////////////////
    RHIPhysicalDeviceType RHIVulkanConverter::TransformFromVkPhysicalDeviceType(VkPhysicalDeviceType ePhysicalDevice)
    {
        switch ((int32)ePhysicalDevice)
        {
        case VK_PHYSICAL_DEVICE_TYPE_OTHER:                 return RHIPhysicalDeviceType::RHI_PhysicalDevice_UnKnown;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:        return RHIPhysicalDeviceType::RHI_PhysicalDevice_GPUIntegrated;
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:          return RHIPhysicalDeviceType::RHI_PhysicalDevice_GPUDiscrete;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:           return RHIPhysicalDeviceType::RHI_PhysicalDevice_GPUVirtual;
        case VK_PHYSICAL_DEVICE_TYPE_CPU:                   return RHIPhysicalDeviceType::RHI_PhysicalDevice_CPU;
        }
        F_Assert(false && "RHIVulkanConverter::TransformFromVkPhysicalDeviceType: Wrong VkPhysicalDeviceType type !")
        return RHIPhysicalDeviceType::RHI_PhysicalDevice_UnKnown; 
    }




    ////////////////////// TransformToXXXX ////////////////////////
    VkPhysicalDeviceType RHIVulkanConverter::TransformToVkPhysicalDeviceType(RHIPhysicalDeviceType ePhysicalDevice)
    {
        switch (ePhysicalDevice)
        {
        case RHIPhysicalDeviceType::RHI_PhysicalDevice_UnKnown:         return VK_PHYSICAL_DEVICE_TYPE_OTHER;
        case RHIPhysicalDeviceType::RHI_PhysicalDevice_GPUIntegrated:   return VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;
        case RHIPhysicalDeviceType::RHI_PhysicalDevice_GPUDiscrete:     return VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
        case RHIPhysicalDeviceType::RHI_PhysicalDevice_GPUVirtual:      return VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU;
        case RHIPhysicalDeviceType::RHI_PhysicalDevice_CPU:             return VK_PHYSICAL_DEVICE_TYPE_CPU;
        case RHIPhysicalDeviceType::RHI_PhysicalDevice_Count:           return VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM;
        }
        F_Assert(false && "RHIVulkanConverter::TransformToVkPhysicalDeviceType: Wrong RHIPhysicalDeviceType type !")
        return VK_PHYSICAL_DEVICE_TYPE_OTHER; 
    }




}; //LostPeterPluginRHIVulkan