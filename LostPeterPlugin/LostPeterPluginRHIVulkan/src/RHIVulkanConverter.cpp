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


    RHIPixelFormatType RHIVulkanConverter::TransformFromVkFormat(VkFormat eFormat)
    {
        switch ((int32)eFormat)
        {
        //8-Bits
        case VK_FORMAT_R8_UNORM:                    return RHIPixelFormatType::RHI_PixelFormat_R8UNorm;
        case VK_FORMAT_R8_SNORM:                    return RHIPixelFormatType::RHI_PixelFormat_R8SNorm;
        case VK_FORMAT_R8_UINT:                     return RHIPixelFormatType::RHI_PixelFormat_R8UInt;
        case VK_FORMAT_R8_SINT:                     return RHIPixelFormatType::RHI_PixelFormat_R8SInt;
        //16-Bits
        case VK_FORMAT_R16_UINT:                    return RHIPixelFormatType::RHI_PixelFormat_R16UInt;
        case VK_FORMAT_R16_SINT:                    return RHIPixelFormatType::RHI_PixelFormat_R16SInt;
        case VK_FORMAT_R16_SFLOAT:                  return RHIPixelFormatType::RHI_PixelFormat_R16Float;
        case VK_FORMAT_R8G8_UNORM:                  return RHIPixelFormatType::RHI_PixelFormat_RG8UNorm;
        case VK_FORMAT_R8G8_SNORM:                  return RHIPixelFormatType::RHI_PixelFormat_RG8SNorm;
        case VK_FORMAT_R8G8_UINT:                   return RHIPixelFormatType::RHI_PixelFormat_RG8UInt;
        case VK_FORMAT_R8G8_SINT:                   return RHIPixelFormatType::RHI_PixelFormat_RG8SInt;
        //32-Bits
        case VK_FORMAT_R32_UINT:                    return RHIPixelFormatType::RHI_PixelFormat_R32UInt;
        case VK_FORMAT_R32_SINT:                    return RHIPixelFormatType::RHI_PixelFormat_R32SInt;
        case VK_FORMAT_R32_SFLOAT:                  return RHIPixelFormatType::RHI_PixelFormat_R32Float;
        case VK_FORMAT_R16G16_UINT:                 return RHIPixelFormatType::RHI_PixelFormat_RG16UInt;
        case VK_FORMAT_R16G16_SINT:                 return RHIPixelFormatType::RHI_PixelFormat_RG16SInt;
        case VK_FORMAT_R16G16_SFLOAT:               return RHIPixelFormatType::RHI_PixelFormat_RG16Float;
        case VK_FORMAT_R8G8B8A8_UNORM:              return RHIPixelFormatType::RHI_PixelFormat_RGBA8UNorm;
        case VK_FORMAT_R8G8B8A8_SRGB:               return RHIPixelFormatType::RHI_PixelFormat_RGBA8UNormSRGB;
        case VK_FORMAT_R8G8B8A8_SNORM:              return RHIPixelFormatType::RHI_PixelFormat_RGBA8SNorm;
        case VK_FORMAT_R8G8B8A8_UINT:               return RHIPixelFormatType::RHI_PixelFormat_RGBA8UInt;
        case VK_FORMAT_R8G8B8A8_SINT:               return RHIPixelFormatType::RHI_PixelFormat_RGBA8SInt;
        case VK_FORMAT_B8G8R8A8_UNORM:              return RHIPixelFormatType::RHI_PixelFormat_BGRA8UNorm;
        case VK_FORMAT_B8G8R8A8_SRGB:               return RHIPixelFormatType::RHI_PixelFormat_BGRA8UNormSRGB;
        case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:      return RHIPixelFormatType::RHI_PixelFormat_RGB9E5Float;
        case VK_FORMAT_A2R10G10B10_UNORM_PACK32:    return RHIPixelFormatType::RHI_PixelFormat_RGB10A2UNorm;
        case VK_FORMAT_B10G11R11_UFLOAT_PACK32:     return RHIPixelFormatType::RHI_PixelFormat_RG11B10Float;
        //64-Bits
        case VK_FORMAT_R32G32_UINT:                 return RHIPixelFormatType::RHI_PixelFormat_RG32UInt;
        case VK_FORMAT_R32G32_SINT:                 return RHIPixelFormatType::RHI_PixelFormat_RG32SInt;
        case VK_FORMAT_R32G32_SFLOAT:               return RHIPixelFormatType::RHI_PixelFormat_RG32Float;
        case VK_FORMAT_R16G16B16A16_UINT:           return RHIPixelFormatType::RHI_PixelFormat_RGBA16UInt;
        case VK_FORMAT_R16G16B16A16_SINT:           return RHIPixelFormatType::RHI_PixelFormat_RGBA16SInt;
        case VK_FORMAT_R16G16B16A16_SFLOAT:         return RHIPixelFormatType::RHI_PixelFormat_RGBA16Float;
        //128-Bits
        case VK_FORMAT_R32G32B32A32_UINT:           return RHIPixelFormatType::RHI_PixelFormat_RGBA32UInt;
        case VK_FORMAT_R32G32B32A32_SINT:           return RHIPixelFormatType::RHI_PixelFormat_RGBA32SInt;
        case VK_FORMAT_R32G32B32A32_SFLOAT:         return RHIPixelFormatType::RHI_PixelFormat_RGBA32Float;
        //Depth-Stencil
        case VK_FORMAT_D16_UNORM:                   return RHIPixelFormatType::RHI_PixelFormat_D16UNorm;
        case VK_FORMAT_D24_UNORM_S8_UINT:           return RHIPixelFormatType::RHI_PixelFormat_D24UNormS8UInt;
        case VK_FORMAT_D32_SFLOAT:                  return RHIPixelFormatType::RHI_PixelFormat_D32Float;
        case VK_FORMAT_D32_SFLOAT_S8_UINT:          return RHIPixelFormatType::RHI_PixelFormat_D32FloatS8UInt;
        //Features /BC/ETC/ASTC

        }
        F_Assert(false && "RHIVulkanConverter::TransformFromVkFormat: Wrong VkFormat type !")
        return RHIPixelFormatType::RHI_PixelFormat_Unknown;
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

    VkFormat RHIVulkanConverter::TransformToVkFormat(RHIPixelFormatType ePixelFormat)
    {
        switch (ePixelFormat)
        {
        //8-Bits
        case RHIPixelFormatType::RHI_PixelFormat_R8UNorm:                    return VK_FORMAT_R8_UNORM;
        case RHIPixelFormatType::RHI_PixelFormat_R8SNorm:                    return VK_FORMAT_R8_SNORM;
        case RHIPixelFormatType::RHI_PixelFormat_R8UInt:                     return VK_FORMAT_R8_UINT;
        case RHIPixelFormatType::RHI_PixelFormat_R8SInt:                     return VK_FORMAT_R8_SINT;
        //16-Bits
        case RHIPixelFormatType::RHI_PixelFormat_R16UInt:                    return VK_FORMAT_R16_UINT;
        case RHIPixelFormatType::RHI_PixelFormat_R16SInt:                    return VK_FORMAT_R16_SINT;
        case RHIPixelFormatType::RHI_PixelFormat_R16Float:                   return VK_FORMAT_R16_SFLOAT;
        case RHIPixelFormatType::RHI_PixelFormat_RG8UNorm:                   return VK_FORMAT_R8G8_UNORM;
        case RHIPixelFormatType::RHI_PixelFormat_RG8SNorm:                   return VK_FORMAT_R8G8_SNORM;
        case RHIPixelFormatType::RHI_PixelFormat_RG8UInt:                    return VK_FORMAT_R8G8_UINT;
        case RHIPixelFormatType::RHI_PixelFormat_RG8SInt:                    return VK_FORMAT_R8G8_SINT;
        //32-Bits
        case RHIPixelFormatType::RHI_PixelFormat_R32UInt:                    return VK_FORMAT_R32_UINT;
        case RHIPixelFormatType::RHI_PixelFormat_R32SInt:                    return VK_FORMAT_R32_SINT;
        case RHIPixelFormatType::RHI_PixelFormat_R32Float:                   return VK_FORMAT_R32_SFLOAT;
        case RHIPixelFormatType::RHI_PixelFormat_RG16UInt:                   return VK_FORMAT_R16G16_UINT;
        case RHIPixelFormatType::RHI_PixelFormat_RG16SInt:                   return VK_FORMAT_R16G16_SINT;
        case RHIPixelFormatType::RHI_PixelFormat_RG16Float:                  return VK_FORMAT_R16G16_SFLOAT;
        case RHIPixelFormatType::RHI_PixelFormat_RGBA8UNorm:                 return VK_FORMAT_R8G8B8A8_UNORM;
        case RHIPixelFormatType::RHI_PixelFormat_RGBA8UNormSRGB:             return VK_FORMAT_R8G8B8A8_SRGB;
        case RHIPixelFormatType::RHI_PixelFormat_RGBA8SNorm:                 return VK_FORMAT_R8G8B8A8_SNORM;
        case RHIPixelFormatType::RHI_PixelFormat_RGBA8UInt:                  return VK_FORMAT_R8G8B8A8_UINT;
        case RHIPixelFormatType::RHI_PixelFormat_RGBA8SInt:                  return VK_FORMAT_R8G8B8A8_SINT;
        case RHIPixelFormatType::RHI_PixelFormat_BGRA8UNorm:                 return VK_FORMAT_B8G8R8A8_UNORM;
        case RHIPixelFormatType::RHI_PixelFormat_BGRA8UNormSRGB:             return VK_FORMAT_B8G8R8A8_SRGB;
        case RHIPixelFormatType::RHI_PixelFormat_RGB9E5Float:                return VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;
        case RHIPixelFormatType::RHI_PixelFormat_RGB10A2UNorm:               return VK_FORMAT_A2R10G10B10_UNORM_PACK32;
        case RHIPixelFormatType::RHI_PixelFormat_RG11B10Float:               return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
        //64-Bits
        case RHIPixelFormatType::RHI_PixelFormat_RG32UInt:                   return VK_FORMAT_R32G32_UINT;
        case RHIPixelFormatType::RHI_PixelFormat_RG32SInt:                   return VK_FORMAT_R32G32_SINT;
        case RHIPixelFormatType::RHI_PixelFormat_RG32Float:                  return VK_FORMAT_R32G32_SFLOAT;
        case RHIPixelFormatType::RHI_PixelFormat_RGBA16UInt:                 return VK_FORMAT_R16G16B16A16_UINT;
        case RHIPixelFormatType::RHI_PixelFormat_RGBA16SInt:                 return VK_FORMAT_R16G16B16A16_SINT;
        case RHIPixelFormatType::RHI_PixelFormat_RGBA16Float:                return VK_FORMAT_R16G16B16A16_SFLOAT;
        //128-Bits
        case RHIPixelFormatType::RHI_PixelFormat_RGBA32UInt:                 return VK_FORMAT_R32G32B32A32_UINT;
        case RHIPixelFormatType::RHI_PixelFormat_RGBA32SInt:                 return VK_FORMAT_R32G32B32A32_SINT;
        case RHIPixelFormatType::RHI_PixelFormat_RGBA32Float:                return VK_FORMAT_R32G32B32A32_SFLOAT;
        //Depth-Stencil
        case RHIPixelFormatType::RHI_PixelFormat_D16UNorm:                   return VK_FORMAT_D16_UNORM;
        case RHIPixelFormatType::RHI_PixelFormat_D24UNormS8UInt:             return VK_FORMAT_D24_UNORM_S8_UINT;
        case RHIPixelFormatType::RHI_PixelFormat_D32Float:                   return VK_FORMAT_D32_SFLOAT;
        case RHIPixelFormatType::RHI_PixelFormat_D32FloatS8UInt:             return VK_FORMAT_D32_SFLOAT_S8_UINT;
        //Features /BC/ETC/ASTC

        case RHIPixelFormatType::RHI_PixelFormat_Unknown:                    return VK_FORMAT_UNDEFINED;
        case RHIPixelFormatType::RHI_PixelFormat_Count:                      return VK_FORMAT_MAX_ENUM;
        }
        F_Assert(false && "RHIVulkanConverter::TransformToVkFormat: Wrong RHIPixelFormatType type !")
        return VK_FORMAT_UNDEFINED;
    }


}; //LostPeterPluginRHIVulkan