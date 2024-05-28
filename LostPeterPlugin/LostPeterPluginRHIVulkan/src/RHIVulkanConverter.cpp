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
    RHIExtent<2> RHIVulkanConverter::TransformFromVkExtent2D(const VkExtent2D& vkExtent2D)
    {
        RHIExtent<2> sExtent;
        sExtent.x = (uint32)vkExtent2D.width;
        sExtent.y = (uint32)vkExtent2D.height;
        return sExtent;
    }
    RHIExtent<3> RHIVulkanConverter::TransformFromVkExtent3D(const VkExtent3D& vkExtent3D)
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

    RHIColorSpaceType RHIVulkanConverter::TransformFromVkColorSpaceKHR(VkColorSpaceKHR vkColorSpaceKHR)
    {
        switch ((int32)vkColorSpaceKHR)
        {
        case VK_COLOR_SPACE_SRGB_NONLINEAR_KHR:             return RHIColorSpaceType::RHI_ColorSpace_Gamma;
        case VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT:        return RHIColorSpaceType::RHI_ColorSpace_Linear;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkColorSpaceKHR: Wrong VkColorSpaceKHR type !")
        }
        return RHIColorSpaceType::RHI_ColorSpace_Linear;
    }

    RHIPrimitiveTopologyType RHIVulkanConverter::TransformFromVkPrimitiveTopology(VkPrimitiveTopology vkPrimitiveTopology)
    {
        switch ((int32)vkPrimitiveTopology)
        {
        case VK_PRIMITIVE_TOPOLOGY_POINT_LIST:                      return RHIPrimitiveTopologyType::RHI_PrimitiveTopology_PointList;
        case VK_PRIMITIVE_TOPOLOGY_LINE_LIST:                       return RHIPrimitiveTopologyType::RHI_PrimitiveTopology_LineList;
        case VK_PRIMITIVE_TOPOLOGY_LINE_STRIP:                      return RHIPrimitiveTopologyType::RHI_PrimitiveTopology_LineStrip;
        case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST:                   return RHIPrimitiveTopologyType::RHI_PrimitiveTopology_TriangleList;
        case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP:                  return RHIPrimitiveTopologyType::RHI_PrimitiveTopology_TriangleStrip;
        case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN:                    return RHIPrimitiveTopologyType::RHI_PrimitiveTopology_TriangleFan;
        case VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY:        return RHIPrimitiveTopologyType::RHI_PrimitiveTopology_LineListAdj;
        case VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY:       return RHIPrimitiveTopologyType::RHI_PrimitiveTopology_LineStripAdj;
        case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY:    return RHIPrimitiveTopologyType::RHI_PrimitiveTopology_TriangleListAdj;
        case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY:   return RHIPrimitiveTopologyType::RHI_PrimitiveTopology_TriangleStripAdj;
        case VK_PRIMITIVE_TOPOLOGY_PATCH_LIST:                      return RHIPrimitiveTopologyType::RHI_PrimitiveTopology_PatchList;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkPrimitiveTopology: Wrong VkPrimitiveTopology type !")
        }
        return RHIPrimitiveTopologyType::RHI_PrimitiveTopology_TriangleList;
    }

    RHIFrontFaceType RHIVulkanConverter::TransformFromVkFrontFace(VkFrontFace vkFrontFace)
    {
        switch ((int32)vkFrontFace)
        {
        case VK_FRONT_FACE_CLOCKWISE:               return RHIFrontFaceType::RHI_FrontFace_ClockWise;
        case VK_FRONT_FACE_COUNTER_CLOCKWISE:       return RHIFrontFaceType::RHI_FrontFace_CounterClockWise;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkFrontFace: Wrong VkFrontFace type !")
        }
        return RHIFrontFaceType::RHI_FrontFace_CounterClockWise;
    }

    RHICullType RHIVulkanConverter::TransformFromVkCullModeFlagBits(VkCullModeFlagBits vkCullModeFlagBits)
    {
        switch ((int32)vkCullModeFlagBits)
        {
        case VK_CULL_MODE_NONE:             return RHICullType::RHI_Cull_None;
        case VK_CULL_MODE_FRONT_BIT:        return RHICullType::RHI_Cull_Front;
        case VK_CULL_MODE_BACK_BIT:         return RHICullType::RHI_Cull_Back;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkCullModeFlagBits: Wrong VkCullModeFlagBits type !")
        }
        return RHICullType::RHI_Cull_Back;
    }

    RHIPolygonType RHIVulkanConverter::TransformFromVkPolygonMode(VkPolygonMode vkPolygonMode)
    {
        switch ((int32)vkPolygonMode)
        {
        case VK_POLYGON_MODE_POINT:         return RHIPolygonType::RHI_Polygon_Point;
        case VK_POLYGON_MODE_LINE:          return RHIPolygonType::RHI_Polygon_WireFrame;
        case VK_POLYGON_MODE_FILL:          return RHIPolygonType::RHI_Polygon_Solid;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkPolygonMode: Wrong VkPolygonMode type !")
        }
        return RHIPolygonType::RHI_Polygon_Solid;
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
        return RHIComparisonFuncType::RHI_ComparisonFunc_Never;
    }

    RHIBlendOpType RHIVulkanConverter::TransformFromVkBlendOp(VkBlendOp vkBlendOp)
    {
        switch ((int32)vkBlendOp)
        {
        case VK_BLEND_OP_ADD:                   return RHIBlendOpType::RHI_BlendOp_Add;
        case VK_BLEND_OP_SUBTRACT:              return RHIBlendOpType::RHI_BlendOp_Substract;
        case VK_BLEND_OP_REVERSE_SUBTRACT:      return RHIBlendOpType::RHI_BlendOp_ReverseSubstract;
        case VK_BLEND_OP_MIN:                   return RHIBlendOpType::RHI_BlendOp_Min;
        case VK_BLEND_OP_MAX:                   return RHIBlendOpType::RHI_BlendOp_Max;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkBlendOp: Wrong VkBlendOp type !")
        }
        return RHIBlendOpType::RHI_BlendOp_Add;
    }

    RHIBlendFactorType RHIVulkanConverter::TransformFromVkBlendFactor(VkBlendFactor vkBlendFactor)
    {
        switch ((int32)vkBlendFactor)
        {
        case VK_BLEND_FACTOR_ZERO:                      return RHIBlendFactorType::RHI_BlendFactor_Zero;
        case VK_BLEND_FACTOR_ONE:                       return RHIBlendFactorType::RHI_BlendFactor_One;
        case VK_BLEND_FACTOR_SRC_COLOR:                 return RHIBlendFactorType::RHI_BlendFactor_Src;
        case VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR:       return RHIBlendFactorType::RHI_BlendFactor_OneMinusSrc;
        case VK_BLEND_FACTOR_SRC_ALPHA:                 return RHIBlendFactorType::RHI_BlendFactor_SrcAlpha;
        case VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:       return RHIBlendFactorType::RHI_BlendFactor_OneMinusSrcAlpha;
        case VK_BLEND_FACTOR_DST_COLOR:                 return RHIBlendFactorType::RHI_BlendFactor_Dst;
        case VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR:       return RHIBlendFactorType::RHI_BlendFactor_OneMinusDst;
        case VK_BLEND_FACTOR_DST_ALPHA:                 return RHIBlendFactorType::RHI_BlendFactor_DstAlpha;
        case VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA:       return RHIBlendFactorType::RHI_BlendFactor_OneMinusDstAlpha;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkBlendFactor: Wrong VkBlendFactor type !")
        }
        return RHIBlendFactorType::RHI_BlendFactor_One;
    }

    RHIStencilOpType RHIVulkanConverter::TransformFromVkStencilOp(VkStencilOp vkStencilOp)
    {
        switch ((int32)vkStencilOp)
        {
        case VK_STENCIL_OP_KEEP:                    return RHIStencilOpType::RHI_StencilOp_Keep;
        case VK_STENCIL_OP_ZERO:                    return RHIStencilOpType::RHI_StencilOp_Zero;
        case VK_STENCIL_OP_REPLACE:                 return RHIStencilOpType::RHI_StencilOp_Replace;
        case VK_STENCIL_OP_INVERT:                  return RHIStencilOpType::RHI_StencilOp_Invert;
        case VK_STENCIL_OP_INCREMENT_AND_CLAMP:     return RHIStencilOpType::RHI_StencilOp_IncrementClamp;
        case VK_STENCIL_OP_DECREMENT_AND_CLAMP:     return RHIStencilOpType::RHI_StencilOp_DecrementClamp;
        case VK_STENCIL_OP_INCREMENT_AND_WRAP:      return RHIStencilOpType::RHI_StencilOp_IncrementWrap;
        case VK_STENCIL_OP_DECREMENT_AND_WRAP:      return RHIStencilOpType::RHI_StencilOp_DecrementWrap;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkStencilOp: Wrong VkStencilOp type !")
        }
        return RHIStencilOpType::RHI_StencilOp_Keep;
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

    RHIPresentType RHIVulkanConverter::TransformFromVkPresentModeKHR(VkPresentModeKHR vkPresentModeKHR)
    {
        switch ((int32)vkPresentModeKHR)
        {
        case VK_PRESENT_MODE_IMMEDIATE_KHR:         return RHIPresentType::RHI_Present_Immediately;
        case VK_PRESENT_MODE_FIFO_KHR:              return RHIPresentType::RHI_Present_VSync;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkPresentModeKHR: Wrong VkPresentModeKHR type !")
        }
        return RHIPresentType::RHI_Present_Immediately;
    }

    RHIBindingType RHIVulkanConverter::TransformFromVkDescriptorType(VkDescriptorType vkDescriptorType)
    {
        switch ((int32)vkDescriptorType)
        {
        case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:             return RHIBindingType::RHI_Binding_UniformBuffer;
        case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:             return RHIBindingType::RHI_Binding_StorageBuffer;
        case VK_DESCRIPTOR_TYPE_SAMPLER:                    return RHIBindingType::RHI_Binding_Sampler;
        case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:     return RHIBindingType::RHI_Binding_CombinedImageSampler;
        case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:              return RHIBindingType::RHI_Binding_Texture;
        case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:              return RHIBindingType::RHI_Binding_StorageTexture;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkDescriptorType: Wrong VkDescriptorType type !")
        }
        return RHIBindingType::RHI_Binding_UniformBuffer;
    }

    RHITextureStateType RHIVulkanConverter::TransformFromVkImageLayout(VkImageLayout vkImageLayout)
    {
        switch ((int32)vkImageLayout)
        {
        case VK_IMAGE_LAYOUT_UNDEFINED:                         return RHITextureStateType::RHI_TextureState_UnDefined;
        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:              return RHITextureStateType::RHI_TextureState_CopySrc;
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:              return RHITextureStateType::RHI_TextureState_CopyDst;
        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:          return RHITextureStateType::RHI_TextureState_ShaderReadOnly;
        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:          return RHITextureStateType::RHI_TextureState_RenderTarget;
        case VK_IMAGE_LAYOUT_GENERAL:                           return RHITextureStateType::RHI_TextureState_Storage;
        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:   return RHITextureStateType::RHI_TextureState_DepthStencilReadonly;
        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:  return RHITextureStateType::RHI_TextureState_DepthStencilWrite;
        case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:                   return RHITextureStateType::RHI_TextureState_Present;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkImageLayout: Wrong VkImageLayout type !")
        }
        return RHITextureStateType::RHI_TextureState_UnDefined;
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

    RHIShaderStageBitsType RHIVulkanConverter::TransformFromVkShaderStageFlags(VkShaderStageFlags vkShaderStageFlags)
    {
        switch ((int32)vkShaderStageFlags)
        {
        case VK_SHADER_STAGE_VERTEX_BIT:                        return RHIShaderStageBitsType::RHI_ShaderStageBits_Vertex;
        case VK_SHADER_STAGE_FRAGMENT_BIT:                      return RHIShaderStageBitsType::RHI_ShaderStageBits_Pixel;
        case VK_SHADER_STAGE_COMPUTE_BIT:                       return RHIShaderStageBitsType::RHI_ShaderStageBits_Compute;
        case VK_SHADER_STAGE_GEOMETRY_BIT:                      return RHIShaderStageBitsType::RHI_ShaderStageBits_Geometry;
        case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:          return RHIShaderStageBitsType::RHI_ShaderStageBits_Domain;
        case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:       return RHIShaderStageBitsType::RHI_ShaderStageBits_Hull;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkShaderStageFlags: Wrong VkShaderStageFlags type !")
        }
        return RHIShaderStageBitsType::RHI_ShaderStageBits_Vertex;
    }


    ////////////////////// TransformToXXXX ////////////////////////
    VkExtent2D RHIVulkanConverter::TransformToVkExtent2D(const RHIExtent<2>& sExtent)
    {
        return { static_cast<uint32_t>(sExtent.x), static_cast<uint32_t>(sExtent.y) };
    }   
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

    VkColorSpaceKHR RHIVulkanConverter::TransformToVkColorSpaceKHR(RHIColorSpaceType eColorSpace)
    {
        switch (eColorSpace)
        {
        case RHIColorSpaceType::RHI_ColorSpace_Gamma:         return VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        case RHIColorSpaceType::RHI_ColorSpace_Linear:        return VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkColorSpaceKHR: Wrong RHIColorSpaceType type !")
        }
        return VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    }

    VkPrimitiveTopology RHIVulkanConverter::TransformToVkPrimitiveTopology(RHIPrimitiveTopologyType ePrimitiveTopology)
    {
        switch (ePrimitiveTopology)
        {
        case RHIPrimitiveTopologyType::RHI_PrimitiveTopology_PointList:         return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
        case RHIPrimitiveTopologyType::RHI_PrimitiveTopology_LineList:          return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
        case RHIPrimitiveTopologyType::RHI_PrimitiveTopology_LineStrip:         return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
        case RHIPrimitiveTopologyType::RHI_PrimitiveTopology_TriangleList:      return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        case RHIPrimitiveTopologyType::RHI_PrimitiveTopology_TriangleStrip:     return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
        case RHIPrimitiveTopologyType::RHI_PrimitiveTopology_TriangleFan:       return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
        case RHIPrimitiveTopologyType::RHI_PrimitiveTopology_LineListAdj:       return VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;
        case RHIPrimitiveTopologyType::RHI_PrimitiveTopology_LineStripAdj:      return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;
        case RHIPrimitiveTopologyType::RHI_PrimitiveTopology_TriangleListAdj:   return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;
        case RHIPrimitiveTopologyType::RHI_PrimitiveTopology_TriangleStripAdj:  return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;
        case RHIPrimitiveTopologyType::RHI_PrimitiveTopology_PatchList:         return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkPrimitiveTopology: Wrong RHIPrimitiveTopologyType type !")
        }
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    }

    VkFrontFace RHIVulkanConverter::TransformToVkFrontFace(RHIFrontFaceType eFrontFace)
    {
        switch (eFrontFace)
        {
        case RHIFrontFaceType::RHI_FrontFace_ClockWise:             return VK_FRONT_FACE_CLOCKWISE;
        case RHIFrontFaceType::RHI_FrontFace_CounterClockWise:      return VK_FRONT_FACE_COUNTER_CLOCKWISE;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkFrontFace: Wrong RHIFrontFaceType type !")
        }
        return VK_FRONT_FACE_COUNTER_CLOCKWISE;
    }

    VkCullModeFlagBits RHIVulkanConverter::TransformToVkCullModeFlagBits(RHICullType eCull)
    {
        switch (eCull)
        {
        case RHICullType::RHI_Cull_None:        return VK_CULL_MODE_NONE;
        case RHICullType::RHI_Cull_Front:       return VK_CULL_MODE_FRONT_BIT;
        case RHICullType::RHI_Cull_Back:        return VK_CULL_MODE_BACK_BIT;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkCullModeFlagBits: Wrong RHICullType type !")
        }
        return VK_CULL_MODE_BACK_BIT;
    }

    VkPolygonMode RHIVulkanConverter::TransformToVkPolygonMode(RHIPolygonType ePolygon)
    {
        switch (ePolygon)
        {
        case RHIPolygonType::RHI_Polygon_Point:        return VK_POLYGON_MODE_POINT;
        case RHIPolygonType::RHI_Polygon_WireFrame:    return VK_POLYGON_MODE_LINE;
        case RHIPolygonType::RHI_Polygon_Solid:        return VK_POLYGON_MODE_FILL;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkPolygonMode: Wrong RHIPolygonType type !")
        }
        return VK_POLYGON_MODE_FILL;
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
        return VK_COMPARE_OP_NEVER;
    }

    VkBlendOp RHIVulkanConverter::TransformToVkBlendOp(RHIBlendOpType eBlendOp)
    {
        switch (eBlendOp)
        {
        case RHIBlendOpType::RHI_BlendOp_Add:                   return VK_BLEND_OP_ADD;
        case RHIBlendOpType::RHI_BlendOp_Substract:             return VK_BLEND_OP_SUBTRACT;
        case RHIBlendOpType::RHI_BlendOp_ReverseSubstract:      return VK_BLEND_OP_REVERSE_SUBTRACT;
        case RHIBlendOpType::RHI_BlendOp_Min:                   return VK_BLEND_OP_MIN;
        case RHIBlendOpType::RHI_BlendOp_Max:                   return VK_BLEND_OP_MAX;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkBlendOp: Wrong RHIBlendOpType type !")
        }
        return VK_BLEND_OP_ADD;
    }

    VkBlendFactor RHIVulkanConverter::TransformToVkBlendFactor(RHIBlendFactorType eBlendFactor)
    {
        switch (eBlendFactor)
        {
        case RHIBlendFactorType::RHI_BlendFactor_Zero:                  return VK_BLEND_FACTOR_ZERO;
        case RHIBlendFactorType::RHI_BlendFactor_One:                   return VK_BLEND_FACTOR_ONE;
        case RHIBlendFactorType::RHI_BlendFactor_Src:                   return VK_BLEND_FACTOR_SRC_COLOR;
        case RHIBlendFactorType::RHI_BlendFactor_OneMinusSrc:           return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
        case RHIBlendFactorType::RHI_BlendFactor_SrcAlpha:              return VK_BLEND_FACTOR_SRC_ALPHA;
        case RHIBlendFactorType::RHI_BlendFactor_OneMinusSrcAlpha:      return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        case RHIBlendFactorType::RHI_BlendFactor_Dst:                   return VK_BLEND_FACTOR_DST_COLOR;
        case RHIBlendFactorType::RHI_BlendFactor_OneMinusDst:           return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
        case RHIBlendFactorType::RHI_BlendFactor_DstAlpha:              return VK_BLEND_FACTOR_DST_ALPHA;
        case RHIBlendFactorType::RHI_BlendFactor_OneMinusDstAlpha:      return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkBlendFactor: Wrong RHIBlendFactorType type !")
        }
        return VK_BLEND_FACTOR_ONE;
    }

    VkStencilOp RHIVulkanConverter::TransformToVkStencilOp(RHIStencilOpType eStencilOP)
    {
        switch (eStencilOP)
        {
        case RHIStencilOpType::RHI_StencilOp_Keep:                  return VK_STENCIL_OP_KEEP;
        case RHIStencilOpType::RHI_StencilOp_Zero:                  return VK_STENCIL_OP_ZERO;
        case RHIStencilOpType::RHI_StencilOp_Replace:               return VK_STENCIL_OP_REPLACE;
        case RHIStencilOpType::RHI_StencilOp_Invert:                return VK_STENCIL_OP_INVERT;
        case RHIStencilOpType::RHI_StencilOp_IncrementClamp:        return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
        case RHIStencilOpType::RHI_StencilOp_DecrementClamp:        return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
        case RHIStencilOpType::RHI_StencilOp_IncrementWrap:         return VK_STENCIL_OP_INCREMENT_AND_WRAP;
        case RHIStencilOpType::RHI_StencilOp_DecrementWrap:         return VK_STENCIL_OP_DECREMENT_AND_WRAP;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkStencilOp: Wrong RHIStencilOpType type !")
        }
        return VK_STENCIL_OP_KEEP;
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

    VkPresentModeKHR RHIVulkanConverter::TransformToVkPresentModeKHR(RHIPresentType ePresent)
    {
        switch (ePresent)
        {
        case RHIPresentType::RHI_Present_Immediately:       return VK_PRESENT_MODE_IMMEDIATE_KHR;
        case RHIPresentType::RHI_Present_VSync:             return VK_PRESENT_MODE_FIFO_KHR;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkPresentModeKHR: Wrong RHIPresentType type !")
        }
        return VK_PRESENT_MODE_IMMEDIATE_KHR;
    }

    VkDescriptorType RHIVulkanConverter::TransformToVkDescriptorType(RHIBindingType eBinding)
    {
        switch (eBinding)
        {
        case RHIBindingType::RHI_Binding_UniformBuffer:         return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        case RHIBindingType::RHI_Binding_StorageBuffer:         return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        case RHIBindingType::RHI_Binding_Sampler:               return VK_DESCRIPTOR_TYPE_SAMPLER;
        case RHIBindingType::RHI_Binding_CombinedImageSampler:  return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        case RHIBindingType::RHI_Binding_Texture:               return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        case RHIBindingType::RHI_Binding_StorageTexture:        return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkDescriptorType: Wrong RHIBindingType type !")
        }
        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    }

    VkImageLayout RHIVulkanConverter::TransformToVkImageLayout(RHITextureStateType eTextureState)
    {
        switch (eTextureState)
        {
        case RHITextureStateType::RHI_TextureState_UnDefined:               return VK_IMAGE_LAYOUT_UNDEFINED;
        case RHITextureStateType::RHI_TextureState_CopySrc:                 return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        case RHITextureStateType::RHI_TextureState_CopyDst:                 return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        case RHITextureStateType::RHI_TextureState_ShaderReadOnly:          return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        case RHITextureStateType::RHI_TextureState_RenderTarget:            return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        case RHITextureStateType::RHI_TextureState_Storage:                 return VK_IMAGE_LAYOUT_GENERAL;
        case RHITextureStateType::RHI_TextureState_DepthStencilReadonly:    return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
        case RHITextureStateType::RHI_TextureState_DepthStencilWrite:       return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        case RHITextureStateType::RHI_TextureState_Present:                 return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkImageLayout: Wrong RHITextureStateType type !")
        }
        return VK_IMAGE_LAYOUT_UNDEFINED;
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


    VkShaderStageFlags RHIVulkanConverter::TransformToVkShaderStageFlags(RHIShaderStageBitsType eShaderStageBits)
    {
        switch (eShaderStageBits)
        {
        case RHIShaderStageBitsType::RHI_ShaderStageBits_Vertex:            return VK_SHADER_STAGE_VERTEX_BIT;
        case RHIShaderStageBitsType::RHI_ShaderStageBits_Pixel:             return VK_SHADER_STAGE_FRAGMENT_BIT;
        case RHIShaderStageBitsType::RHI_ShaderStageBits_Compute:           return VK_SHADER_STAGE_COMPUTE_BIT;
        case RHIShaderStageBitsType::RHI_ShaderStageBits_Geometry:          return VK_SHADER_STAGE_GEOMETRY_BIT;
        case RHIShaderStageBitsType::RHI_ShaderStageBits_Domain:            return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        case RHIShaderStageBitsType::RHI_ShaderStageBits_Hull:              return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkShaderStageFlags: Wrong RHIShaderStageBitsType type !")
        }
        return VK_SHADER_STAGE_VERTEX_BIT;
    }
    VkShaderStageFlags RHIVulkanConverter::TransformToVkShaderStageFlagsFromShaderStagelags(RHIShaderStageFlags flagsShaderStages)
    {
        static std::map<RHIShaderStageBitsType, VkShaderStageFlags> s_Rules = 
         {
            { RHIShaderStageBitsType::RHI_ShaderStageBits_Vertex,       VK_SHADER_STAGE_VERTEX_BIT },
            { RHIShaderStageBitsType::RHI_ShaderStageBits_Pixel,        VK_SHADER_STAGE_FRAGMENT_BIT },
            { RHIShaderStageBitsType::RHI_ShaderStageBits_Compute,      VK_SHADER_STAGE_COMPUTE_BIT },
            { RHIShaderStageBitsType::RHI_ShaderStageBits_Geometry,     VK_SHADER_STAGE_GEOMETRY_BIT },
            { RHIShaderStageBitsType::RHI_ShaderStageBits_Domain,       VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT },
            { RHIShaderStageBitsType::RHI_ShaderStageBits_Hull,         VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT },
        };

        VkShaderStageFlags vkResult = {};
        for (std::map<RHIShaderStageBitsType, VkShaderStageFlags>::iterator it = s_Rules.begin();
             it != s_Rules.end(); ++it)
        {
            if (flagsShaderStages & it->first) 
            {
                vkResult |= it->second;
            }
        }
        return vkResult;
    }

}; //LostPeterPluginRHIVulkan