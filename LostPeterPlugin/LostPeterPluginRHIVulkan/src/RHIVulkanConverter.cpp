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
    RHIExtent<3> TransformFromVkExtent3D(const VkExtent3D& vkExtent3D)
    {
        RHIExtent<3> sExtent;
        sExtent.x = (uint32)vkExtent3D.width;
        sExtent.y = (uint32)vkExtent3D.height;
        sExtent.z = (uint32)vkExtent3D.depth;
        return sExtent;
    }
    RHIPhysicalDeviceType RHIVulkanConverter::TransformFromVkPhysicalDeviceType(VkPhysicalDeviceType vkPhysicalDevice)
    {
        switch ((int32)vkPhysicalDevice)
        {
        case VK_PHYSICAL_DEVICE_TYPE_OTHER:                 return RHIPhysicalDeviceType::RHI_PhysicalDevice_UnKnown;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:        return RHIPhysicalDeviceType::RHI_PhysicalDevice_GPUIntegrated;
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:          return RHIPhysicalDeviceType::RHI_PhysicalDevice_GPUDiscrete;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:           return RHIPhysicalDeviceType::RHI_PhysicalDevice_GPUVirtual;
        case VK_PHYSICAL_DEVICE_TYPE_CPU:                   return RHIPhysicalDeviceType::RHI_PhysicalDevice_CPU;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkPhysicalDeviceType: Wrong VkPhysicalDeviceType type !")
        }
        return RHIPhysicalDeviceType::RHI_PhysicalDevice_UnKnown; 
    }

    RHIPixelFormatType RHIVulkanConverter::TransformFromVkFormat(VkFormat vkFormat)
    {
        switch ((int32)vkFormat)
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

        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkFormat: Wrong VkFormat type !")
        }
        return RHIPixelFormatType::RHI_PixelFormat_Unknown;
    }

    RHIFilterType RHIVulkanConverter::TransformFromVkFilter(VkFilter vkFilter)
    {
        switch ((int32)vkFilter)
        {
        case VK_FILTER_NEAREST:         return RHIFilterType::RHI_Filter_Nearest;
        case VK_FILTER_LINEAR:          return RHIFilterType::RHI_Filter_Linear;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkFilter: Wrong VkFilter type !")
        }
        return RHIFilterType::RHI_Filter_Linear;
    }

    RHIFilterType RHIVulkanConverter::TransformFromVkSamplerMipmapMode(VkSamplerMipmapMode vkSamplerMipmapMode)
    {
        switch ((int32)vkSamplerMipmapMode)
        {
        case VK_SAMPLER_MIPMAP_MODE_NEAREST:         return RHIFilterType::RHI_Filter_Nearest;
        case VK_SAMPLER_MIPMAP_MODE_LINEAR:          return RHIFilterType::RHI_Filter_Linear;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkSamplerMipmapMode: Wrong VkSamplerMipmapMode type !")
        }
        return RHIFilterType::RHI_Filter_Linear;
    }

    RHIAddressType RHIVulkanConverter::TransformFromVkSamplerAddressMode(VkSamplerAddressMode vkSamplerAddressMode)
    {
        switch ((int32)vkSamplerAddressMode)
        {
        case VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE:         return RHIAddressType::RHI_Address_ClampToEdge;
        case VK_SAMPLER_ADDRESS_MODE_REPEAT:                return RHIAddressType::RHI_Address_Repeat;
        case VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT:       return RHIAddressType::RHI_Address_MirrorRepeat;
        case VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER:       return RHIAddressType::RHI_Address_Border;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkSamplerAddressMode: Wrong VkSamplerAddressMode type !")
        }
        return RHIAddressType::RHI_Address_ClampToEdge;
    }

    RHIBorderColorType RHIVulkanConverter::TransformFromVkBorderColor(VkBorderColor vkBorderColor)
    {
        switch ((int32)vkBorderColor)
        {
        case VK_BORDER_COLOR_INT_OPAQUE_BLACK:           return RHIBorderColorType::RHI_BorderColor_OpaqueBlack;
        case VK_BORDER_COLOR_INT_OPAQUE_WHITE:           return RHIBorderColorType::RHI_BorderColor_OpaqueWhite;
        case VK_BORDER_COLOR_INT_TRANSPARENT_BLACK:      return RHIBorderColorType::RHI_BorderColor_TransparentBlack;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkBorderColor: Wrong VkBorderColor type !")
        }
        return RHIBorderColorType::RHI_BorderColor_OpaqueBlack;
    }

    RHIComparisonFuncType RHIVulkanConverter::TransformFromVkCompareOp(VkCompareOp vkCompareOp)
    {
        switch ((int32)vkCompareOp)
        {
        case VK_COMPARE_OP_NEVER:               return RHIComparisonFuncType::RHI_ComparisonFunc_Never;
        case VK_COMPARE_OP_LESS:                return RHIComparisonFuncType::RHI_ComparisonFunc_Less;
        case VK_COMPARE_OP_EQUAL:               return RHIComparisonFuncType::RHI_ComparisonFunc_Equal;
        case VK_COMPARE_OP_LESS_OR_EQUAL:       return RHIComparisonFuncType::RHI_ComparisonFunc_LessEqual;
        case VK_COMPARE_OP_GREATER:             return RHIComparisonFuncType::RHI_ComparisonFunc_Greater;
        case VK_COMPARE_OP_NOT_EQUAL:           return RHIComparisonFuncType::RHI_ComparisonFunc_NotEqual;
        case VK_COMPARE_OP_GREATER_OR_EQUAL:    return RHIComparisonFuncType::RHI_ComparisonFunc_GreaterEqual;
        case VK_COMPARE_OP_ALWAYS:              return RHIComparisonFuncType::RHI_ComparisonFunc_Always;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkCompareOp: Wrong VkCompareOp type !")
        }
        return RHIComparisonFuncType::RHI_ComparisonFunc_LessEqual;;
    }

    RHITextureDimensionType RHIVulkanConverter::TransformFromVkImageType(VkImageType vkImageType)
    {
        switch ((int32)vkImageType)
        {
        case VK_IMAGE_TYPE_1D:          return RHITextureDimensionType::RHI_TextureDimension_1D;
        case VK_IMAGE_TYPE_2D:          return RHITextureDimensionType::RHI_TextureDimension_2D;
        case VK_IMAGE_TYPE_3D:          return RHITextureDimensionType::RHI_TextureDimension_3D;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkImageType: Wrong VkImageType type !")
        }
        return RHITextureDimensionType::RHI_TextureDimension_2D;
    }

    RHITextureViewDimensionType RHIVulkanConverter::TransformFromVkImageViewType(VkImageViewType vkImageViewType)
    {
        switch ((int32)vkImageViewType)
        {
        case VK_IMAGE_VIEW_TYPE_1D:             return RHITextureViewDimensionType::RHI_TextureViewDimension_1D;
        case VK_IMAGE_VIEW_TYPE_2D:             return RHITextureViewDimensionType::RHI_TextureViewDimension_2D;
        case VK_IMAGE_VIEW_TYPE_2D_ARRAY:       return RHITextureViewDimensionType::RHI_TextureViewDimension_2DArray;
        case VK_IMAGE_VIEW_TYPE_CUBE:           return RHITextureViewDimensionType::RHI_TextureViewDimension_Cube;
        case VK_IMAGE_VIEW_TYPE_CUBE_ARRAY:     return RHITextureViewDimensionType::RHI_TextureViewDimension_CubeArray;
        case VK_IMAGE_VIEW_TYPE_3D:             return RHITextureViewDimensionType::RHI_TextureViewDimension_3D;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkImageViewType: Wrong VkImageViewType type !")
        }
        return RHITextureViewDimensionType::RHI_TextureViewDimension_2D;
    }

    RHISampleCountType RHIVulkanConverter::TransformFromVkSampleCountFlagBits(VkSampleCountFlagBits vkSampleCountFlagBits)
    {
        switch ((int32)vkSampleCountFlagBits)
        {
        case VK_SAMPLE_COUNT_1_BIT:          return RHISampleCountType::RHI_SampleCount_1_Bit;
        case VK_SAMPLE_COUNT_2_BIT:          return RHISampleCountType::RHI_SampleCount_2_Bit;
        case VK_SAMPLE_COUNT_4_BIT:          return RHISampleCountType::RHI_SampleCount_4_Bit;
        case VK_SAMPLE_COUNT_8_BIT:          return RHISampleCountType::RHI_SampleCount_8_Bit;
        case VK_SAMPLE_COUNT_16_BIT:         return RHISampleCountType::RHI_SampleCount_16_Bit;
        case VK_SAMPLE_COUNT_32_BIT:         return RHISampleCountType::RHI_SampleCount_32_Bit;
        case VK_SAMPLE_COUNT_64_BIT:         return RHISampleCountType::RHI_SampleCount_64_Bit;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkSampleCountFlagBits: Wrong VkSampleCountFlagBits type !")
        }
        return RHISampleCountType::RHI_SampleCount_1_Bit;
    }


    RHIBufferUsageBitsType RHIVulkanConverter::TransformFromVkBufferUsageFlags(VkBufferUsageFlags vkBufferUsageFlags)
    {   
        switch ((int32)vkBufferUsageFlags)
        {
        case VK_BUFFER_USAGE_TRANSFER_SRC_BIT:       return RHIBufferUsageBitsType::RHI_BufferUsageBits_CopySrc;
        case VK_BUFFER_USAGE_TRANSFER_DST_BIT:       return RHIBufferUsageBitsType::RHI_BufferUsageBits_CopyDst;
        case VK_BUFFER_USAGE_INDEX_BUFFER_BIT:       return RHIBufferUsageBitsType::RHI_BufferUsageBits_Index;
        case VK_BUFFER_USAGE_VERTEX_BUFFER_BIT:      return RHIBufferUsageBitsType::RHI_BufferUsageBits_Vertex;
        case VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT:     return RHIBufferUsageBitsType::RHI_BufferUsageBits_Uniform;
        case VK_BUFFER_USAGE_STORAGE_BUFFER_BIT:     return RHIBufferUsageBitsType::RHI_BufferUsageBits_Storage;
        case VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT:    return RHIBufferUsageBitsType::RHI_BufferUsageBits_Indirect;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkBufferUsageFlags: Wrong VkBufferUsageFlags type !")
        }
        return RHIBufferUsageBitsType::RHI_BufferUsageBits_Uniform;
    }

    RHITextureUsageBitsType RHIVulkanConverter::TransformFromVkImageUsageFlags(VkImageUsageFlags vkImageUsageFlags)
    {
        switch ((int32)vkImageUsageFlags)
        {
        case VK_IMAGE_USAGE_TRANSFER_SRC_BIT:               return RHITextureUsageBitsType::RHI_TextureUsageBits_CopySrc;
        case VK_IMAGE_USAGE_TRANSFER_DST_BIT:               return RHITextureUsageBitsType::RHI_TextureUsageBits_CopyDst;
        case VK_IMAGE_USAGE_SAMPLED_BIT:                    return RHITextureUsageBitsType::RHI_TextureUsageBits_TextureBinding;
        case VK_IMAGE_USAGE_STORAGE_BIT:                    return RHITextureUsageBitsType::RHI_TextureUsageBits_StorageBinding;
        case VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT:           return RHITextureUsageBitsType::RHI_TextureUsageBits_RenderAttachment;
        case VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT:   return RHITextureUsageBitsType::RHI_TextureUsageBits_DepthStencilAttachment;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkImageUsageFlags: Wrong VkImageUsageFlags type !")
        }
        return RHITextureUsageBitsType::RHI_TextureUsageBits_TextureBinding;
    }



    ////////////////////// TransformToXXXX ////////////////////////
    VkExtent3D RHIVulkanConverter::TransformToVkExtent3D(const RHIExtent<3>& sExtent)
    {
        return { static_cast<uint32_t>(sExtent.x), static_cast<uint32_t>(sExtent.y), static_cast<uint32_t>(sExtent.z) };
    }

    VkPhysicalDeviceType RHIVulkanConverter::TransformToVkPhysicalDeviceType(RHIPhysicalDeviceType ePhysicalDevice)
    {
        switch (ePhysicalDevice)
        {
        case RHIPhysicalDeviceType::RHI_PhysicalDevice_UnKnown:         return VK_PHYSICAL_DEVICE_TYPE_OTHER;
        case RHIPhysicalDeviceType::RHI_PhysicalDevice_GPUIntegrated:   return VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;
        case RHIPhysicalDeviceType::RHI_PhysicalDevice_GPUDiscrete:     return VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
        case RHIPhysicalDeviceType::RHI_PhysicalDevice_GPUVirtual:      return VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU;
        case RHIPhysicalDeviceType::RHI_PhysicalDevice_CPU:             return VK_PHYSICAL_DEVICE_TYPE_CPU;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkPhysicalDeviceType: Wrong RHIPhysicalDeviceType type !")
        }
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

        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkFormat: Wrong RHIPixelFormatType type !")
        }
        return VK_FORMAT_UNDEFINED;
    }

    static const String s_nameVkFormatTypes[] = 
    {
        //8-Bits
        "VK_FORMAT_R8_UNORM",                      //0:    R8Unorm
        "VK_FORMAT_R8_SNORM",                      //1:    R8SNorm
        "VK_FORMAT_R8_UINT",                       //2:    R8UInt
        "VK_FORMAT_R8_SINT",                       //3:    R8SInt

        //16-Bits
        "VK_FORMAT_R16_UINT",                      //4:    R16UInt
        "VK_FORMAT_R16_SINT",                      //5:    R16SInt
        "VK_FORMAT_R16_SFLOAT",                    //6:    R16Float
        "VK_FORMAT_R8G8_UNORM",                    //7:    RG8UNorm
        "VK_FORMAT_R8G8_SNORM",                    //8:    RG8SNorm
        "VK_FORMAT_R8G8_UINT",                     //9:    RG8UInt
        "VK_FORMAT_R8G8_SINT",                     //10:   RG8SInt

        //32-Bits
        "VK_FORMAT_R32_UINT",                      //11:   R32UInt
        "VK_FORMAT_R32_SINT",                      //12:   R32UInt
        "VK_FORMAT_R32_SFLOAT",                    //13:   R32Float
        "VK_FORMAT_R16G16_UINT",                   //14:   RG16UInt
        "VK_FORMAT_R16G16_SINT",                   //15:   RG16SInt
        "VK_FORMAT_R16G16_SFLOAT",                 //16:   RG16Float
        "VK_FORMAT_R8G8B8A8_UNORM",                //17:   RGBA8UNorm
        "VK_FORMAT_R8G8B8A8_SRGB",                 //18:   RGBA8UNormSRGB
        "VK_FORMAT_R8G8B8A8_SNORM",                //19:   RGBA8SNorm
        "VK_FORMAT_R8G8B8A8_UINT",                 //20:   RGBA8UInt
        "VK_FORMAT_R8G8B8A8_SINT",                 //21:   RGBA8SInt
        "VK_FORMAT_B8G8R8A8_UNORM",                //22:   BGRA8UNorm
        "VK_FORMAT_B8G8R8A8_SRGB",                 //23:   BGRA8UNormSRGB
        "VK_FORMAT_E5B9G9R9_UFLOAT_PACK32",        //24:   RGB9E5Float
        "VK_FORMAT_A2R10G10B10_UNORM_PACK32",      //25:   RGB10A2UNorm
        "VK_FORMAT_B10G11R11_UFLOAT_PACK32",       //26:   RG11B10Float

        //64-Bits
        "VK_FORMAT_R32G32_UINT",                   //27:   RG32UInt
        "VK_FORMAT_R32G32_SINT",                   //28:   RG32SInt
        "VK_FORMAT_R32G32_SFLOAT",                 //29:   RG32Float
        "VK_FORMAT_R16G16B16A16_UINT",             //30:   RGBA16UInt
        "VK_FORMAT_R16G16B16A16_SINT",             //31:   RGBA16SInt
        "VK_FORMAT_R16G16B16A16_SFLOAT",           //32:   RGBA16Float

        //128-Bits
        "VK_FORMAT_R32G32B32A32_UINT",             //33:   RGBA32UInt
        "VK_FORMAT_R32G32B32A32_SINT",             //34:   RGBA32SInt
        "VK_FORMAT_R32G32B32A32_SFLOAT",           //35:   RGBA32Float

        //Depth-Stencil
        "VK_FORMAT_D16_UNORM",                     //36:   D16UNorm
        "VK_FORMAT_D24_UNORM_S8_UINT",             //37:   D24UNormS8UInt
        "VK_FORMAT_D32_SFLOAT",                    //38:   D32Float
        "VK_FORMAT_D32_SFLOAT_S8_UINT",            //39:   D32FloatS8UInt

        //Features /BC/ETC/ASTC

        "VK_FORMAT_UNDEFINED",                     //40:   Unknown
    };
    const String& RHIVulkanConverter::TransformToVkFormatName(RHIPixelFormatType ePixelFormat)
    {
        return s_nameVkFormatTypes[(uint32)ePixelFormat];
    }

    VkFilter RHIVulkanConverter::TransformToVkFilter(RHIFilterType eFilter)
    {
        switch (eFilter)
        {
        case RHIFilterType::RHI_Filter_Nearest:         return VK_FILTER_NEAREST;
        case RHIFilterType::RHI_Filter_Linear:          return VK_FILTER_LINEAR;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkFilter: Wrong RHIFilterType type !")
        }
        return VK_FILTER_LINEAR;
    }

    VkSamplerMipmapMode RHIVulkanConverter::TransformToVkSamplerMipmapMode(RHIFilterType eFilter)
    {
        switch (eFilter)
        {
        case RHIFilterType::RHI_Filter_Nearest:         return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        case RHIFilterType::RHI_Filter_Linear:          return VK_SAMPLER_MIPMAP_MODE_LINEAR;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkSamplerMipmapMode: Wrong RHIFilterType type !")
        }
        return VK_SAMPLER_MIPMAP_MODE_LINEAR;
    }

    VkSamplerAddressMode RHIVulkanConverter::TransformToVkSamplerAddressMode(RHIAddressType eAddress)
    {
        switch (eAddress)
        {
        case RHIAddressType::RHI_Address_ClampToEdge:       return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        case RHIAddressType::RHI_Address_Repeat:            return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        case RHIAddressType::RHI_Address_MirrorRepeat:      return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
        case RHIAddressType::RHI_Address_Border:            return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkSamplerAddressMode: Wrong RHIAddressType type !")
        }
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    }

    VkBorderColor RHIVulkanConverter::TransformToVkBorderColor(RHIBorderColorType eBorderColor)
    {
        switch (eBorderColor)
        {
        case RHIBorderColorType::RHI_BorderColor_OpaqueBlack:           return VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        case RHIBorderColorType::RHI_BorderColor_OpaqueWhite:           return VK_BORDER_COLOR_INT_OPAQUE_WHITE;
        case RHIBorderColorType::RHI_BorderColor_TransparentBlack:      return VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkBorderColor: Wrong RHIBorderColorType type !")
        }
        return VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    }

    VkCompareOp RHIVulkanConverter::TransformToVkCompareOp(RHIComparisonFuncType eComparisonFunc)
    {
        switch (eComparisonFunc)
        {
        case RHIComparisonFuncType::RHI_ComparisonFunc_Never:           return VK_COMPARE_OP_NEVER;
        case RHIComparisonFuncType::RHI_ComparisonFunc_Less:            return VK_COMPARE_OP_LESS;
        case RHIComparisonFuncType::RHI_ComparisonFunc_Equal:           return VK_COMPARE_OP_EQUAL;
        case RHIComparisonFuncType::RHI_ComparisonFunc_LessEqual:       return VK_COMPARE_OP_LESS_OR_EQUAL;
        case RHIComparisonFuncType::RHI_ComparisonFunc_Greater:         return VK_COMPARE_OP_GREATER;
        case RHIComparisonFuncType::RHI_ComparisonFunc_NotEqual:        return VK_COMPARE_OP_NOT_EQUAL;
        case RHIComparisonFuncType::RHI_ComparisonFunc_GreaterEqual:    return VK_COMPARE_OP_GREATER_OR_EQUAL;
        case RHIComparisonFuncType::RHI_ComparisonFunc_Always:          return VK_COMPARE_OP_ALWAYS;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkCompareOp: Wrong RHIComparisonFuncType type !")
        }
        return VK_COMPARE_OP_LESS_OR_EQUAL;
    }

    VkImageType RHIVulkanConverter::TransformToVkImageType(RHITextureDimensionType eTextureDimension)
    {
        switch (eTextureDimension)
        {
        case RHITextureDimensionType::RHI_TextureDimension_1D:          return VK_IMAGE_TYPE_1D;
        case RHITextureDimensionType::RHI_TextureDimension_2D:          return VK_IMAGE_TYPE_2D;
        case RHITextureDimensionType::RHI_TextureDimension_3D:          return VK_IMAGE_TYPE_3D;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkImageType: Wrong RHITextureDimensionType type !")
        }
        return VK_IMAGE_TYPE_2D;
    }

    VkImageViewType RHIVulkanConverter::TransformToVkImageViewType(RHITextureViewDimensionType eTextureViewDimension)
    {
        switch (eTextureViewDimension)
        {
        case RHITextureViewDimensionType::RHI_TextureViewDimension_1D:          return VK_IMAGE_VIEW_TYPE_1D;
        case RHITextureViewDimensionType::RHI_TextureViewDimension_2D:          return VK_IMAGE_VIEW_TYPE_2D;
        case RHITextureViewDimensionType::RHI_TextureViewDimension_2DArray:     return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        case RHITextureViewDimensionType::RHI_TextureViewDimension_Cube:        return VK_IMAGE_VIEW_TYPE_CUBE;
        case RHITextureViewDimensionType::RHI_TextureViewDimension_CubeArray:   return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
        case RHITextureViewDimensionType::RHI_TextureViewDimension_3D:          return VK_IMAGE_VIEW_TYPE_3D;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkImageViewType: Wrong RHITextureViewDimensionType type !")
        }
        return VK_IMAGE_VIEW_TYPE_2D;
    }

    VkSampleCountFlagBits RHIVulkanConverter::TransformToVkSampleCountFlagBits(RHISampleCountType eSampleCount)
    {
        switch (eSampleCount)
        {
        case RHISampleCountType::RHI_SampleCount_1_Bit:          return VK_SAMPLE_COUNT_1_BIT;
        case RHISampleCountType::RHI_SampleCount_2_Bit:          return VK_SAMPLE_COUNT_2_BIT;
        case RHISampleCountType::RHI_SampleCount_4_Bit:          return VK_SAMPLE_COUNT_4_BIT;
        case RHISampleCountType::RHI_SampleCount_8_Bit:          return VK_SAMPLE_COUNT_8_BIT;
        case RHISampleCountType::RHI_SampleCount_16_Bit:         return VK_SAMPLE_COUNT_16_BIT;
        case RHISampleCountType::RHI_SampleCount_32_Bit:         return VK_SAMPLE_COUNT_32_BIT;
        case RHISampleCountType::RHI_SampleCount_64_Bit:         return VK_SAMPLE_COUNT_64_BIT;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkSampleCountFlagBits: Wrong RHISampleCountType type !")
        }
        return VK_SAMPLE_COUNT_1_BIT;
    }


    VkBufferUsageFlags RHIVulkanConverter::TransformToVkBufferUsageFlags(RHIBufferUsageBitsType eBufferUsageBits)
    {
        switch (eBufferUsageBits)
        {
        case RHIBufferUsageBitsType::RHI_BufferUsageBits_CopySrc:       return VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        case RHIBufferUsageBitsType::RHI_BufferUsageBits_CopyDst:       return VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        case RHIBufferUsageBitsType::RHI_BufferUsageBits_Index:         return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        case RHIBufferUsageBitsType::RHI_BufferUsageBits_Vertex:        return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        case RHIBufferUsageBitsType::RHI_BufferUsageBits_Uniform:       return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        case RHIBufferUsageBitsType::RHI_BufferUsageBits_Storage:       return VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
        case RHIBufferUsageBitsType::RHI_BufferUsageBits_Indirect:      return VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkBufferUsageFlags: Wrong RHIBufferUsageBitsType type !")
        }
        return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    }
    VkBufferUsageFlags RHIVulkanConverter::TransformToVkBufferUsageFlagsFromBufferUsageFlags(RHIBufferUsageFlags flagsBufferUsages)
    {
        static std::map<RHIBufferUsageBitsType, VkBufferUsageFlags> s_Rules = 
        {
            { RHIBufferUsageBitsType::RHI_BufferUsageBits_CopySrc,  VK_BUFFER_USAGE_TRANSFER_SRC_BIT },
            { RHIBufferUsageBitsType::RHI_BufferUsageBits_CopyDst,  VK_BUFFER_USAGE_TRANSFER_DST_BIT },
            { RHIBufferUsageBitsType::RHI_BufferUsageBits_Index,    VK_BUFFER_USAGE_INDEX_BUFFER_BIT },
            { RHIBufferUsageBitsType::RHI_BufferUsageBits_Vertex,   VK_BUFFER_USAGE_VERTEX_BUFFER_BIT },
            { RHIBufferUsageBitsType::RHI_BufferUsageBits_Uniform,  VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT },
            { RHIBufferUsageBitsType::RHI_BufferUsageBits_Storage,  VK_BUFFER_USAGE_STORAGE_BUFFER_BIT },
            { RHIBufferUsageBitsType::RHI_BufferUsageBits_Indirect, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT },
        };
        VkBufferUsageFlags vkResult = {};
        for (std::map<RHIBufferUsageBitsType, VkBufferUsageFlags>::iterator it = s_Rules.begin();
             it != s_Rules.end(); ++it)
        {
            if (flagsBufferUsages & it->first) 
            {
                vkResult |= it->second;
            }
        }
        return vkResult;
    }


    VkImageUsageFlags RHIVulkanConverter::TransformToVkImageUsageFlags(RHITextureUsageBitsType eTextureUsageBits)
    {
        switch (eTextureUsageBits)
        {
        case RHITextureUsageBitsType::RHI_TextureUsageBits_CopySrc:                 return VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        case RHITextureUsageBitsType::RHI_TextureUsageBits_CopyDst:                 return VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        case RHITextureUsageBitsType::RHI_TextureUsageBits_TextureBinding:          return VK_IMAGE_USAGE_SAMPLED_BIT;
        case RHITextureUsageBitsType::RHI_TextureUsageBits_StorageBinding:          return VK_IMAGE_USAGE_STORAGE_BIT;
        case RHITextureUsageBitsType::RHI_TextureUsageBits_RenderAttachment:        return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        case RHITextureUsageBitsType::RHI_TextureUsageBits_DepthStencilAttachment:  return VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkImageUsageFlags: Wrong RHITextureUsageBitsType type !")
        }
        return VK_IMAGE_USAGE_SAMPLED_BIT;
    }
    VkImageUsageFlags RHIVulkanConverter::TransformToVkImageUsageFlagsFromTextureUsageFlags(RHITextureUsageFlags flagsTextureUsages)
    {
         static std::map<RHITextureUsageBitsType, VkImageUsageFlags> s_Rules = 
         {
            { RHITextureUsageBitsType::RHI_TextureUsageBits_CopySrc,                VK_IMAGE_USAGE_TRANSFER_SRC_BIT },
            { RHITextureUsageBitsType::RHI_TextureUsageBits_CopyDst,                VK_IMAGE_USAGE_TRANSFER_DST_BIT },
            { RHITextureUsageBitsType::RHI_TextureUsageBits_TextureBinding,         VK_IMAGE_USAGE_SAMPLED_BIT },
            { RHITextureUsageBitsType::RHI_TextureUsageBits_StorageBinding,         VK_IMAGE_USAGE_STORAGE_BIT },
            { RHITextureUsageBitsType::RHI_TextureUsageBits_RenderAttachment,       VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT },
            { RHITextureUsageBitsType::RHI_TextureUsageBits_DepthStencilAttachment, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT },
        };

        VkImageUsageFlags vkResult = {};
        for (std::map<RHITextureUsageBitsType, VkImageUsageFlags>::iterator it = s_Rules.begin();
             it != s_Rules.end(); ++it)
        {
            if (flagsTextureUsages & it->first) 
            {
                vkResult |= it->second;
            }
        }
        return vkResult;
    }

}; //LostPeterPluginRHIVulkan