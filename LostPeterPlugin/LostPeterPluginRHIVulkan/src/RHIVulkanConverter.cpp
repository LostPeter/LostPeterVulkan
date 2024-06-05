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
#include "../include/RHIVulkanShaderModule.h"
#include "../include/RHIVulkanTexture.h"
#include "../include/RHIVulkanTextureView.h"

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

    RHILoadOpType RHIVulkanConverter::TransformFromVkAttachmentLoadOp(VkAttachmentLoadOp vkAttachmentLoadOp)
    {
        switch ((int32)vkAttachmentLoadOp)
        {
        case VK_ATTACHMENT_LOAD_OP_LOAD:            return RHILoadOpType::RHI_LoadOp_Load;
        case VK_ATTACHMENT_LOAD_OP_CLEAR:           return RHILoadOpType::RHI_LoadOp_Clear;
        case VK_ATTACHMENT_LOAD_OP_DONT_CARE:       return RHILoadOpType::RHI_LoadOp_DoNotCare;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkAttachmentLoadOp: Wrong VkAttachmentLoadOp type !")
        }
        return RHILoadOpType::RHI_LoadOp_Clear;
    }

    RHIStoreOpType RHIVulkanConverter::TransformFromVkAttachmentStoreOp(VkAttachmentStoreOp vkAttachmentStoreOp)
    {
        switch ((int32)vkAttachmentStoreOp)
        {
        case VK_ATTACHMENT_STORE_OP_STORE:          return RHIStoreOpType::RHI_StoreOp_Store;
        case VK_ATTACHMENT_STORE_OP_DONT_CARE:      return RHIStoreOpType::RHI_StoreOp_DoNotCare;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkAttachmentStoreOp: Wrong VkAttachmentStoreOp type !")
        }
        return RHIStoreOpType::RHI_StoreOp_DoNotCare;
    }

    RHIPipelineBindPointType RHIVulkanConverter::TransformFromVkPipelineBindPoint(VkPipelineBindPoint vkPipelineBindPoint)
    {
        switch ((int32)vkPipelineBindPoint)
        {
        case VK_PIPELINE_BIND_POINT_GRAPHICS:           return RHIPipelineBindPointType::RHI_PipelineBindPoint_Graphics;
        case VK_PIPELINE_BIND_POINT_COMPUTE:            return RHIPipelineBindPointType::RHI_PipelineBindPoint_Compute;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkPipelineBindPoint: Wrong VkPipelineBindPoint type !")
        }
        return RHIPipelineBindPointType::RHI_PipelineBindPoint_Graphics;
    }

    RHICommandBufferLevelType RHIVulkanConverter::TransformFromVkCommandBufferLevel(VkCommandBufferLevel vkCommandBufferLevel)
    {
        switch ((int32)vkCommandBufferLevel)
        {
        case VK_COMMAND_BUFFER_LEVEL_PRIMARY:           return RHICommandBufferLevelType::RHI_CommandBufferLevel_Primary;
        case VK_COMMAND_BUFFER_LEVEL_SECONDARY:         return RHICommandBufferLevelType::RHI_CommandBufferLevel_Secondary;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkCommandBufferLevel: Wrong VkCommandBufferLevel type !")
        }
        return RHICommandBufferLevelType::RHI_CommandBufferLevel_Primary;
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

    RHIColorWriteBitsType RHIVulkanConverter::TransformFromVkColorComponentFlags(VkColorComponentFlags vkColorComponentFlags)
    {
        switch ((int32)vkColorComponentFlags)
        {
        case VK_COLOR_COMPONENT_R_BIT:              return RHIColorWriteBitsType::RHI_ColorWriteBits_Red;
        case VK_COLOR_COMPONENT_G_BIT:              return RHIColorWriteBitsType::RHI_ColorWriteBits_Green;
        case VK_COLOR_COMPONENT_B_BIT:              return RHIColorWriteBitsType::RHI_ColorWriteBits_Blue;
        case VK_COLOR_COMPONENT_A_BIT:              return RHIColorWriteBitsType::RHI_ColorWriteBits_Alpha;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkColorComponentFlags: Wrong VkColorComponentFlags type !")
        }
        return RHIColorWriteBitsType::RHI_ColorWriteBits_Red;
    }

    RHIPipelineStageBitsType RHIVulkanConverter::TransformFromVkPipelineStageFlags(VkPipelineStageFlags vkPipelineStageFlags)
    {
        switch ((int32)vkPipelineStageFlags)
        {
        case VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT:                     return RHIPipelineStageBitsType::RHI_PipelineStageBits_TopOfPipe;
        case VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT:                   return RHIPipelineStageBitsType::RHI_PipelineStageBits_DrawIndirect;
        case VK_PIPELINE_STAGE_VERTEX_INPUT_BIT:                    return RHIPipelineStageBitsType::RHI_PipelineStageBits_VertexInput;
        case VK_PIPELINE_STAGE_VERTEX_SHADER_BIT:                   return RHIPipelineStageBitsType::RHI_PipelineStageBits_VertexShader;
        case VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT:     return RHIPipelineStageBitsType::RHI_PipelineStageBits_TessellationControlShader;
        case VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT:  return RHIPipelineStageBitsType::RHI_PipelineStageBits_TessellationEvaluationShader;
        case VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT:                 return RHIPipelineStageBitsType::RHI_PipelineStageBits_GeometryShader;
        case VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT:                 return RHIPipelineStageBitsType::RHI_PipelineStageBits_FragmentShader;
        case VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT:            return RHIPipelineStageBitsType::RHI_PipelineStageBits_EarlyFragmentTests;
        case VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT:             return RHIPipelineStageBitsType::RHI_PipelineStageBits_LateFragmentTests;
        case VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT:         return RHIPipelineStageBitsType::RHI_PipelineStageBits_ColorAttachmentOutput;
        case VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT:                  return RHIPipelineStageBitsType::RHI_PipelineStageBits_ComputeShader;
        case VK_PIPELINE_STAGE_TRANSFER_BIT:                        return RHIPipelineStageBitsType::RHI_PipelineStageBits_Transfer;
        case VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT:                  return RHIPipelineStageBitsType::RHI_PipelineStageBits_BottomOfPipe;
        case VK_PIPELINE_STAGE_HOST_BIT:                            return RHIPipelineStageBitsType::RHI_PipelineStageBits_Host;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkPipelineStageFlags: Wrong VkPipelineStageFlags type !")
        }
        return RHIPipelineStageBitsType::RHI_PipelineStageBits_BottomOfPipe;
    }

    RHIAccessBitsType RHIVulkanConverter::TransformFromVkAccessFlags(VkAccessFlags vkAccessFlags)
    {
        switch ((int32)vkAccessFlags)
        {
        case VK_ACCESS_INDIRECT_COMMAND_READ_BIT:                   return RHIAccessBitsType::RHI_AccessBits_IndirectCommandRead;
        case VK_ACCESS_INDEX_READ_BIT:                              return RHIAccessBitsType::RHI_AccessBits_IndexRead;
        case VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT:                   return RHIAccessBitsType::RHI_AccessBits_VertexAttributeRead;
        case VK_ACCESS_UNIFORM_READ_BIT:                            return RHIAccessBitsType::RHI_AccessBits_UniformRead;
        case VK_ACCESS_INPUT_ATTACHMENT_READ_BIT:                   return RHIAccessBitsType::RHI_AccessBits_InputAttachmentRead;
        case VK_ACCESS_SHADER_READ_BIT:                             return RHIAccessBitsType::RHI_AccessBits_ShaderRead;
        case VK_ACCESS_SHADER_WRITE_BIT:                            return RHIAccessBitsType::RHI_AccessBits_ShaderWrite;
        case VK_ACCESS_COLOR_ATTACHMENT_READ_BIT:                   return RHIAccessBitsType::RHI_AccessBits_ColorAttachmentRead;
        case VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT:                  return RHIAccessBitsType::RHI_AccessBits_ColorAttachmentWrite;
        case VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT:           return RHIAccessBitsType::RHI_AccessBits_DepthStencilAttachmentRead;
        case VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT:          return RHIAccessBitsType::RHI_AccessBits_DepthStencilAttachmentWrite;
        case VK_ACCESS_TRANSFER_READ_BIT:                           return RHIAccessBitsType::RHI_AccessBits_TransferRead;
        case VK_ACCESS_TRANSFER_WRITE_BIT:                          return RHIAccessBitsType::RHI_AccessBits_TransferWrite;
        case VK_ACCESS_HOST_READ_BIT:                               return RHIAccessBitsType::RHI_AccessBits_HostRead;
        case VK_ACCESS_HOST_WRITE_BIT:                              return RHIAccessBitsType::RHI_AccessBits_HostWrite;
        case VK_ACCESS_MEMORY_READ_BIT:                             return RHIAccessBitsType::RHI_AccessBits_MemoryRead;
        case VK_ACCESS_MEMORY_WRITE_BIT:                            return RHIAccessBitsType::RHI_AccessBits_MemoryWrite;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkAccessFlags: Wrong VkAccessFlags type !")
        }
        return RHIAccessBitsType::RHI_AccessBits_MemoryRead;
    }

    RHIDependencyBitsType RHIVulkanConverter::TransformFromVkDependencyFlags(VkDependencyFlags vkDependencyFlags)
    {
        switch ((int32)vkDependencyFlags)
        {
        case VK_DEPENDENCY_BY_REGION_BIT:                   return RHIDependencyBitsType::RHI_DependencyBits_ByRegion;
        case VK_DEPENDENCY_VIEW_LOCAL_BIT:                  return RHIDependencyBitsType::RHI_DependencyBits_ViewLocal;
        case VK_DEPENDENCY_DEVICE_GROUP_BIT:                return RHIDependencyBitsType::RHI_DependencyBits_DeviceGroup;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformFromVkDependencyFlags: Wrong VkDependencyFlags type !")
        }
        return RHIDependencyBitsType::RHI_DependencyBits_ByRegion;
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

    VkAttachmentLoadOp RHIVulkanConverter::TransformToVkAttachmentLoadOp(RHILoadOpType eLoadOp)
    {
        switch (eLoadOp)
        {
        case RHILoadOpType::RHI_LoadOp_Load:            return VK_ATTACHMENT_LOAD_OP_LOAD;
        case RHILoadOpType::RHI_LoadOp_Clear:           return VK_ATTACHMENT_LOAD_OP_CLEAR;
        case RHILoadOpType::RHI_LoadOp_DoNotCare:       return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkAttachmentLoadOp: Wrong RHILoadOpType type !")
        }
        return VK_ATTACHMENT_LOAD_OP_CLEAR;
    }

    VkAttachmentStoreOp RHIVulkanConverter::TransformToVkAttachmentStoreOp(RHIStoreOpType eStoreOp)
    {
        switch (eStoreOp)
        {
        case RHIStoreOpType::RHI_StoreOp_Store:            return VK_ATTACHMENT_STORE_OP_STORE;
        case RHIStoreOpType::RHI_StoreOp_DoNotCare:         return VK_ATTACHMENT_STORE_OP_DONT_CARE;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkAttachmentStoreOp: Wrong RHIStoreOpType type !")
        }
        return VK_ATTACHMENT_STORE_OP_DONT_CARE;
    }

    VkPipelineBindPoint RHIVulkanConverter::TransformToVkPipelineBindPoint(RHIPipelineBindPointType ePipelineBindPoint)
    {
        switch (ePipelineBindPoint)
        {
        case RHIPipelineBindPointType::RHI_PipelineBindPoint_Graphics:           return VK_PIPELINE_BIND_POINT_GRAPHICS;
        case RHIPipelineBindPointType::RHI_PipelineBindPoint_Compute:            return VK_PIPELINE_BIND_POINT_COMPUTE;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkPipelineBindPoint: Wrong RHIPipelineBindPointType type !")
        }
        return VK_PIPELINE_BIND_POINT_GRAPHICS;
    }

    VkCommandBufferLevel RHIVulkanConverter::TransformToVkCommandBufferLevel(RHICommandBufferLevelType eCommandBufferLevel)
    {
        switch (eCommandBufferLevel)
        {
        case RHICommandBufferLevelType::RHI_CommandBufferLevel_Primary:           return VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        case RHICommandBufferLevelType::RHI_CommandBufferLevel_Secondary:         return VK_COMMAND_BUFFER_LEVEL_SECONDARY;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkCommandBufferLevel: Wrong RHICommandBufferLevelType type !")
        }
        return VK_COMMAND_BUFFER_LEVEL_PRIMARY;
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
    VkShaderStageFlagBits RHIVulkanConverter::TransformToVkShaderStageFlagBits(RHIShaderStageBitsType eShaderStageBits)
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
            F_Assert(false && "RHIVulkanConverter::TransformToVkShaderStageFlagBits: Wrong RHIShaderStageBitsType type !")
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

    VkColorComponentFlags RHIVulkanConverter::TransformToVkColorComponentFlags(RHIColorWriteBitsType eColorWriteBits)
    {
        switch (eColorWriteBits)
        {
        case RHIColorWriteBitsType::RHI_ColorWriteBits_Red:            return VK_COLOR_COMPONENT_R_BIT;
        case RHIColorWriteBitsType::RHI_ColorWriteBits_Green:          return VK_COLOR_COMPONENT_G_BIT;
        case RHIColorWriteBitsType::RHI_ColorWriteBits_Blue:           return VK_COLOR_COMPONENT_B_BIT;
        case RHIColorWriteBitsType::RHI_ColorWriteBits_Alpha:          return VK_COLOR_COMPONENT_A_BIT;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkColorComponentFlags: Wrong RHIColorWriteBitsType type !")
        }
        return VK_COLOR_COMPONENT_R_BIT;
    }
    VkColorComponentFlags RHIVulkanConverter::TransformToVkColorComponentFlagsFromColorWriteFlags(RHIColorWriteFlags flagsColorWrite)
    {
        static std::map<RHIColorWriteBitsType, VkColorComponentFlags> s_Rules = 
        {
            { RHIColorWriteBitsType::RHI_ColorWriteBits_Red,       VK_COLOR_COMPONENT_R_BIT },
            { RHIColorWriteBitsType::RHI_ColorWriteBits_Green,     VK_COLOR_COMPONENT_G_BIT },
            { RHIColorWriteBitsType::RHI_ColorWriteBits_Blue,      VK_COLOR_COMPONENT_B_BIT },
            { RHIColorWriteBitsType::RHI_ColorWriteBits_Alpha,     VK_COLOR_COMPONENT_A_BIT },
        };

        VkColorComponentFlags vkResult = {};
        for (std::map<RHIColorWriteBitsType, VkColorComponentFlags>::iterator it = s_Rules.begin();
             it != s_Rules.end(); ++it)
        {
            if (flagsColorWrite & it->first) 
            {
                vkResult |= it->second;
            }
        }
        return vkResult;
    }

    VkPipelineStageFlags RHIVulkanConverter::TransformToVkPipelineStageFlags(RHIPipelineStageBitsType ePipelineStageBits)
    {
        switch (ePipelineStageBits)
        {
        case RHIPipelineStageBitsType::RHI_PipelineStageBits_TopOfPipe:                     return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        case RHIPipelineStageBitsType::RHI_PipelineStageBits_DrawIndirect:                  return VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
        case RHIPipelineStageBitsType::RHI_PipelineStageBits_VertexInput:                   return VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
        case RHIPipelineStageBitsType::RHI_PipelineStageBits_VertexShader:                  return VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
        case RHIPipelineStageBitsType::RHI_PipelineStageBits_TessellationControlShader:     return VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;
        case RHIPipelineStageBitsType::RHI_PipelineStageBits_TessellationEvaluationShader:  return VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
        case RHIPipelineStageBitsType::RHI_PipelineStageBits_GeometryShader:                return VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
        case RHIPipelineStageBitsType::RHI_PipelineStageBits_FragmentShader:                return VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        case RHIPipelineStageBitsType::RHI_PipelineStageBits_EarlyFragmentTests:            return VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        case RHIPipelineStageBitsType::RHI_PipelineStageBits_LateFragmentTests:             return VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        case RHIPipelineStageBitsType::RHI_PipelineStageBits_ColorAttachmentOutput:         return VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        case RHIPipelineStageBitsType::RHI_PipelineStageBits_ComputeShader:                 return VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
        case RHIPipelineStageBitsType::RHI_PipelineStageBits_Transfer:                      return VK_PIPELINE_STAGE_TRANSFER_BIT;
        case RHIPipelineStageBitsType::RHI_PipelineStageBits_BottomOfPipe:                  return VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        case RHIPipelineStageBitsType::RHI_PipelineStageBits_Host:                          return VK_PIPELINE_STAGE_HOST_BIT;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkPipelineStageFlags: Wrong RHIPipelineStageBitsType type !")
        }
        return VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    }
    VkPipelineStageFlags RHIVulkanConverter::TransformToVkPipelineStageFlagsFromPipelineStageFlags(RHIPipelineStageFlags flagsPipelineStage)
    {
        static std::map<RHIPipelineStageBitsType, VkPipelineStageFlags> s_Rules = 
        {
            { RHIPipelineStageBitsType::RHI_PipelineStageBits_TopOfPipe,                    VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT },
            { RHIPipelineStageBitsType::RHI_PipelineStageBits_DrawIndirect,                 VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT },
            { RHIPipelineStageBitsType::RHI_PipelineStageBits_VertexInput,                  VK_PIPELINE_STAGE_VERTEX_INPUT_BIT },
            { RHIPipelineStageBitsType::RHI_PipelineStageBits_VertexShader,                 VK_PIPELINE_STAGE_VERTEX_SHADER_BIT },
            { RHIPipelineStageBitsType::RHI_PipelineStageBits_TessellationControlShader,    VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT },
            { RHIPipelineStageBitsType::RHI_PipelineStageBits_TessellationEvaluationShader, VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT },
            { RHIPipelineStageBitsType::RHI_PipelineStageBits_GeometryShader,               VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT },
            { RHIPipelineStageBitsType::RHI_PipelineStageBits_FragmentShader,               VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT },
            { RHIPipelineStageBitsType::RHI_PipelineStageBits_EarlyFragmentTests,           VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT },
            { RHIPipelineStageBitsType::RHI_PipelineStageBits_LateFragmentTests,            VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT },
            { RHIPipelineStageBitsType::RHI_PipelineStageBits_ColorAttachmentOutput,        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT },
            { RHIPipelineStageBitsType::RHI_PipelineStageBits_ComputeShader,                VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT },
            { RHIPipelineStageBitsType::RHI_PipelineStageBits_Transfer,                     VK_PIPELINE_STAGE_TRANSFER_BIT },
            { RHIPipelineStageBitsType::RHI_PipelineStageBits_BottomOfPipe,                 VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT },
            { RHIPipelineStageBitsType::RHI_PipelineStageBits_Host,                         VK_PIPELINE_STAGE_HOST_BIT },
        };

        VkPipelineStageFlags vkResult = {};
        for (std::map<RHIPipelineStageBitsType, VkPipelineStageFlags>::iterator it = s_Rules.begin();
             it != s_Rules.end(); ++it)
        {
            if (flagsPipelineStage & it->first) 
            {
                vkResult |= it->second;
            }
        }
        return vkResult;
    }

    VkAccessFlags RHIVulkanConverter::TransformToVkAccessFlags(RHIAccessBitsType eAccessBits)
    {
        switch (eAccessBits)
        {
        case RHIAccessBitsType::RHI_AccessBits_IndirectCommandRead:                 return VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
        case RHIAccessBitsType::RHI_AccessBits_IndexRead:                           return VK_ACCESS_INDEX_READ_BIT;
        case RHIAccessBitsType::RHI_AccessBits_VertexAttributeRead:                 return VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
        case RHIAccessBitsType::RHI_AccessBits_UniformRead:                         return VK_ACCESS_UNIFORM_READ_BIT;
        case RHIAccessBitsType::RHI_AccessBits_InputAttachmentRead:                 return VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
        case RHIAccessBitsType::RHI_AccessBits_ShaderRead:                          return VK_ACCESS_SHADER_READ_BIT;
        case RHIAccessBitsType::RHI_AccessBits_ShaderWrite:                         return VK_ACCESS_SHADER_WRITE_BIT;
        case RHIAccessBitsType::RHI_AccessBits_ColorAttachmentRead:                 return VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
        case RHIAccessBitsType::RHI_AccessBits_ColorAttachmentWrite:                return VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        case RHIAccessBitsType::RHI_AccessBits_DepthStencilAttachmentRead:          return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
        case RHIAccessBitsType::RHI_AccessBits_DepthStencilAttachmentWrite:         return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        case RHIAccessBitsType::RHI_AccessBits_TransferRead:                        return VK_ACCESS_TRANSFER_READ_BIT;
        case RHIAccessBitsType::RHI_AccessBits_TransferWrite:                       return VK_ACCESS_TRANSFER_WRITE_BIT;
        case RHIAccessBitsType::RHI_AccessBits_HostRead:                            return VK_ACCESS_HOST_READ_BIT;
        case RHIAccessBitsType::RHI_AccessBits_HostWrite:                           return VK_ACCESS_HOST_WRITE_BIT;
        case RHIAccessBitsType::RHI_AccessBits_MemoryRead:                          return VK_ACCESS_MEMORY_READ_BIT;
        case RHIAccessBitsType::RHI_AccessBits_MemoryWrite:                         return VK_ACCESS_MEMORY_WRITE_BIT;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkAccessFlags: Wrong RHIAccessBitsType type !")
        }
        return VK_ACCESS_MEMORY_READ_BIT;
    }
    VkAccessFlags RHIVulkanConverter::TransformToVkAccessFlagsFromAccessFlags(RHIAccessFlags flagsAccess)
    {
        static std::map<RHIAccessBitsType, VkAccessFlags> s_Rules = 
        {
            { RHIAccessBitsType::RHI_AccessBits_IndirectCommandRead,            VK_ACCESS_INDIRECT_COMMAND_READ_BIT },
            { RHIAccessBitsType::RHI_AccessBits_IndexRead,                      VK_ACCESS_INDEX_READ_BIT },
            { RHIAccessBitsType::RHI_AccessBits_VertexAttributeRead,            VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT },
            { RHIAccessBitsType::RHI_AccessBits_UniformRead,                    VK_ACCESS_UNIFORM_READ_BIT },
            { RHIAccessBitsType::RHI_AccessBits_InputAttachmentRead,            VK_ACCESS_INPUT_ATTACHMENT_READ_BIT },
            { RHIAccessBitsType::RHI_AccessBits_ShaderRead,                     VK_ACCESS_SHADER_READ_BIT },
            { RHIAccessBitsType::RHI_AccessBits_ShaderWrite,                    VK_ACCESS_SHADER_WRITE_BIT },
            { RHIAccessBitsType::RHI_AccessBits_ColorAttachmentRead,            VK_ACCESS_COLOR_ATTACHMENT_READ_BIT },
            { RHIAccessBitsType::RHI_AccessBits_ColorAttachmentWrite,           VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT },
            { RHIAccessBitsType::RHI_AccessBits_DepthStencilAttachmentRead,     VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT },
            { RHIAccessBitsType::RHI_AccessBits_DepthStencilAttachmentWrite,    VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT },
            { RHIAccessBitsType::RHI_AccessBits_TransferRead,                   VK_ACCESS_TRANSFER_READ_BIT },
            { RHIAccessBitsType::RHI_AccessBits_TransferWrite,                  VK_ACCESS_TRANSFER_WRITE_BIT },
            { RHIAccessBitsType::RHI_AccessBits_HostRead,                       VK_ACCESS_HOST_READ_BIT },
            { RHIAccessBitsType::RHI_AccessBits_HostWrite,                      VK_ACCESS_HOST_WRITE_BIT },
            { RHIAccessBitsType::RHI_AccessBits_MemoryRead,                     VK_ACCESS_MEMORY_READ_BIT },
            { RHIAccessBitsType::RHI_AccessBits_MemoryWrite,                    VK_ACCESS_MEMORY_WRITE_BIT },
        };

        VkAccessFlags vkResult = {};
        for (std::map<RHIAccessBitsType, VkAccessFlags>::iterator it = s_Rules.begin();
             it != s_Rules.end(); ++it)
        {
            if (flagsAccess & it->first) 
            {
                vkResult |= it->second;
            }
        }
        return vkResult;
    }

    VkDependencyFlags RHIVulkanConverter::TransformToVkDependencyFlags(RHIDependencyBitsType eDependencyBits)
    {
        switch (eDependencyBits)
        {
        case RHIDependencyBitsType::RHI_DependencyBits_ByRegion:                   return VK_DEPENDENCY_BY_REGION_BIT;
        case RHIDependencyBitsType::RHI_DependencyBits_ViewLocal:                  return VK_DEPENDENCY_VIEW_LOCAL_BIT;
        case RHIDependencyBitsType::RHI_DependencyBits_DeviceGroup:                return VK_DEPENDENCY_DEVICE_GROUP_BIT;
        default:
            F_Assert(false && "RHIVulkanConverter::TransformToVkDependencyFlags: Wrong RHIDependencyBitsType type !")
        }
        return VK_DEPENDENCY_BY_REGION_BIT;
    }
    VkDependencyFlags RHIVulkanConverter::TransformToVkDependencyFlagsFromPipelineStageFlags(RHIDependencyFlags flagsDependency)
    {   
        static std::map<RHIDependencyBitsType, VkDependencyFlags> s_Rules = 
        {
            { RHIDependencyBitsType::RHI_DependencyBits_ByRegion,         VK_DEPENDENCY_BY_REGION_BIT },
            { RHIDependencyBitsType::RHI_DependencyBits_ViewLocal,        VK_DEPENDENCY_VIEW_LOCAL_BIT },
            { RHIDependencyBitsType::RHI_DependencyBits_DeviceGroup,      VK_DEPENDENCY_DEVICE_GROUP_BIT },
        };

        VkDependencyFlags vkResult = {};
        for (std::map<RHIDependencyBitsType, VkDependencyFlags>::iterator it = s_Rules.begin();
             it != s_Rules.end(); ++it)
        {
            if (flagsDependency & it->first) 
            {
                vkResult |= it->second;
            }
        }
        return vkResult;
    }


    //0> VkPipelineShaderStageCreateInfo
    VkPipelineShaderStageCreateInfo RHIVulkanConverter::TransformToVkPipelineShaderStageCreateInfo(VkShaderStageFlagBits stage,
                                                                                                   VkShaderModule module,
                                                                                                   const char* pName,
                                                                                                   const VkSpecializationInfo* pSpecializationInfo)
    {
        VkPipelineShaderStageCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        createInfo.stage = stage;
        createInfo.module = module;
        createInfo.pName = pName;
        createInfo.pSpecializationInfo = pSpecializationInfo;

        return createInfo;
    }
    VkPipelineShaderStageCreateInfo RHIVulkanConverter::TransformToVkPipelineShaderStageCreateInfo(RHIVulkanShaderModule* pShader, 
                                                                                                   RHIShaderStageBitsType eShaderStageBits)
    {
        return TransformToVkPipelineShaderStageCreateInfo(TransformToVkShaderStageFlagBits(eShaderStageBits),
                                                          pShader->GetVkShaderModule(),
                                                          pShader->GetNameMain().c_str(),
                                                          nullptr);
    }
    void RHIVulkanConverter::TransformToVkPipelineShaderStageCreateInfo(RHIVulkanShaderModule* pShader, 
                                                                        RHIShaderStageBitsType eShaderStageBits,
                                                                        VkPipelineShaderStageCreateInfo& createInfo)
    {
        createInfo = TransformToVkPipelineShaderStageCreateInfo(pShader, eShaderStageBits);
    }
    void RHIVulkanConverter::TransformToVkPipelineShaderStageCreateInfoVector(VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos,
                                                                              RHIVulkanShaderModule* pShaderVertex, 
                                                                              RHIVulkanShaderModule* pShaderPixel, 
                                                                              RHIVulkanShaderModule* pShaderGeometry, 
                                                                              RHIVulkanShaderModule* pShaderDomain, 
                                                                              RHIVulkanShaderModule* pShaderHull)
    {
        F_Assert(pShaderVertex && pShaderPixel && "RHIVulkanConverter::TransformToVkPipelineShaderStageCreateInfoVector")
        VkPipelineShaderStageCreateInfo createInfo_Vertex = RHIVulkanConverter::TransformToVkPipelineShaderStageCreateInfo(pShaderVertex, RHIShaderStageBitsType::RHI_ShaderStageBits_Vertex);
        aShaderStageCreateInfos.push_back(createInfo_Vertex);
        VkPipelineShaderStageCreateInfo createInfo_Pixel = RHIVulkanConverter::TransformToVkPipelineShaderStageCreateInfo(pShaderPixel, RHIShaderStageBitsType::RHI_ShaderStageBits_Pixel);
        aShaderStageCreateInfos.push_back(createInfo_Pixel);
        if (pShaderGeometry != nullptr)
        {
            VkPipelineShaderStageCreateInfo createInfo_Geometry = RHIVulkanConverter::TransformToVkPipelineShaderStageCreateInfo(pShaderGeometry, RHIShaderStageBitsType::RHI_ShaderStageBits_Geometry);
            aShaderStageCreateInfos.push_back(createInfo_Geometry);
        }
        if (pShaderDomain != nullptr)
        {
            VkPipelineShaderStageCreateInfo createInfo_Domain = RHIVulkanConverter::TransformToVkPipelineShaderStageCreateInfo(pShaderDomain, RHIShaderStageBitsType::RHI_ShaderStageBits_Domain);
            aShaderStageCreateInfos.push_back(createInfo_Domain);
        }
        if (pShaderHull != nullptr)
        {
            VkPipelineShaderStageCreateInfo createInfo_Hull = RHIVulkanConverter::TransformToVkPipelineShaderStageCreateInfo(pShaderHull, RHIShaderStageBitsType::RHI_ShaderStageBits_Hull);
            aShaderStageCreateInfos.push_back(createInfo_Hull);
        }
    }

    //1> VkPipelineVertexInputStateCreateInfo
    VkPipelineVertexInputStateCreateInfo RHIVulkanConverter::TransformToVkPipelineVertexInputStateCreateInfo(VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                                                                                             VkVertexInputAttributeDescriptionVector* pAttributeDescriptions)
    {
        VkPipelineVertexInputStateCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        if (pBindingDescriptions != nullptr)
        {   
            createInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(pBindingDescriptions->size());
            createInfo.pVertexBindingDescriptions = pBindingDescriptions->data();
        }
        if (pAttributeDescriptions != nullptr)
        {
            createInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(pAttributeDescriptions->size());
            createInfo.pVertexAttributeDescriptions = pAttributeDescriptions->data();
        }

        return createInfo;
    }                                                 
    void RHIVulkanConverter::TransformToVkPipelineVertexInputStateCreateInfo(VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                                                             VkVertexInputAttributeDescriptionVector* pAttributeDescriptions, 
                                                                             VkPipelineVertexInputStateCreateInfo& createInfo)
    {
        createInfo = RHIVulkanConverter::TransformToVkPipelineVertexInputStateCreateInfo(pBindingDescriptions, pAttributeDescriptions);
    }

    //2> VkPipelineInputAssemblyStateCreateInfo
    VkPipelineInputAssemblyStateCreateInfo RHIVulkanConverter::TransformToVkPipelineInputAssemblyStateCreateInfo(VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable)
    {
        VkPipelineInputAssemblyStateCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        createInfo.topology = topology;
        createInfo.primitiveRestartEnable = primitiveRestartEnable;
        
        return createInfo;
    }
    VkPipelineInputAssemblyStateCreateInfo RHIVulkanConverter::TransformToVkPipelineInputAssemblyStateCreateInfo(RHIPrimitiveTopologyType ePrimitiveTopology, bool bPrimitiveRestartEnable)
    {
        return TransformToVkPipelineInputAssemblyStateCreateInfo(TransformToVkPrimitiveTopology(ePrimitiveTopology), bPrimitiveRestartEnable ? VK_TRUE : VK_FALSE);
    }
    void RHIVulkanConverter::TransformToVkPipelineInputAssemblyStateCreateInfo(RHIPrimitiveTopologyType ePrimitiveTopology, bool bPrimitiveRestartEnable, VkPipelineInputAssemblyStateCreateInfo& createInfo)
    {
        createInfo = TransformToVkPipelineInputAssemblyStateCreateInfo(ePrimitiveTopology, bPrimitiveRestartEnable);
    }

    //3> VkPipelineViewportStateCreateInfo
    VkPipelineViewportStateCreateInfo RHIVulkanConverter::TransformToVkPipelineViewportStateCreateInfo(const VkViewportVector& aViewports, const VkRect2DVector& aScissors)
    {
        VkPipelineViewportStateCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        createInfo.viewportCount = static_cast<uint32_t>(aViewports.size());
        createInfo.pViewports = aViewports.data();
        createInfo.scissorCount = static_cast<uint32_t>(aScissors.size());
        createInfo.pScissors = aScissors.data();
        return createInfo;
    }
    void RHIVulkanConverter::TransformToVkPipelineViewportStateCreateInfo(const VkViewportVector& aViewports, const VkRect2DVector& aScissors, VkPipelineViewportStateCreateInfo& createInfo)
    {
        createInfo = TransformToVkPipelineViewportStateCreateInfo(aViewports, aScissors);
    }

    //4> VkPipelineRasterizationStateCreateInfo
    VkPipelineRasterizationStateCreateInfo RHIVulkanConverter::TransformToVkPipelineRasterizationStateCreateInfo(VkBool32 depthClampEnable,
                                                                                                                 VkBool32 rasterizerDiscardEnable,
                                                                                                                 VkPolygonMode polygonMode,
                                                                                                                 VkCullModeFlags cullMode,
                                                                                                                 VkFrontFace frontFace,
                                                                                                                 VkBool32 depthBiasEnable,
                                                                                                                 float depthBiasConstantFactor,
                                                                                                                 float depthBiasClamp,
                                                                                                                 float depthBiasSlopeFactor,
                                                                                                                 float lineWidth)
    {
        VkPipelineRasterizationStateCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        createInfo.depthClampEnable = depthClampEnable;
        createInfo.rasterizerDiscardEnable = rasterizerDiscardEnable;
        createInfo.polygonMode = polygonMode;
        createInfo.cullMode = cullMode;
        createInfo.frontFace = frontFace;
        createInfo.depthBiasEnable = depthBiasEnable;
        createInfo.depthBiasConstantFactor = depthBiasConstantFactor;
        createInfo.depthBiasClamp = depthBiasClamp;
        createInfo.depthBiasSlopeFactor = depthBiasSlopeFactor;
        createInfo.lineWidth = lineWidth;

        return createInfo;
    }
    void RHIVulkanConverter::TransformToVkPipelineRasterizationStateCreateInfo(VkBool32 depthClampEnable,
                                                                               VkBool32 rasterizerDiscardEnable,
                                                                               VkPolygonMode polygonMode,
                                                                               VkCullModeFlags cullMode,
                                                                               VkFrontFace frontFace,
                                                                               VkBool32 depthBiasEnable,
                                                                               float depthBiasConstantFactor,
                                                                               float depthBiasClamp,
                                                                               float depthBiasSlopeFactor,
                                                                               float lineWidth,
                                                                               VkPipelineRasterizationStateCreateInfo& createInfo)
    {
        createInfo = TransformToVkPipelineRasterizationStateCreateInfo(depthClampEnable,
                                                                       rasterizerDiscardEnable,
                                                                       polygonMode,
                                                                       cullMode,
                                                                       frontFace,
                                                                       depthBiasEnable,
                                                                       depthBiasConstantFactor,
                                                                       depthBiasClamp,
                                                                       depthBiasSlopeFactor,
                                                                       lineWidth);
    }
    VkPipelineRasterizationStateCreateInfo RHIVulkanConverter::TransformToVkPipelineRasterizationStateCreateInfo(const RHIPrimitiveState& statePrimitive, 
                                                                                                                 const RHIDepthStencilState& stateDepthStencil)
    {
        return TransformToVkPipelineRasterizationStateCreateInfo(statePrimitive.bDepthClipEnable ? VK_TRUE : VK_FALSE,
                                                                 statePrimitive.bRasterizerDiscardEnable ? VK_TRUE : VK_FALSE,
                                                                 TransformToVkPolygonMode(statePrimitive.ePolygon),
                                                                 TransformToVkCullModeFlagBits(statePrimitive.eCull),
                                                                 TransformToVkFrontFace(statePrimitive.eFrontFace),
                                                                 stateDepthStencil.nDepthBias == 0 ? VK_FALSE : VK_TRUE,
                                                                 (float)stateDepthStencil.nDepthBias,
                                                                 stateDepthStencil.fDepthBiasClamp,
                                                                 stateDepthStencil.fDepthBiasSlopeScale,
                                                                 statePrimitive.fLineWidth);
    }

    //5> VkPipelineMultisampleStateCreateInfo
    VkPipelineMultisampleStateCreateInfo RHIVulkanConverter::TransformToVkPipelineMultisampleStateCreateInfo(VkSampleCountFlagBits rasterizationSamples,
                                                                                                             VkBool32 alphaToCoverageEnable,
                                                                                                             VkSampleMask* pSampleMask)
    {
        VkPipelineMultisampleStateCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        createInfo.rasterizationSamples = rasterizationSamples;
        createInfo.sampleShadingEnable = VK_FALSE;
        createInfo.minSampleShading = 1.0f; // Optional
        createInfo.pSampleMask = pSampleMask; // Optional
        createInfo.alphaToCoverageEnable = alphaToCoverageEnable; // Optional
        createInfo.alphaToOneEnable = VK_FALSE; // Optional

        return createInfo;
    }
    void RHIVulkanConverter::TransformToVkPipelineMultisampleStateCreateInfo(VkSampleCountFlagBits rasterizationSamples,
                                                                             VkBool32 alphaToCoverageEnable,
                                                                             VkSampleMask* pSampleMask,
                                                                             VkPipelineMultisampleStateCreateInfo& createInfo)
    {
        createInfo = TransformToVkPipelineMultisampleStateCreateInfo(rasterizationSamples, 
                                                                     alphaToCoverageEnable,
                                                                     pSampleMask);
    }
    VkPipelineMultisampleStateCreateInfo RHIVulkanConverter::TransformToVkPipelineMultisampleStateCreateInfo(const RHIMultiSampleState& state)
    {
        VkSampleMask* pSampleMask = nullptr;
        if (state.nMask != 0xFFFFFFFF)
            pSampleMask = (VkSampleMask*)(&state.nMask);
        return TransformToVkPipelineMultisampleStateCreateInfo(TransformToVkSampleCountFlagBits(state.eSampleCount),
                                                               state.bAlphaToCoverage ? VK_TRUE : VK_FALSE,
                                                               pSampleMask);
    }

    //6> VkPipelineDepthStencilStateCreateInfo
    VkStencilOpState RHIVulkanConverter::TransformToVkStencilOpState(VkStencilOp failOp,
                                                                     VkStencilOp passOp,
                                                                     VkStencilOp depthFailOp,
                                                                     VkCompareOp compareOp,
                                                                     uint32_t compareMask,
                                                                     uint32_t writeMask,
                                                                     uint32_t reference)
    {
        VkStencilOpState state = {};
        state.failOp = failOp;
        state.passOp = passOp;
        state.depthFailOp = depthFailOp;
        state.compareOp = compareOp;
        state.compareMask = compareMask;
        state.writeMask = writeMask;
        state.reference = reference;

        return state;
    }
    VkStencilOpState RHIVulkanConverter::TransformToVkStencilOpState(const RHIStencilFaceState& stencilFace)
    {
        return TransformToVkStencilOpState(TransformToVkStencilOp(stencilFace.eFailOp),
                                           TransformToVkStencilOp(stencilFace.ePassOp),
                                           TransformToVkStencilOp(stencilFace.eDepthFailOp),
                                           TransformToVkCompareOp(stencilFace.eComparisonFunc),
                                           stencilFace.nCompareMask,
                                           stencilFace.nWriteMask,
                                           stencilFace.nReference);
    }
    VkPipelineDepthStencilStateCreateInfo RHIVulkanConverter::TransformToVkPipelineDepthStencilStateCreateInfo(VkBool32 depthTestEnable,
                                                                                                               VkBool32 depthWriteEnable,
                                                                                                               VkCompareOp depthCompareOp,
                                                                                                               VkBool32 depthBoundsTestEnable,
                                                                                                               VkBool32 stencilTestEnable,
                                                                                                               const VkStencilOpState& front,
                                                                                                               const VkStencilOpState& back,
                                                                                                               float minDepthBounds,
                                                                                                               float maxDepthBounds)
    {
        VkPipelineDepthStencilStateCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        createInfo.depthTestEnable = depthTestEnable;
        createInfo.depthWriteEnable = depthWriteEnable;
        createInfo.depthCompareOp = depthCompareOp;
        createInfo.depthBoundsTestEnable = depthBoundsTestEnable;
        createInfo.stencilTestEnable = stencilTestEnable;
        if (stencilTestEnable)
        {
            createInfo.front = front;
            createInfo.back = back;
        }
        createInfo.minDepthBounds = minDepthBounds;
        createInfo.maxDepthBounds = maxDepthBounds;

        return createInfo;
    }
    void RHIVulkanConverter::TransformToVkPipelineDepthStencilStateCreateInfo(VkBool32 depthTestEnable,
                                                                              VkBool32 depthWriteEnable,
                                                                              VkCompareOp depthCompareOp,
                                                                              VkBool32 depthBoundsTestEnable,
                                                                              VkBool32 stencilTestEnable,
                                                                              const VkStencilOpState& front,
                                                                              const VkStencilOpState& back,
                                                                              float minDepthBounds,
                                                                              float maxDepthBounds,
                                                                              VkPipelineDepthStencilStateCreateInfo& createInfo)
    {
        createInfo = TransformToVkPipelineDepthStencilStateCreateInfo(depthTestEnable,
                                                                      depthWriteEnable,
                                                                      depthCompareOp,
                                                                      depthBoundsTestEnable,
                                                                      stencilTestEnable,
                                                                      front,
                                                                      back,
                                                                      minDepthBounds,
                                                                      maxDepthBounds);
    }
    VkPipelineDepthStencilStateCreateInfo RHIVulkanConverter::TransformToVkPipelineDepthStencilStateCreateInfo(const RHIDepthStencilState& state)
    {   
        return TransformToVkPipelineDepthStencilStateCreateInfo(state.bDepthTestEnable ? VK_TRUE : VK_FALSE,
                                                                state.bDepthWriteEnable ? VK_TRUE : VK_FALSE,
                                                                TransformToVkCompareOp(state.eDepthComparisonFunc),
                                                                VK_FALSE,
                                                                state.bStencilEnable ? VK_TRUE : VK_FALSE,
                                                                TransformToVkStencilOpState(state.sStencilFront),
                                                                TransformToVkStencilOpState(state.sStencilBack),
                                                                0.0f,
                                                                1.0f);
    }

    //7> VkPipelineColorBlendAttachmentState/VkPipelineColorBlendStateCreateInfo
    VkPipelineColorBlendAttachmentState RHIVulkanConverter::TransformToVkPipelineColorBlendAttachmentState(VkBool32 blendEnable,
                                                                                                           VkBlendFactor srcColorBlendFactor,
                                                                                                           VkBlendFactor dstColorBlendFactor,
                                                                                                           VkBlendOp colorBlendOp,
                                                                                                           VkBlendFactor srcAlphaBlendFactor,
                                                                                                           VkBlendFactor dstAlphaBlendFactor,
                                                                                                           VkBlendOp alphaBlendOp,
                                                                                                           VkColorComponentFlags colorWriteMask)
    {
        VkPipelineColorBlendAttachmentState state = {};
        state.blendEnable = blendEnable;
        if (blendEnable)
        {
            state.srcColorBlendFactor = srcColorBlendFactor;
            state.dstColorBlendFactor = dstColorBlendFactor;
            state.colorBlendOp = colorBlendOp;
            state.srcAlphaBlendFactor = srcAlphaBlendFactor;
            state.dstAlphaBlendFactor = dstAlphaBlendFactor;
            state.alphaBlendOp = alphaBlendOp;
        }
        state.colorWriteMask = colorWriteMask;

        return state;
    }
    void RHIVulkanConverter::TransformToVkPipelineColorBlendAttachmentState(VkBool32 blendEnable,
                                                                            VkBlendFactor srcColorBlendFactor,
                                                                            VkBlendFactor dstColorBlendFactor,
                                                                            VkBlendOp colorBlendOp,
                                                                            VkBlendFactor srcAlphaBlendFactor,
                                                                            VkBlendFactor dstAlphaBlendFactor,
                                                                            VkBlendOp alphaBlendOp,
                                                                            VkColorComponentFlags colorWriteMask,
                                                                            VkPipelineColorBlendAttachmentState& createInfo) 
    {
        createInfo = TransformToVkPipelineColorBlendAttachmentState(blendEnable,
                                                                    srcColorBlendFactor,
                                                                    dstColorBlendFactor,
                                                                    colorBlendOp,
                                                                    srcAlphaBlendFactor,
                                                                    dstAlphaBlendFactor,
                                                                    alphaBlendOp,
                                                                    colorWriteMask);
    }
    VkPipelineColorBlendAttachmentState RHIVulkanConverter::TransformToVkPipelineColorBlendAttachmentState(const RHIColorTargetState& state)
    {
        VkBool32 blendEnable = state.bBlendEnable ? VK_TRUE : VK_FALSE;
        VkBlendFactor srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        VkBlendFactor dstColorBlendFactor =VK_BLEND_FACTOR_ZERO;
        VkBlendOp colorBlendOp = VK_BLEND_OP_ADD;
        VkBlendFactor srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        VkBlendFactor dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        VkBlendOp alphaBlendOp = VK_BLEND_OP_ADD;
        if (blendEnable)
        {
            srcColorBlendFactor = TransformToVkBlendFactor(state.sBlendState.sBlendColor.eBlendFactorSrc);
            dstColorBlendFactor = TransformToVkBlendFactor(state.sBlendState.sBlendColor.eBlendFactorDst);
            colorBlendOp = TransformToVkBlendOp(state.sBlendState.sBlendColor.eBlendOp);
            srcAlphaBlendFactor = TransformToVkBlendFactor(state.sBlendState.sBlendAlpha.eBlendFactorSrc);
            dstAlphaBlendFactor = TransformToVkBlendFactor(state.sBlendState.sBlendAlpha.eBlendFactorDst);
            alphaBlendOp = TransformToVkBlendOp(state.sBlendState.sBlendAlpha.eBlendOp);
        }
        VkColorComponentFlags colorWriteMask = TransformToVkColorComponentFlagsFromColorWriteFlags(state.flagsWrite);

        return TransformToVkPipelineColorBlendAttachmentState(blendEnable,
                                                              srcColorBlendFactor,
                                                              dstColorBlendFactor,
                                                              colorBlendOp,
                                                              srcAlphaBlendFactor,
                                                              dstAlphaBlendFactor,
                                                              alphaBlendOp,
                                                              colorWriteMask);
    }

    VkPipelineColorBlendStateCreateInfo RHIVulkanConverter::TransformToVkPipelineColorBlendStateCreateInfo(const VkPipelineColorBlendAttachmentStateVector& aState)
    {
        VkPipelineColorBlendStateCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        createInfo.logicOpEnable = VK_FALSE;
        createInfo.logicOp = VK_LOGIC_OP_COPY;
        createInfo.attachmentCount = (uint32_t)aState.size();
        createInfo.pAttachments = aState.data();
        createInfo.blendConstants[0] = 0.0f;
        createInfo.blendConstants[1] = 0.0f;
        createInfo.blendConstants[2] = 0.0f;
        createInfo.blendConstants[3] = 0.0f;

        return createInfo;
    }
    void RHIVulkanConverter::TransformToVkPipelineColorBlendStateCreateInfo(const VkPipelineColorBlendAttachmentStateVector& aState,
                                                                            VkPipelineColorBlendStateCreateInfo& createInfo)
    {
        createInfo = TransformToVkPipelineColorBlendStateCreateInfo(aState);
    }
    VkPipelineColorBlendStateCreateInfo RHIVulkanConverter::TransformToVkPipelineColorBlendStateCreateInfo(const RHIColorTargetStateVector& aColorTargetState)
    {
        VkPipelineColorBlendAttachmentStateVector aState;
        size_t count = aColorTargetState.size();
        for (size_t i = 0; i < count; i++)
        {
            VkPipelineColorBlendAttachmentState createInfo = TransformToVkPipelineColorBlendAttachmentState(aColorTargetState[i]);
            aState.push_back(createInfo);
        }
        return TransformToVkPipelineColorBlendStateCreateInfo(aState);
    }

    //8> VkPipelineDynamicStateCreateInfo
    VkPipelineDynamicStateCreateInfo RHIVulkanConverter::TransformToVkPipelineDynamicStateCreateInfo(const VkDynamicStateVector& aState)
    {
        VkPipelineDynamicStateCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        createInfo.dynamicStateCount = static_cast<uint32_t>(aState.size());
        createInfo.pDynamicStates = aState.data();
        createInfo.flags = 0;

        return createInfo;
    }
    void RHIVulkanConverter::TransformToVkPipelineDynamicStateCreateInfo(const VkDynamicStateVector& aState,
                                                                         VkPipelineDynamicStateCreateInfo& createInfo)
    {
        createInfo = TransformToVkPipelineDynamicStateCreateInfo(aState);
    }

    //9> VkPipelineTessellationStateCreateInfo
    VkPipelineTessellationStateCreateInfo RHIVulkanConverter::TransformToVkPipelineTessellationStateCreateInfo(VkPipelineTessellationStateCreateFlags flags,
                                                                                                               uint32_t patchControlPoints)
    {
        VkPipelineTessellationStateCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
        createInfo.flags = flags;
        createInfo.patchControlPoints = patchControlPoints;

        return createInfo;
    }
    void RHIVulkanConverter::TransformToVkPipelineTessellationStateCreateInfo(VkPipelineTessellationStateCreateFlags flags,
                                                                              uint32_t patchControlPoints,
                                                                              VkPipelineTessellationStateCreateInfo& createInfo)
    {
        createInfo = TransformToVkPipelineTessellationStateCreateInfo(flags, patchControlPoints);
    }

    //10> VkGraphicsPipelineCreateInfo
    VkGraphicsPipelineCreateInfo RHIVulkanConverter::TransformToVkGraphicsPipelineCreateInfo(VkPipelineCreateFlags flags,
                                                                                             uint32_t stageCount,
                                                                                             const VkPipelineShaderStageCreateInfo* pStages,
                                                                                             const VkPipelineVertexInputStateCreateInfo* pVertexInputState,
                                                                                             const VkPipelineInputAssemblyStateCreateInfo* pInputAssemblyState,
                                                                                             const VkPipelineTessellationStateCreateInfo* pTessellationState,
                                                                                             const VkPipelineViewportStateCreateInfo* pViewportState,
                                                                                             const VkPipelineRasterizationStateCreateInfo* pRasterizationState,
                                                                                             const VkPipelineMultisampleStateCreateInfo* pMultisampleState,
                                                                                             const VkPipelineDepthStencilStateCreateInfo* pDepthStencilState,
                                                                                             const VkPipelineColorBlendStateCreateInfo* pColorBlendState,
                                                                                             const VkPipelineDynamicStateCreateInfo* pDynamicState,
                                                                                             VkPipelineLayout layout,
                                                                                             VkRenderPass renderPass,
                                                                                             uint32_t subpass,
                                                                                             VkPipeline basePipelineHandle,
                                                                                             int32_t basePipelineIndex)
    {
        VkGraphicsPipelineCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        createInfo.flags = flags;
        createInfo.stageCount = stageCount;
        createInfo.pStages = pStages;
        createInfo.pVertexInputState = pVertexInputState;
        createInfo.pInputAssemblyState = pInputAssemblyState;
        createInfo.pTessellationState = pTessellationState;
        createInfo.pViewportState = pViewportState;
        createInfo.pRasterizationState = pRasterizationState;
        createInfo.pMultisampleState = pMultisampleState;
        createInfo.pDepthStencilState = pDepthStencilState;
        createInfo.pColorBlendState = pColorBlendState;
        createInfo.pDynamicState = pDynamicState;
        createInfo.layout = layout;
        createInfo.renderPass = renderPass;
        createInfo.subpass = subpass;
        createInfo.basePipelineHandle = basePipelineHandle;
        createInfo.basePipelineIndex = basePipelineIndex;

        return createInfo;
    }
    void RHIVulkanConverter::TransformToVkGraphicsPipelineCreateInfo(VkPipelineCreateFlags flags,
                                                                     uint32_t stageCount,
                                                                     const VkPipelineShaderStageCreateInfo* pStages,
                                                                     const VkPipelineVertexInputStateCreateInfo* pVertexInputState,
                                                                     const VkPipelineInputAssemblyStateCreateInfo* pInputAssemblyState,
                                                                     const VkPipelineTessellationStateCreateInfo* pTessellationState,
                                                                     const VkPipelineViewportStateCreateInfo* pViewportState,
                                                                     const VkPipelineRasterizationStateCreateInfo* pRasterizationState,
                                                                     const VkPipelineMultisampleStateCreateInfo* pMultisampleState,
                                                                     const VkPipelineDepthStencilStateCreateInfo* pDepthStencilState,
                                                                     const VkPipelineColorBlendStateCreateInfo* pColorBlendState,
                                                                     const VkPipelineDynamicStateCreateInfo* pDynamicState,
                                                                     VkPipelineLayout layout,
                                                                     VkRenderPass renderPass,
                                                                     uint32_t subpass,
                                                                     VkPipeline basePipelineHandle,
                                                                     int32_t basePipelineIndex,
                                                                     VkGraphicsPipelineCreateInfo& createInfo)
    {
        createInfo = TransformToVkGraphicsPipelineCreateInfo(flags,
                                                             stageCount,
                                                             pStages,
                                                             pVertexInputState,
                                                             pInputAssemblyState,
                                                             pTessellationState,
                                                             pViewportState,
                                                             pRasterizationState,
                                                             pMultisampleState,
                                                             pDepthStencilState,
                                                             pColorBlendState,
                                                             pDynamicState,
                                                             layout,
                                                             renderPass,
                                                             subpass,
                                                             basePipelineHandle,
                                                             basePipelineIndex);
    }

    //11> VkComputePipelineCreateInfo
    VkComputePipelineCreateInfo RHIVulkanConverter::TransformToVkComputePipelineCreateInfo(const VkPipelineShaderStageCreateInfo& shaderStageCreateInfo,
                                                                                           VkPipelineLayout vkPipelineLayout, 
                                                                                           VkPipelineCreateFlags flags)
    {
        VkComputePipelineCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
        createInfo.flags = flags;
        createInfo.stage = shaderStageCreateInfo;
        createInfo.layout = vkPipelineLayout;

        return createInfo;
    }
    void RHIVulkanConverter::TransformToVkComputePipelineCreateInfo(const VkPipelineShaderStageCreateInfo& shaderStageCreateInfo,
                                                                    VkPipelineLayout vkPipelineLayout, 
                                                                    VkPipelineCreateFlags flags,
                                                                    VkComputePipelineCreateInfo& createInfo)
    {
        createInfo = TransformToVkComputePipelineCreateInfo(shaderStageCreateInfo,
                                                            vkPipelineLayout,
                                                            flags);
    }

    //12> VkRenderPassCreateInfo
    VkRenderPassCreateInfo RHIVulkanConverter::TransformToVkRenderPassCreateInfo(const VkAttachmentDescriptionVector& aAttachmentDescription,
                                                                                 const VkSubpassDescriptionVector& aSubpassDescription,
                                                                                 const VkSubpassDependencyVector& aSubpassDependency,
                                                                                 VkRenderPassMultiviewCreateInfo* pMultiviewCI)
    {
        VkRenderPassCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        createInfo.attachmentCount = static_cast<uint32_t>(aAttachmentDescription.size());
        createInfo.pAttachments = aAttachmentDescription.data();
        createInfo.subpassCount = static_cast<uint32_t>(aSubpassDescription.size());
        createInfo.pSubpasses = aSubpassDescription.data();
        createInfo.dependencyCount = static_cast<uint32_t>(aSubpassDependency.size());
        createInfo.pDependencies = aSubpassDependency.data();
        if (pMultiviewCI != nullptr)
        {
            createInfo.pNext = pMultiviewCI;
        }

        return createInfo;
    }
    void RHIVulkanConverter::TransformToVkRenderPassCreateInfo(const VkAttachmentDescriptionVector& aAttachmentDescription,
                                                               const VkSubpassDescriptionVector& aSubpassDescription,
                                                               const VkSubpassDependencyVector& aSubpassDependency,
                                                               VkRenderPassMultiviewCreateInfo* pMultiviewCI,
                                                               VkRenderPassCreateInfo& createInfo)
    {
        createInfo = TransformToVkRenderPassCreateInfo(aAttachmentDescription,
                                                       aSubpassDescription,
                                                       aSubpassDependency,
                                                       pMultiviewCI);
    }
    VkAttachmentDescription RHIVulkanConverter::TransformToVkAttachmentDescription(VkAttachmentDescriptionFlags flags,
                                                                                   VkFormat typeFormat,
                                                                                   VkSampleCountFlagBits samples,
                                                                                   VkAttachmentLoadOp loadOp,
                                                                                   VkAttachmentStoreOp storeOp,
                                                                                   VkAttachmentLoadOp stencilLoadOp,
                                                                                   VkAttachmentStoreOp stencilStoreOp,
                                                                                   VkImageLayout initialLayout,
                                                                                   VkImageLayout finalLayout)
    {
        VkAttachmentDescription desci = {};
        desci.format = typeFormat;
        desci.samples = samples;
        desci.loadOp = loadOp;
        desci.storeOp = storeOp;
        desci.stencilLoadOp = stencilLoadOp;
        desci.stencilStoreOp = stencilStoreOp;
        desci.initialLayout = initialLayout;
        desci.finalLayout = finalLayout;

        return desci;
    }
    VkAttachmentDescription RHIVulkanConverter::TransformToVkAttachmentDescriptionFromColorAttachment(const RHIGraphicsPassColorAttachment& colorAttachment)
    {
        F_Assert(colorAttachment.pView && "HIVulkanConverter::TransformToVkAttachmentDescriptionFromColorAttachment")
        RHIVulkanTextureView* pVulkanTextureView = (RHIVulkanTextureView*)colorAttachment.pView;
        RHIVulkanTexture* pVulkanTexture = pVulkanTextureView->GetVulkanTexture();
        return TransformToVkAttachmentDescription(0,
                                                  pVulkanTexture->GetVkFormat(),
                                                  pVulkanTexture->GetVkSampleCountFlagBits(),
                                                  TransformToVkAttachmentLoadOp(colorAttachment.eLoadOp),
                                                  TransformToVkAttachmentStoreOp(colorAttachment.eStoreOp),
                                                  VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                  VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                  TransformToVkImageLayout(colorAttachment.eStateInitial),
                                                  TransformToVkImageLayout(colorAttachment.eStateFinal));
    }
    VkAttachmentDescription RHIVulkanConverter::TransformToVkAttachmentDescriptionFromDepthStencilAttachment(const RHIGraphicsPassDepthStencilAttachment& depthStencilAttachment)
    {
        F_Assert(depthStencilAttachment.pView && "HIVulkanConverter::TransformToVkAttachmentDescriptionFromDepthStencilAttachment")
        RHIVulkanTextureView* pVulkanTextureView = (RHIVulkanTextureView*)depthStencilAttachment.pView;
        RHIVulkanTexture* pVulkanTexture = pVulkanTextureView->GetVulkanTexture();
        return TransformToVkAttachmentDescription(0,
                                                  pVulkanTexture->GetVkFormat(),
                                                  pVulkanTexture->GetVkSampleCountFlagBits(),
                                                  TransformToVkAttachmentLoadOp(depthStencilAttachment.eDepthLoadOp),
                                                  TransformToVkAttachmentStoreOp(depthStencilAttachment.eDepthStoreOp),
                                                  TransformToVkAttachmentLoadOp(depthStencilAttachment.eStencilLoadOp),
                                                  TransformToVkAttachmentStoreOp(depthStencilAttachment.eStencilStoreOp),
                                                  TransformToVkImageLayout(depthStencilAttachment.eStateInitial),
                                                  TransformToVkImageLayout(depthStencilAttachment.eStateFinal));
    }
    void RHIVulkanConverter::TransformToVkAttachmentDescription(VkAttachmentDescriptionVector& aAttachmentDescription,   
                                                                const RHIGraphicsPassColorAttachmentVector& aColorAttachment,
                                                                const RHIGraphicsPassDepthStencilAttachmentVector& aDepthStencilAttachment)
    {
        size_t count_color = aColorAttachment.size();
        size_t count_depthstencil = aDepthStencilAttachment.size();
        aAttachmentDescription.resize(count_color + count_depthstencil);
        for (size_t i = 0; i < count_color; i++)
        {
            const RHIGraphicsPassColorAttachment& attachment = aColorAttachment[i];
            VkAttachmentDescription desc = TransformToVkAttachmentDescriptionFromColorAttachment(attachment);
            aAttachmentDescription[attachment.nIndex] = desc;
        }
        for (size_t i = 0; i < count_depthstencil; i++)
        {
            const RHIGraphicsPassDepthStencilAttachment& attachment = aDepthStencilAttachment[i];
            VkAttachmentDescription desc = TransformToVkAttachmentDescriptionFromDepthStencilAttachment(attachment);
            aAttachmentDescription[attachment.nIndex] = desc;
        }
    }

    VkSubpassDescription RHIVulkanConverter::TransformToVkSubpassDescription(VkSubpassDescriptionFlags flags,
                                                                             VkPipelineBindPoint pipelineBindPoint,
                                                                             uint32_t inputAttachmentCount,
                                                                             const VkAttachmentReference* pInputAttachments,
                                                                             uint32_t colorAttachmentCount,
                                                                             const VkAttachmentReference* pColorAttachments,
                                                                             const VkAttachmentReference* pResolveAttachments,
                                                                             const VkAttachmentReference* pDepthStencilAttachment,
                                                                             uint32_t preserveAttachmentCount,
                                                                             const uint32_t* pPreserveAttachments)
    {
        VkSubpassDescription desc = {};
        desc.flags = flags;
        desc.pipelineBindPoint = pipelineBindPoint;
        desc.inputAttachmentCount = inputAttachmentCount;
        desc.pInputAttachments = pInputAttachments;
        desc.colorAttachmentCount = colorAttachmentCount;
        desc.pColorAttachments = pColorAttachments;
        desc.pResolveAttachments = pResolveAttachments;
        desc.pDepthStencilAttachment = pDepthStencilAttachment;
        desc.preserveAttachmentCount = preserveAttachmentCount;
        desc.pPreserveAttachments = pPreserveAttachments;

        return desc;
    }                              
    VkAttachmentReference RHIVulkanConverter::TransformToVkAttachmentReference(const RHIGraphicsAttachmentReference& reference)
    {   
        VkAttachmentReference vkAttachmentReference = {};
        vkAttachmentReference.attachment = reference.nAttachment;
        vkAttachmentReference.layout = TransformToVkImageLayout(reference.eState);

        return vkAttachmentReference;
    }
    void RHIVulkanConverter::TransformToVkAttachmentReferenceVector(VkAttachmentReferenceVector& aVkReference,
                                                                    const RHIGraphicsAttachmentReferenceVector& aAttachmentReference)   
    {
        size_t count = aAttachmentReference.size();
        for (size_t i = 0; i < count; i++)
        {
            VkAttachmentReference vkAttachmentReference = TransformToVkAttachmentReference(aAttachmentReference[i]);
            aVkReference.push_back(vkAttachmentReference);
        }
    }                  
    VkSubpassDescription RHIVulkanConverter::TransformToVkSubpassDescription(const RHIGraphicsSubpassDescription& desc)
    {
        VkAttachmentReferenceVector aVkReferenceInput;
        TransformToVkAttachmentReferenceVector(aVkReferenceInput, desc.aAttachmentReferenceInput);
        VkAttachmentReferenceVector aVkReferenceColor;
        TransformToVkAttachmentReferenceVector(aVkReferenceColor, desc.aAttachmentReferenceColor);
        VkAttachmentReferenceVector aVkReferenceResolve;
        TransformToVkAttachmentReferenceVector(aVkReferenceResolve, desc.aAttachmentReferenceResolve);
        VkAttachmentReferenceVector aVkReferenceDepthStencil;
        TransformToVkAttachmentReferenceVector(aVkReferenceDepthStencil, desc.aAttachmentReferenceDepthStencil);

        return TransformToVkSubpassDescription(0,
                                               TransformToVkPipelineBindPoint(desc.ePipelineBindPoint),
                                               (uint32_t)aVkReferenceInput.size(),
                                               aVkReferenceInput.data(),
                                               (uint32_t)aVkReferenceColor.size(),
                                               aVkReferenceColor.data(),
                                               aVkReferenceResolve.data(),
                                               aVkReferenceDepthStencil.data(),
                                               0,
                                               nullptr);
    }   
    void RHIVulkanConverter::TransformToVkSubpassDescription(VkSubpassDescriptionVector& aVkDesc,
                                                             const RHIGraphicsSubpassDescriptionVector& aDesc)
    {
        size_t count = aDesc.size();
        for (size_t i = 0; i < count; i++)
        {
            VkSubpassDescription vkSubpassDescription = TransformToVkSubpassDescription(aDesc[i]);
            aVkDesc.push_back(vkSubpassDescription);
        }
    }                                 

    VkSubpassDependency RHIVulkanConverter::TransformToVkSubpassDependency(uint32_t srcSubpass,
                                                                           uint32_t dstSubpass,
                                                                           VkPipelineStageFlags srcStageMask,
                                                                           VkPipelineStageFlags dstStageMask,
                                                                           VkAccessFlags srcAccessMask,
                                                                           VkAccessFlags dstAccessMask,
                                                                           VkDependencyFlags dependencyFlags)
    {
        VkSubpassDependency vkSubpassDependency = {};
        vkSubpassDependency.srcSubpass = srcSubpass;
        vkSubpassDependency.dstSubpass = dstSubpass;
        vkSubpassDependency.srcStageMask = srcStageMask;
        vkSubpassDependency.dstStageMask = dstStageMask;
        vkSubpassDependency.srcAccessMask = srcAccessMask;
        vkSubpassDependency.dstAccessMask = dstAccessMask;
        vkSubpassDependency.dependencyFlags = dependencyFlags;

        return vkSubpassDependency;
    }                                                  
    VkSubpassDependency RHIVulkanConverter::TransformToVkSubpassDependency(const RHIGraphicsSubpassDependency& dependency)
    {
        return TransformToVkSubpassDependency(dependency.nSrcSubpass,
                                              dependency.nDstSubpass,
                                              TransformToVkPipelineStageFlagsFromPipelineStageFlags(dependency.nSrcStageMask),
                                              TransformToVkPipelineStageFlagsFromPipelineStageFlags(dependency.nDstStageMask),
                                              TransformToVkAccessFlagsFromAccessFlags(dependency.nSrcAccessMask),
                                              TransformToVkAccessFlagsFromAccessFlags(dependency.nDstAccessMask),
                                              TransformToVkDependencyFlagsFromPipelineStageFlags(dependency.nDependencyMask));
    }
    void RHIVulkanConverter::TransformToVkSubpassDependency(VkSubpassDependencyVector& aVkDependency,
                                                            const RHIGraphicsSubpassDependencyVector& aDependency)
    {
        size_t count = aDependency.size();
        for (size_t i = 0; i < count; i++)
        {
            VkSubpassDependency vkSubpassDependency = TransformToVkSubpassDependency(aDependency[i]);
            aVkDependency.push_back(vkSubpassDependency);
        }
    }

    VkFramebufferCreateInfo RHIVulkanConverter::TransformToVkFramebufferCreateInfo(VkFramebufferCreateFlags flags,
                                                                                   VkRenderPass renderPass,
                                                                                   uint32_t attachmentCount,
                                                                                   const VkImageView* pAttachments,
                                                                                   uint32_t width,
                                                                                   uint32_t height,
                                                                                   uint32_t layers)
    {
        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.flags = flags;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = attachmentCount;
        framebufferInfo.pAttachments = pAttachments;
        framebufferInfo.width = width;
        framebufferInfo.height = height;
        framebufferInfo.layers = layers;

        return framebufferInfo;
    }
    void RHIVulkanConverter::TransformToVkFramebufferCreateInfo(VkFramebufferCreateFlags flags,
                                                                VkRenderPass renderPass,
                                                                uint32_t attachmentCount,
                                                                const VkImageView* pAttachments,
                                                                uint32_t width,
                                                                uint32_t height,
                                                                uint32_t layers,
                                                                VkFramebufferCreateInfo& createInfo)
    {
        createInfo = TransformToVkFramebufferCreateInfo(flags,
                                                        renderPass,
                                                        attachmentCount,
                                                        pAttachments,
                                                        width,
                                                        height,
                                                        layers);
    }                                                 

}; //LostPeterPluginRHIVulkan