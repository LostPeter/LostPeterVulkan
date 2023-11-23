/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-23
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanConverter.h"

namespace LostPeter
{
    VkPrimitiveTopology VulkanConverter::Transform2VkPrimitiveTopology(FRenderPrimitiveType eRenderPrimitive)
    {
        switch ((int32)eRenderPrimitive)
        {
        case F_RenderPrimitive_PointList:     return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
        case F_RenderPrimitive_LineList:      return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
        case F_RenderPrimitive_LineStrip:     return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
        case F_RenderPrimitive_TriangleList:  return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        case F_RenderPrimitive_TriangleStrip: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
        case F_RenderPrimitive_TriangleFan:   return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
        default:
            return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
        }
    }

    VkCullModeFlags VulkanConverter::Transform2VkCullModeFlags(FCullingType eCulling)
    {
        switch ((int32)eCulling)
        {
        case F_Culling_None:             return VK_CULL_MODE_NONE;
        case F_Culling_ClockWise:        return VK_CULL_MODE_BACK_BIT;
        case F_Culling_CounterClockWise: return VK_CULL_MODE_FRONT_BIT;
        }
        return VK_CULL_MODE_BACK_BIT;
    }

    VkCompareOp VulkanConverter::Transform2VkCompareOp(FCompareFuncType eCompareFunc)
    {
        switch ((int32)eCompareFunc)
        {
        case F_CompareFunc_AlwaysPass:      return VK_COMPARE_OP_ALWAYS;
        case F_CompareFunc_AlwaysFail:      return VK_COMPARE_OP_NEVER;
        case F_CompareFunc_Less:            return VK_COMPARE_OP_LESS;
        case F_CompareFunc_LessEqual:       return VK_COMPARE_OP_LESS_OR_EQUAL;
        case F_CompareFunc_Equal:           return VK_COMPARE_OP_EQUAL;
        case F_CompareFunc_NotEqual:        return VK_COMPARE_OP_NOT_EQUAL;
        case F_CompareFunc_GreaterEqual:    return VK_COMPARE_OP_GREATER_OR_EQUAL;
        case F_CompareFunc_Greater:         return VK_COMPARE_OP_GREATER;
        }
        return VK_COMPARE_OP_NEVER;
    }

    VkPolygonMode VulkanConverter::Transform2VkPolygonMode(FPolygonType ePolygon)
    {
        switch ((int32)ePolygon)
        {
        case F_Polygon_Point:      return VK_POLYGON_MODE_POINT;
        case F_Polygon_WireFrame:  return VK_POLYGON_MODE_LINE;
        case F_Polygon_Solid:      return VK_POLYGON_MODE_FILL;
        }
        return VK_POLYGON_MODE_FILL;
    }

    VkBlendOp VulkanConverter::Transform2VkBlendOp(FSceneBlendingOPType eSceneBlendingOP)
    {
        switch ((int32)eSceneBlendingOP)
        {
        case F_SceneBlendingOP_Add:               return VK_BLEND_OP_ADD;
        case F_SceneBlendingOP_Subtract:          return VK_BLEND_OP_SUBTRACT;
        case F_SceneBlendingOP_SubtractReverse:   return VK_BLEND_OP_REVERSE_SUBTRACT;
        case F_SceneBlendingOP_Min:               return VK_BLEND_OP_MIN;
        case F_SceneBlendingOP_Max:               return VK_BLEND_OP_MAX;
        }
        return VK_BLEND_OP_ADD;
    }

    VkBlendFactor VulkanConverter::Transform2VkBlendFactor(FSceneBlendingFactorType eSceneBlendingFactor)
    {
        switch ((int32)eSceneBlendingFactor)
        {
        case F_SceneBlendingFactor_One:                   return VK_BLEND_FACTOR_ONE;
        case F_SceneBlendingFactor_Zero:                  return VK_BLEND_FACTOR_ZERO;
        case F_SceneBlendingFactor_SourceColor:           return VK_BLEND_FACTOR_SRC_COLOR;
        case F_SceneBlendingFactor_DestColor:             return VK_BLEND_FACTOR_DST_COLOR;
        case F_SceneBlendingFactor_OneMinusSourceColor:   return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
        case F_SceneBlendingFactor_OneMinusDestColor:     return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
        case F_SceneBlendingFactor_SourceAlpha:           return VK_BLEND_FACTOR_SRC_ALPHA;
        case F_SceneBlendingFactor_DestAlpha:             return VK_BLEND_FACTOR_DST_ALPHA;
        case F_SceneBlendingFactor_OneMinusSourceAlpha:   return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        case F_SceneBlendingFactor_OneMinusDestAlpha:     return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
        }
        return VK_BLEND_FACTOR_ONE;
    }

    VkStencilOp VulkanConverter::Transform2VkStencilOp(FStencilOPType eStencilOP)
    {
        switch ((int32)eStencilOP)
        {
        case F_StencilOP_Keep:              return VK_STENCIL_OP_KEEP;
        case F_StencilOP_Zero:              return VK_STENCIL_OP_ZERO;
        case F_StencilOP_Replace:           return VK_STENCIL_OP_REPLACE;
        case F_StencilOP_Increment:         return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
        case F_StencilOP_Decrement:         return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
        case F_StencilOP_IncrementWrap:     return VK_STENCIL_OP_INCREMENT_AND_WRAP;
        case F_StencilOP_DecrementWrap:     return VK_STENCIL_OP_DECREMENT_AND_WRAP;
        case F_StencilOP_Invert:            return VK_STENCIL_OP_INVERT;
        }
        return VK_STENCIL_OP_KEEP;
    }

    VkImageViewType VulkanConverter::Transform2VkImageViewType(FTextureType eTexture)
    {
        switch ((int32)eTexture)
        {
        case F_Texture_1D:          return VK_IMAGE_VIEW_TYPE_1D;
        case F_Texture_2D:          return VK_IMAGE_VIEW_TYPE_2D;
        case F_Texture_2DArray:     return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        case F_Texture_3D:          return VK_IMAGE_VIEW_TYPE_3D;
        case F_Texture_CubeMap:     return VK_IMAGE_VIEW_TYPE_CUBE;
        }
        return VK_IMAGE_VIEW_TYPE_2D;
    }

    VkFilter VulkanConverter::Transform2VkFilter(FTextureFilterPixelType eTextureFilterPixel)
    {
        switch ((int32)eTextureFilterPixel)
        {
        case F_TextureFilterPixel_None:             return VK_FILTER_NEAREST;
        case F_TextureFilterPixel_Point:            return VK_FILTER_NEAREST;
        case F_TextureFilterPixel_Linear:           return VK_FILTER_LINEAR;
        case F_TextureFilterPixel_Anisotropic:      return VK_FILTER_LINEAR;
        }
        return VK_FILTER_NEAREST;
    }

    VkSamplerMipmapMode VulkanConverter::Transform2VkSamplerMipmapMode(FTextureFilterPixelType eTextureFilterPixel)
    {
        switch ((int32)eTextureFilterPixel)
        {
        case F_TextureFilterPixel_None:             return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        case F_TextureFilterPixel_Point:            return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        case F_TextureFilterPixel_Linear:           return VK_SAMPLER_MIPMAP_MODE_LINEAR;
        case F_TextureFilterPixel_Anisotropic:      return VK_SAMPLER_MIPMAP_MODE_LINEAR;
        }
        return VK_SAMPLER_MIPMAP_MODE_NEAREST;
    }   

    VkSamplerAddressMode VulkanConverter::Transform2VkSamplerAddressMode(FTextureAddressingType eTextureAddressing)
    {
        switch ((int32)eTextureAddressing)
        {
        case F_TextureAddressing_Wrap:              return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        case F_TextureAddressing_Mirror:            return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
        case F_TextureAddressing_Clamp:             return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        case F_TextureAddressing_Border:            return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        }
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    }

    VkFormat VulkanConverter::Transform2VkFormat(FPixelFormatType ePixelFormat)
    {
        switch ((int32)ePixelFormat)
        {
        // BYTE CHANNEL
        case F_PixelFormat_BYTE_R4G4_UNORM_PACK8:          return VK_FORMAT_R4G4_UNORM_PACK8;              //1
        case F_PixelFormat_BYTE_L8_UNORM:                  return VK_FORMAT_R8_UNORM;                      //2
        case F_PixelFormat_BYTE_A8_UNORM:                  return VK_FORMAT_R8_UNORM;                      //3
        case F_PixelFormat_BYTE_R8_UNORM:                  return VK_FORMAT_R8_UNORM;                      //4
        case F_PixelFormat_BYTE_R8_SNORM:                  return VK_FORMAT_R8_SNORM;                      //5
        case F_PixelFormat_BYTE_R8_USCALED:                return VK_FORMAT_R8_USCALED;                    //6
        case F_PixelFormat_BYTE_R8_SSCALED:                return VK_FORMAT_R8_SSCALED;                    //7
        case F_PixelFormat_BYTE_R8_UINT:                   return VK_FORMAT_R8_UINT;                       //8
        case F_PixelFormat_BYTE_R8_SINT:                   return VK_FORMAT_R8_SINT;                       //9
        case F_PixelFormat_BYTE_R8_SRGB:                   return VK_FORMAT_R8_SRGB;                       //10
        
        case F_PixelFormat_BYTE_R4G4B4A4_UNORM_PACK16:     return VK_FORMAT_R4G4B4A4_UNORM_PACK16;         //11
        case F_PixelFormat_BYTE_B4G4R4A4_UNORM_PACK16:     return VK_FORMAT_B4G4R4A4_UNORM_PACK16;         //12
        case F_PixelFormat_BYTE_R5G6B5_UNORM_PACK16:       return VK_FORMAT_R5G6B5_UNORM_PACK16;           //13
        case F_PixelFormat_BYTE_B5G6R5_UNORM_PACK16:       return VK_FORMAT_B5G6R5_UNORM_PACK16;           //14
        case F_PixelFormat_BYTE_R5G5B5A1_UNORM_PACK16:     return VK_FORMAT_R5G5B5A1_UNORM_PACK16;         //15
        case F_PixelFormat_BYTE_B5G5R5A1_UNORM_PACK16:     return VK_FORMAT_B5G5R5A1_UNORM_PACK16;         //16
        case F_PixelFormat_BYTE_A1R5G5B5_UNORM_PACK16:     return VK_FORMAT_A1R5G5B5_UNORM_PACK16;         //17
        case F_PixelFormat_BYTE_AL_UNORM:                  return VK_FORMAT_R8G8_UNORM;                    //18
        case F_PixelFormat_BYTE_G8R8_UNORM:                return VK_FORMAT_R8G8_UNORM;                    //19
        case F_PixelFormat_BYTE_G8R8_SNORM:                return VK_FORMAT_R8G8_SNORM;                    //20
        case F_PixelFormat_BYTE_G8R8_USCALED:              return VK_FORMAT_R8G8_USCALED;                  //21
        case F_PixelFormat_BYTE_G8R8_SSCALED:              return VK_FORMAT_R8G8_SSCALED;                  //22
        case F_PixelFormat_BYTE_G8R8_UINT:                 return VK_FORMAT_R8G8_UINT;                     //23
        case F_PixelFormat_BYTE_G8R8_SINT:                 return VK_FORMAT_R8G8_SINT;                     //24
        case F_PixelFormat_BYTE_G8R8_SRGB:                 return VK_FORMAT_R8G8_SRGB;                     //25
        
        case F_PixelFormat_BYTE_R8G8B8_UNORM:              return VK_FORMAT_B8G8R8_UNORM;                  //26
        case F_PixelFormat_BYTE_R8G8B8_SNORM:              return VK_FORMAT_B8G8R8_SNORM;                  //27
        case F_PixelFormat_BYTE_R8G8B8_USCALED:            return VK_FORMAT_B8G8R8_USCALED;                //28
        case F_PixelFormat_BYTE_R8G8B8_SSCALED:            return VK_FORMAT_B8G8R8_SSCALED;                //29
        case F_PixelFormat_BYTE_R8G8B8_UINT:               return VK_FORMAT_B8G8R8_UINT;                   //30
        case F_PixelFormat_BYTE_R8G8B8_SINT:               return VK_FORMAT_B8G8R8_SINT;                   //31
        case F_PixelFormat_BYTE_R8G8B8_SRGB:               return VK_FORMAT_B8G8R8_SRGB;                   //32
        case F_PixelFormat_BYTE_B8G8R8_UNORM:              return VK_FORMAT_R8G8B8_UNORM;                  //33
        case F_PixelFormat_BYTE_B8G8R8_SNORM:              return VK_FORMAT_R8G8B8_SNORM;                  //34
        case F_PixelFormat_BYTE_B8G8R8_USCALED:            return VK_FORMAT_R8G8B8_USCALED;                //35
        case F_PixelFormat_BYTE_B8G8R8_SSCALED:            return VK_FORMAT_R8G8B8_SSCALED;                //36
        case F_PixelFormat_BYTE_B8G8R8_UINT:               return VK_FORMAT_R8G8B8_UINT;                   //37
        case F_PixelFormat_BYTE_B8G8R8_SINT:               return VK_FORMAT_R8G8B8_SINT;                   //38
        case F_PixelFormat_BYTE_B8G8R8_SRGB:               return VK_FORMAT_R8G8B8_SRGB;                   //39

        case F_PixelFormat_BYTE_A8B8G8R8_UNORM_PACK32:     return VK_FORMAT_A8B8G8R8_UNORM_PACK32;         //40
        case F_PixelFormat_BYTE_A8B8G8R8_SNORM_PACK32:     return VK_FORMAT_A8B8G8R8_SNORM_PACK32;         //41
        case F_PixelFormat_BYTE_A8B8G8R8_USCALED_PACK32:   return VK_FORMAT_A8B8G8R8_USCALED_PACK32;       //42
        case F_PixelFormat_BYTE_A8B8G8R8_SSCALED_PACK32:   return VK_FORMAT_A8B8G8R8_SSCALED_PACK32;       //43
        case F_PixelFormat_BYTE_A8B8G8R8_UINT_PACK32:      return VK_FORMAT_A8B8G8R8_UINT_PACK32;          //44
        case F_PixelFormat_BYTE_A8B8G8R8_SINT_PACK32:      return VK_FORMAT_A8B8G8R8_SINT_PACK32;          //45
        case F_PixelFormat_BYTE_A8B8G8R8_SRGB_PACK32:      return VK_FORMAT_A8B8G8R8_SRGB_PACK32;          //46
        case F_PixelFormat_BYTE_A2R10G10B10_UNORM_PACK32:  return VK_FORMAT_A2R10G10B10_UNORM_PACK32;      //47
        case F_PixelFormat_BYTE_A2R10G10B10_SNORM_PACK32:  return VK_FORMAT_A2R10G10B10_SNORM_PACK32;      //48
        case F_PixelFormat_BYTE_A2R10G10B10_USCALED_PACK32: return VK_FORMAT_A2R10G10B10_USCALED_PACK32;   //49
        case F_PixelFormat_BYTE_A2R10G10B10_SSCALED_PACK32: return VK_FORMAT_A2R10G10B10_SSCALED_PACK32;   //50
        case F_PixelFormat_BYTE_A2R10G10B10_UINT_PACK32:   return VK_FORMAT_A2R10G10B10_UINT_PACK32;       //51
        case F_PixelFormat_BYTE_A2R10G10B10_SINT_PACK32:   return VK_FORMAT_A2R10G10B10_SINT_PACK32;       //52
        case F_PixelFormat_BYTE_A2B10G10R10_UNORM_PACK32:  return VK_FORMAT_A2B10G10R10_UNORM_PACK32;      //53
        case F_PixelFormat_BYTE_A2B10G10R10_SNORM_PACK32:  return VK_FORMAT_A2B10G10R10_SNORM_PACK32;      //54
        case F_PixelFormat_BYTE_A2B10G10R10_USCALED_PACK32: return VK_FORMAT_A2B10G10R10_USCALED_PACK32;   //55
        case F_PixelFormat_BYTE_A2B10G10R10_SSCALED_PACK32: return VK_FORMAT_A2B10G10R10_SSCALED_PACK32;   //56
        case F_PixelFormat_BYTE_A2B10G10R10_UINT_PACK32:   return VK_FORMAT_A2B10G10R10_UINT_PACK32;       //57
        case F_PixelFormat_BYTE_A2B10G10R10_SINT_PACK32:   return VK_FORMAT_A2B10G10R10_SINT_PACK32;       //58
        case F_PixelFormat_BYTE_A8R8G8B8_UNORM:            return VK_FORMAT_B8G8R8A8_UNORM;                //59
        case F_PixelFormat_BYTE_A8R8G8B8_SNORM:            return VK_FORMAT_B8G8R8A8_SNORM;                //60
        case F_PixelFormat_BYTE_A8R8G8B8_USCALED:          return VK_FORMAT_B8G8R8A8_USCALED;              //61
        case F_PixelFormat_BYTE_A8R8G8B8_SSCALED:          return VK_FORMAT_B8G8R8A8_SSCALED;              //62
        case F_PixelFormat_BYTE_A8R8G8B8_UINT:             return VK_FORMAT_B8G8R8A8_UINT;                 //63
        case F_PixelFormat_BYTE_A8R8G8B8_SINT:             return VK_FORMAT_B8G8R8A8_SINT;                 //64
        case F_PixelFormat_BYTE_A8R8G8B8_SRGB:             return VK_FORMAT_B8G8R8A8_SRGB;                 //65
        case F_PixelFormat_BYTE_A8B8G8R8_UNORM:            return VK_FORMAT_R8G8B8A8_UNORM;                //66
        case F_PixelFormat_BYTE_A8B8G8R8_SNORM:            return VK_FORMAT_R8G8B8A8_SNORM;                //67
        case F_PixelFormat_BYTE_A8B8G8R8_USCALED:          return VK_FORMAT_R8G8B8A8_USCALED;              //68
        case F_PixelFormat_BYTE_A8B8G8R8_SSCALED:          return VK_FORMAT_R8G8B8A8_SSCALED;              //69
        case F_PixelFormat_BYTE_A8B8G8R8_UINT:             return VK_FORMAT_R8G8B8A8_UINT;                 //70
        case F_PixelFormat_BYTE_A8B8G8R8_SINT:             return VK_FORMAT_R8G8B8A8_SINT;                 //71
        case F_PixelFormat_BYTE_A8B8G8R8_SRGB:             return VK_FORMAT_R8G8B8A8_SRGB;                 //72

        // SHORT CHANNEL
        case F_PixelFormat_SHORT_L16_UNORM:                return VK_FORMAT_R16_UNORM;                     //73
        case F_PixelFormat_SHORT_R16_UNORM:                return VK_FORMAT_R16_UNORM;                     //74
        case F_PixelFormat_SHORT_R16_SNORM:                return VK_FORMAT_R16_SNORM;                     //75
        case F_PixelFormat_SHORT_R16_USCALED:              return VK_FORMAT_R16_USCALED;                   //76
        case F_PixelFormat_SHORT_R16_SSCALED:              return VK_FORMAT_R16_SSCALED;                   //77
        case F_PixelFormat_SHORT_R16_UINT:                 return VK_FORMAT_R16_UINT;                      //78
        case F_PixelFormat_SHORT_R16_SINT:                 return VK_FORMAT_R16_SINT;                      //79
        case F_PixelFormat_SHORT_R16_SFLOAT:               return VK_FORMAT_R16_SFLOAT;                    //80

        case F_PixelFormat_SHORT_G16R16_UNORM:             return VK_FORMAT_R16G16_UNORM;                  //81
        case F_PixelFormat_SHORT_G16R16_SNORM:             return VK_FORMAT_R16G16_SNORM;                  //82
        case F_PixelFormat_SHORT_G16R16_USCALED:           return VK_FORMAT_R16G16_USCALED;                //83
        case F_PixelFormat_SHORT_G16R16_SSCALED:           return VK_FORMAT_R16G16_SSCALED;                //84
        case F_PixelFormat_SHORT_G16R16_UINT:              return VK_FORMAT_R16G16_UINT;                   //85
        case F_PixelFormat_SHORT_G16R16_SINT:              return VK_FORMAT_R16G16_SINT;                   //86
        case F_PixelFormat_SHORT_G16R16_SFLOAT:            return VK_FORMAT_R16G16_SFLOAT;                 //87

        case F_PixelFormat_SHORT_B16G16R16_UNORM:          return VK_FORMAT_R16G16B16_UNORM;               //88
        case F_PixelFormat_SHORT_B16G16R16_SNORM:          return VK_FORMAT_R16G16B16_SNORM;               //89
        case F_PixelFormat_SHORT_B16G16R16_USCALED:        return VK_FORMAT_R16G16B16_USCALED;             //90
        case F_PixelFormat_SHORT_B16G16R16_SSCALED:        return VK_FORMAT_R16G16B16_SSCALED;             //91
        case F_PixelFormat_SHORT_B16G16R16_UINT:           return VK_FORMAT_R16G16B16_UINT;                //92
        case F_PixelFormat_SHORT_B16G16R16_SINT:           return VK_FORMAT_R16G16B16_SINT;                //93
        case F_PixelFormat_SHORT_B16G16R16_SFLOAT:         return VK_FORMAT_R16G16B16_SFLOAT;              //94

        case F_PixelFormat_SHORT_A16B16G16R16_UNORM:       return VK_FORMAT_R16G16B16A16_UNORM;            //95
        case F_PixelFormat_SHORT_A16B16G16R16_SNORM:       return VK_FORMAT_R16G16B16A16_SNORM;            //96
        case F_PixelFormat_SHORT_A16B16G16R16_USCALED:     return VK_FORMAT_R16G16B16A16_USCALED;          //97
        case F_PixelFormat_SHORT_A16B16G16R16_SSCALED:     return VK_FORMAT_R16G16B16A16_SSCALED;          //98
        case F_PixelFormat_SHORT_A16B16G16R16_UINT:        return VK_FORMAT_R16G16B16A16_UINT;             //99
        case F_PixelFormat_SHORT_A16B16G16R16_SINT:        return VK_FORMAT_R16G16B16A16_SINT;             //100
        case F_PixelFormat_SHORT_A16B16G16R16_SFLOAT:      return VK_FORMAT_R16G16B16A16_SFLOAT;           //101

        // INT CHANNEL
        case F_PixelFormat_INT_R32_UINT:                   return VK_FORMAT_R32_UINT;                      //102
        case F_PixelFormat_INT_R32_SINT:                   return VK_FORMAT_R32_SINT;                      //103
        case F_PixelFormat_INT_R32_SFLOAT:                 return VK_FORMAT_R32_SFLOAT;                    //104

        case F_PixelFormat_INT_G32R32_UINT:                return VK_FORMAT_R32G32_UINT;                   //105
        case F_PixelFormat_INT_G32R32_SINT:                return VK_FORMAT_R32G32_SINT;                   //106
        case F_PixelFormat_INT_G32R32_SFLOAT:              return VK_FORMAT_R32G32_SFLOAT;                 //107

        case F_PixelFormat_INT_B32G32R32_UINT:             return VK_FORMAT_R32G32B32_UINT;                //108
        case F_PixelFormat_INT_B32G32R32_SINT:             return VK_FORMAT_R32G32B32_SINT;                //109
        case F_PixelFormat_INT_B32G32R32_SFLOAT:           return VK_FORMAT_R32G32B32_SFLOAT;              //110

        case F_PixelFormat_INT_A32B32G32R32_UINT:          return VK_FORMAT_R32G32B32A32_UINT;             //111
        case F_PixelFormat_INT_A32B32G32R32_SINT:          return VK_FORMAT_R32G32B32A32_SINT;             //112
        case F_PixelFormat_INT_A32B32G32R32_SFLOAT:        return VK_FORMAT_R32G32B32A32_SFLOAT;           //113

        // LONG CHANNEL
        case F_PixelFormat_LONG_R64_UINT:                  return VK_FORMAT_R64_UINT;                      //114
        case F_PixelFormat_LONG_R64_SINT:                  return VK_FORMAT_R64_SINT;                      //115
        case F_PixelFormat_LONG_R64_SFLOAT:                return VK_FORMAT_R64_SFLOAT;                    //116

        case F_PixelFormat_LONG_G64R64_UINT:               return VK_FORMAT_R64G64_UINT;                   //117
        case F_PixelFormat_LONG_G64R64_SINT:               return VK_FORMAT_R64G64_SINT;                   //118
        case F_PixelFormat_LONG_G64R64_SFLOAT:             return VK_FORMAT_R64G64_SFLOAT;                 //119

        case F_PixelFormat_LONG_B64G64R64_UINT:            return VK_FORMAT_R64G64B64_UINT;                //120
        case F_PixelFormat_LONG_B64G64R64_SINT:            return VK_FORMAT_R64G64B64_SINT;                //121
        case F_PixelFormat_LONG_B64G64R64_SFLOAT:          return VK_FORMAT_R64G64B64_SFLOAT;              //122

        case F_PixelFormat_LONG_A64B64G32R64_UINT:         return VK_FORMAT_R64G64B64A64_UINT;             //123
        case F_PixelFormat_LONG_A64B64G32R64_SINT:         return VK_FORMAT_R64G64B64A64_SINT;             //124
        case F_PixelFormat_LONG_A64B64G32R64_SFLOAT:       return VK_FORMAT_R64G64B64A64_SFLOAT;           //125

        // FLOAT
        case F_PixelFormat_FLOAT_B10G11R11_UFLOAT_PACK32:  return VK_FORMAT_B10G11R11_UFLOAT_PACK32;       //126
        case F_PixelFormat_FLOAT_E5B9G9R9_UFLOAT_PACK32:   return VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;        //127

        // STENCIL
        case F_PixelFormat_STENCIL_S8_UINT:                return VK_FORMAT_S8_UINT;                       //128

        // DEPTH
        case F_PixelFormat_DEPTH_D16_UNORM:                return VK_FORMAT_D16_UNORM;                     //129
        case F_PixelFormat_DEPTH_D24_UNORM:                return VK_FORMAT_X8_D24_UNORM_PACK32;           //130
        case F_PixelFormat_DEPTH_D32_SFLOAT:               return VK_FORMAT_D32_SFLOAT;                    //131

        // DEPTHSTENCIL
        case F_PixelFormat_DEPTHSTENCIL_D16_UNORM_S8_UINT: return VK_FORMAT_D16_UNORM_S8_UINT;             //132
        case F_PixelFormat_DEPTHSTENCIL_D24_UNORM_S8_UINT: return VK_FORMAT_D24_UNORM_S8_UINT;             //133
        case F_PixelFormat_DEPTHSTENCIL_D32_SFLOAT_S8_UINT: return VK_FORMAT_D32_SFLOAT_S8_UINT;           //134

        // DXT
        case F_PixelFormat_DXT1:                           return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;          //135
        case F_PixelFormat_DXT2:                           return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;          //136
        case F_PixelFormat_DXT3:                           return VK_FORMAT_BC2_UNORM_BLOCK;               //137
        case F_PixelFormat_DXT4:                           return VK_FORMAT_BC2_UNORM_BLOCK;               //138
        case F_PixelFormat_DXT5:                           return VK_FORMAT_BC3_UNORM_BLOCK;               //139

        // BC
        case F_PixelFormat_BC4_UNORM:                      return VK_FORMAT_BC4_UNORM_BLOCK;               //140
        case F_PixelFormat_BC4_SNORM:                      return VK_FORMAT_BC4_SNORM_BLOCK;               //141
        case F_PixelFormat_BC5_UNORM:                      return VK_FORMAT_BC5_UNORM_BLOCK;               //142
        case F_PixelFormat_BC5_SNORM:                      return VK_FORMAT_BC5_SNORM_BLOCK;               //143
        case F_PixelFormat_BC6H_UF16:                      return VK_FORMAT_BC6H_UFLOAT_BLOCK;             //144
        case F_PixelFormat_BC6H_SF16:                      return VK_FORMAT_BC6H_SFLOAT_BLOCK;             //145
        case F_PixelFormat_BC7_UNORM:                      return VK_FORMAT_BC7_UNORM_BLOCK;               //146

        // PVRTC (PowerVR)
        case F_PixelFormat_PVRTC_RGB2:                     return VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG;   //147
        case F_PixelFormat_PVRTC_RGBA2:                    return VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG;   //148
        case F_PixelFormat_PVRTC_RGB4:                     return VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG;   //149
        case F_PixelFormat_PVRTC_RGBA4:                    return VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG;   //150
        case F_PixelFormat_PVRTC2_2BPP:                    return VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG;   //151
        case F_PixelFormat_PVRTC2_4BPP:                    return VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG;   //152

        // ETC
        case F_PixelFormat_ETC1_RGB8:                      return VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK;       //153
        case F_PixelFormat_ETC2_RGB8:                      return VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK;       //154
        case F_PixelFormat_ETC2_RGBA8:                     return VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK;     //155
        case F_PixelFormat_ETC2_RGB8A1:                    return VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK;     //156

        // ATC
        case F_PixelFormat_ATC_RGB:                        return VK_FORMAT_UNDEFINED;                     //157
        case F_PixelFormat_ATC_RGBA_EXPLICIT_ALPHA:        return VK_FORMAT_UNDEFINED;                     //158
        case F_PixelFormat_ATC_RGBA_INTERPOLATED_ALPHA:    return VK_FORMAT_UNDEFINED;                     //159

        // ASTC
        case F_PixelFormat_ASTC_RGBA_4X4_LDR:              return VK_FORMAT_ASTC_4x4_UNORM_BLOCK;          //160
        case F_PixelFormat_ASTC_RGBA_5X4_LDR:              return VK_FORMAT_ASTC_5x4_UNORM_BLOCK;          //161
        case F_PixelFormat_ASTC_RGBA_5X5_LDR:              return VK_FORMAT_ASTC_5x5_UNORM_BLOCK;          //162
        case F_PixelFormat_ASTC_RGBA_6X5_LDR:              return VK_FORMAT_ASTC_6x5_UNORM_BLOCK;          //163
        case F_PixelFormat_ASTC_RGBA_6X6_LDR:              return VK_FORMAT_ASTC_6x6_UNORM_BLOCK;          //164
        case F_PixelFormat_ASTC_RGBA_8X5_LDR:              return VK_FORMAT_ASTC_8x5_UNORM_BLOCK;          //165
        case F_PixelFormat_ASTC_RGBA_8X6_LDR:              return VK_FORMAT_ASTC_8x6_UNORM_BLOCK;          //166
        case F_PixelFormat_ASTC_RGBA_8X8_LDR:              return VK_FORMAT_ASTC_8x8_UNORM_BLOCK;          //167
        case F_PixelFormat_ASTC_RGBA_10X5_LDR:             return VK_FORMAT_ASTC_10x5_UNORM_BLOCK;         //168
        case F_PixelFormat_ASTC_RGBA_10X6_LDR:             return VK_FORMAT_ASTC_10x6_UNORM_BLOCK;         //169
        case F_PixelFormat_ASTC_RGBA_10X8_LDR:             return VK_FORMAT_ASTC_10x8_UNORM_BLOCK;         //170
        case F_PixelFormat_ASTC_RGBA_10X10_LDR:            return VK_FORMAT_ASTC_10x10_UNORM_BLOCK;        //171
        case F_PixelFormat_ASTC_RGBA_12X10_LDR:            return VK_FORMAT_ASTC_12x10_UNORM_BLOCK;        //172
        case F_PixelFormat_ASTC_RGBA_12X12_LDR:            return VK_FORMAT_ASTC_12x12_UNORM_BLOCK;        //173

        }

        return VK_FORMAT_UNDEFINED;
    }

    VkComponentMapping VulkanConverter::Transform2VkComponentMapping(FPixelFormatType ePixelFormat)
    {
        VkComponentMapping componentMapping;
        componentMapping.r = VK_COMPONENT_SWIZZLE_R;
		componentMapping.g = VK_COMPONENT_SWIZZLE_G;
		componentMapping.b = VK_COMPONENT_SWIZZLE_B;
		componentMapping.a = VK_COMPONENT_SWIZZLE_A;
        
        switch ((int32)ePixelFormat)
        {
        // BYTE CHANNEL
        case F_PixelFormat_BYTE_R4G4_UNORM_PACK8:              //1
            break;
        case F_PixelFormat_BYTE_L8_UNORM:                      //2               
            {
                componentMapping.g = VK_COMPONENT_SWIZZLE_ZERO;
                componentMapping.b = VK_COMPONENT_SWIZZLE_ZERO;
                componentMapping.a = VK_COMPONENT_SWIZZLE_ZERO;
            } 
            break;
        case F_PixelFormat_BYTE_A8_UNORM:                      //3        
            {
                componentMapping.r = VK_COMPONENT_SWIZZLE_ZERO;
                componentMapping.g = VK_COMPONENT_SWIZZLE_ZERO;
                componentMapping.b = VK_COMPONENT_SWIZZLE_ZERO;
                componentMapping.a = VK_COMPONENT_SWIZZLE_R;
            } 
            break;
        case F_PixelFormat_BYTE_R8_UNORM:                      //4
        case F_PixelFormat_BYTE_R8_SNORM:                      //5
        case F_PixelFormat_BYTE_R8_USCALED:                    //6
        case F_PixelFormat_BYTE_R8_SSCALED:                    //7
        case F_PixelFormat_BYTE_R8_UINT:                       //8
        case F_PixelFormat_BYTE_R8_SINT:                       //9
        case F_PixelFormat_BYTE_R8_SRGB:                       //10
            {
                componentMapping.g = VK_COMPONENT_SWIZZLE_ZERO;
                componentMapping.b = VK_COMPONENT_SWIZZLE_ZERO;
                componentMapping.a = VK_COMPONENT_SWIZZLE_ZERO;
            } 
            break;

        case F_PixelFormat_BYTE_R4G4B4A4_UNORM_PACK16:         //11            
        case F_PixelFormat_BYTE_B4G4R4A4_UNORM_PACK16:         //12
        case F_PixelFormat_BYTE_R5G6B5_UNORM_PACK16:           //13  
        case F_PixelFormat_BYTE_B5G6R5_UNORM_PACK16:           //14
        case F_PixelFormat_BYTE_R5G5B5A1_UNORM_PACK16:         //15 
        case F_PixelFormat_BYTE_B5G5R5A1_UNORM_PACK16:         //16
        case F_PixelFormat_BYTE_A1R5G5B5_UNORM_PACK16:         //17
            break;
        case F_PixelFormat_BYTE_AL_UNORM:                      //18      
        case F_PixelFormat_BYTE_G8R8_UNORM:                    //19
        case F_PixelFormat_BYTE_G8R8_SNORM:                    //20
        case F_PixelFormat_BYTE_G8R8_USCALED:                  //21
        case F_PixelFormat_BYTE_G8R8_SSCALED:                  //22
        case F_PixelFormat_BYTE_G8R8_UINT:                     //23
        case F_PixelFormat_BYTE_G8R8_SINT:                     //24
        case F_PixelFormat_BYTE_G8R8_SRGB:                     //25
            {
                componentMapping.b = VK_COMPONENT_SWIZZLE_ZERO;
                componentMapping.a = VK_COMPONENT_SWIZZLE_ZERO;
            } 
            break;       

        case F_PixelFormat_BYTE_R8G8B8_UNORM:                  //26
        case F_PixelFormat_BYTE_R8G8B8_SNORM:                  //27
        case F_PixelFormat_BYTE_R8G8B8_USCALED:                //28
        case F_PixelFormat_BYTE_R8G8B8_SSCALED:                //29
        case F_PixelFormat_BYTE_R8G8B8_UINT:                   //30
        case F_PixelFormat_BYTE_R8G8B8_SINT:                   //31
        case F_PixelFormat_BYTE_R8G8B8_SRGB:                   //32
        case F_PixelFormat_BYTE_B8G8R8_UNORM:                  //33
        case F_PixelFormat_BYTE_B8G8R8_SNORM:                  //34
        case F_PixelFormat_BYTE_B8G8R8_USCALED:                //35
        case F_PixelFormat_BYTE_B8G8R8_SSCALED:                //36
        case F_PixelFormat_BYTE_B8G8R8_UINT:                   //37
        case F_PixelFormat_BYTE_B8G8R8_SINT:                   //38
        case F_PixelFormat_BYTE_B8G8R8_SRGB:                   //39
            {
                componentMapping.a = VK_COMPONENT_SWIZZLE_ZERO;
            } 
            break;

        case F_PixelFormat_BYTE_A8B8G8R8_UNORM_PACK32:         //40
        case F_PixelFormat_BYTE_A8B8G8R8_SNORM_PACK32:         //41
        case F_PixelFormat_BYTE_A8B8G8R8_USCALED_PACK32:       //42
        case F_PixelFormat_BYTE_A8B8G8R8_SSCALED_PACK32:       //43
        case F_PixelFormat_BYTE_A8B8G8R8_UINT_PACK32:          //44
        case F_PixelFormat_BYTE_A8B8G8R8_SINT_PACK32:          //45
        case F_PixelFormat_BYTE_A8B8G8R8_SRGB_PACK32:          //46
        case F_PixelFormat_BYTE_A2R10G10B10_UNORM_PACK32:      //47
        case F_PixelFormat_BYTE_A2R10G10B10_SNORM_PACK32:      //48
        case F_PixelFormat_BYTE_A2R10G10B10_USCALED_PACK32:    //49
        case F_PixelFormat_BYTE_A2R10G10B10_SSCALED_PACK32:    //50
        case F_PixelFormat_BYTE_A2R10G10B10_UINT_PACK32:       //51
        case F_PixelFormat_BYTE_A2R10G10B10_SINT_PACK32:       //52
        case F_PixelFormat_BYTE_A2B10G10R10_UNORM_PACK32:      //53
        case F_PixelFormat_BYTE_A2B10G10R10_SNORM_PACK32:      //54
        case F_PixelFormat_BYTE_A2B10G10R10_USCALED_PACK32:    //55
        case F_PixelFormat_BYTE_A2B10G10R10_SSCALED_PACK32:    //56
        case F_PixelFormat_BYTE_A2B10G10R10_UINT_PACK32:       //57
        case F_PixelFormat_BYTE_A2B10G10R10_SINT_PACK32:       //58
        case F_PixelFormat_BYTE_A8R8G8B8_UNORM:                //59
        case F_PixelFormat_BYTE_A8R8G8B8_SNORM:                //60
        case F_PixelFormat_BYTE_A8R8G8B8_USCALED:              //61
        case F_PixelFormat_BYTE_A8R8G8B8_SSCALED:              //62
        case F_PixelFormat_BYTE_A8R8G8B8_UINT:                 //63
        case F_PixelFormat_BYTE_A8R8G8B8_SINT:                 //64
        case F_PixelFormat_BYTE_A8R8G8B8_SRGB:                 //65
        case F_PixelFormat_BYTE_A8B8G8R8_UNORM:                //66
        case F_PixelFormat_BYTE_A8B8G8R8_SNORM:                //67
        case F_PixelFormat_BYTE_A8B8G8R8_USCALED:              //68
        case F_PixelFormat_BYTE_A8B8G8R8_SSCALED:              //69
        case F_PixelFormat_BYTE_A8B8G8R8_UINT:                 //70
        case F_PixelFormat_BYTE_A8B8G8R8_SINT:                 //71
        case F_PixelFormat_BYTE_A8B8G8R8_SRGB:                 //72
            break;

        // SHORT CHANNEL
        case F_PixelFormat_SHORT_L16_UNORM:                    //73  
        case F_PixelFormat_SHORT_R16_UNORM:                    //74
        case F_PixelFormat_SHORT_R16_SNORM:                    //75  
        case F_PixelFormat_SHORT_R16_USCALED:                  //76  
        case F_PixelFormat_SHORT_R16_SSCALED:                  //77  
        case F_PixelFormat_SHORT_R16_UINT:                     //78             
        case F_PixelFormat_SHORT_R16_SINT:                     //79  
        case F_PixelFormat_SHORT_R16_SFLOAT:                   //80  
            {
                componentMapping.g = VK_COMPONENT_SWIZZLE_ZERO;
                componentMapping.b = VK_COMPONENT_SWIZZLE_ZERO;
                componentMapping.a = VK_COMPONENT_SWIZZLE_ZERO;
            } 
            break;

        case F_PixelFormat_SHORT_G16R16_UNORM:                 //81
        case F_PixelFormat_SHORT_G16R16_SNORM:                 //82
        case F_PixelFormat_SHORT_G16R16_USCALED:               //83
        case F_PixelFormat_SHORT_G16R16_SSCALED:               //84
        case F_PixelFormat_SHORT_G16R16_UINT:                  //85
        case F_PixelFormat_SHORT_G16R16_SINT:                  //86
        case F_PixelFormat_SHORT_G16R16_SFLOAT:                //87     
            {
                componentMapping.b = VK_COMPONENT_SWIZZLE_ZERO;
                componentMapping.a = VK_COMPONENT_SWIZZLE_ZERO;
            } 
            break;

        case F_PixelFormat_SHORT_B16G16R16_UNORM:              //88   
        case F_PixelFormat_SHORT_B16G16R16_SNORM:              //89
        case F_PixelFormat_SHORT_B16G16R16_USCALED:            //90   
        case F_PixelFormat_SHORT_B16G16R16_SSCALED:            //91   
        case F_PixelFormat_SHORT_B16G16R16_UINT:               //92   
        case F_PixelFormat_SHORT_B16G16R16_SINT:               //93   
        case F_PixelFormat_SHORT_B16G16R16_SFLOAT:             //94             
            {
                componentMapping.a = VK_COMPONENT_SWIZZLE_ZERO;
            } 
            break;

        case F_PixelFormat_SHORT_A16B16G16R16_UNORM:           //95 
        case F_PixelFormat_SHORT_A16B16G16R16_SNORM:           //96
        case F_PixelFormat_SHORT_A16B16G16R16_USCALED:         //97 
        case F_PixelFormat_SHORT_A16B16G16R16_SSCALED:         //98 
        case F_PixelFormat_SHORT_A16B16G16R16_UINT:            //99 
        case F_PixelFormat_SHORT_A16B16G16R16_SINT:            //100 
        case F_PixelFormat_SHORT_A16B16G16R16_SFLOAT:          //101             
            break;

        // INT CHANNEL
        case F_PixelFormat_INT_R32_UINT:                       //102
        case F_PixelFormat_INT_R32_SINT:                       //103
        case F_PixelFormat_INT_R32_SFLOAT:                     //104
            {
                componentMapping.g = VK_COMPONENT_SWIZZLE_ZERO;
                componentMapping.b = VK_COMPONENT_SWIZZLE_ZERO;
                componentMapping.a = VK_COMPONENT_SWIZZLE_ZERO;
            } 
            break;
        
        case F_PixelFormat_INT_G32R32_UINT:                    //105
        case F_PixelFormat_INT_G32R32_SINT:                    //106
        case F_PixelFormat_INT_G32R32_SFLOAT:                  //107
            {
                componentMapping.b = VK_COMPONENT_SWIZZLE_ZERO;
                componentMapping.a = VK_COMPONENT_SWIZZLE_ZERO;
            } 
            break;

        case F_PixelFormat_INT_B32G32R32_UINT:                 //108
        case F_PixelFormat_INT_B32G32R32_SINT:                 //109
        case F_PixelFormat_INT_B32G32R32_SFLOAT:               //110
            {
                componentMapping.a = VK_COMPONENT_SWIZZLE_ZERO;
            } 
            break;

        case F_PixelFormat_INT_A32B32G32R32_UINT:              //111
        case F_PixelFormat_INT_A32B32G32R32_SINT:              //112
        case F_PixelFormat_INT_A32B32G32R32_SFLOAT:            //113
            break;

        // LONG CHANNEL
        case F_PixelFormat_LONG_R64_UINT:                      //114
        case F_PixelFormat_LONG_R64_SINT:                      //115
        case F_PixelFormat_LONG_R64_SFLOAT:                    //116
            {
                componentMapping.g = VK_COMPONENT_SWIZZLE_ZERO;
                componentMapping.b = VK_COMPONENT_SWIZZLE_ZERO;
                componentMapping.a = VK_COMPONENT_SWIZZLE_ZERO;
            } 
            break;
        
        case F_PixelFormat_LONG_G64R64_UINT:                   //117
        case F_PixelFormat_LONG_G64R64_SINT:                   //118
        case F_PixelFormat_LONG_G64R64_SFLOAT:                 //119
            {
                componentMapping.b = VK_COMPONENT_SWIZZLE_ZERO;
                componentMapping.a = VK_COMPONENT_SWIZZLE_ZERO;
            } 
            break;

        case F_PixelFormat_LONG_B64G64R64_UINT:                //120
        case F_PixelFormat_LONG_B64G64R64_SINT:                //121
        case F_PixelFormat_LONG_B64G64R64_SFLOAT:              //122
            {
                componentMapping.a = VK_COMPONENT_SWIZZLE_ZERO;
            } 
            break;

        case F_PixelFormat_LONG_A64B64G32R64_UINT:             //123
        case F_PixelFormat_LONG_A64B64G32R64_SINT:             //124
        case F_PixelFormat_LONG_A64B64G32R64_SFLOAT:           //125
            break;

        // FLOAT
        case F_PixelFormat_FLOAT_B10G11R11_UFLOAT_PACK32:      //126
        case F_PixelFormat_FLOAT_E5B9G9R9_UFLOAT_PACK32:       //127
            break;

        // STENCIL
        case F_PixelFormat_STENCIL_S8_UINT:                    //128
            break;
        // DEPTH
        case F_PixelFormat_DEPTH_D16_UNORM:                    //129
        case F_PixelFormat_DEPTH_D24_UNORM:                    //130
        case F_PixelFormat_DEPTH_D32_SFLOAT:                   //131
            {
                componentMapping.g = VK_COMPONENT_SWIZZLE_ZERO;
                componentMapping.b = VK_COMPONENT_SWIZZLE_ZERO;
                componentMapping.a = VK_COMPONENT_SWIZZLE_ZERO;
            } 
            break;
        // DEPTHSTENCIL
        case F_PixelFormat_DEPTHSTENCIL_D16_UNORM_S8_UINT:     //132
        case F_PixelFormat_DEPTHSTENCIL_D24_UNORM_S8_UINT:     //133
        case F_PixelFormat_DEPTHSTENCIL_D32_SFLOAT_S8_UINT:    //134
            {
                componentMapping.g = VK_COMPONENT_SWIZZLE_IDENTITY;
                componentMapping.b = VK_COMPONENT_SWIZZLE_IDENTITY;
                componentMapping.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            } 
            break;

        // DXT
        case F_PixelFormat_DXT1:                               //135                 
        case F_PixelFormat_DXT2:                               //136        
        case F_PixelFormat_DXT3:                               //137        
        case F_PixelFormat_DXT4:                               //138        
        case F_PixelFormat_DXT5:                               //139        

        // BC
        case F_PixelFormat_BC4_UNORM:                          //140
        case F_PixelFormat_BC4_SNORM:                          //141
        case F_PixelFormat_BC5_UNORM:                          //142
        case F_PixelFormat_BC5_SNORM:                          //143
        case F_PixelFormat_BC6H_UF16:                          //144
        case F_PixelFormat_BC6H_SF16:                          //145
        case F_PixelFormat_BC7_UNORM:                          //146

        // PVRTC (PowerVR)
        case F_PixelFormat_PVRTC_RGB2:                         //147
        case F_PixelFormat_PVRTC_RGBA2:                        //148
        case F_PixelFormat_PVRTC_RGB4:                         //149
        case F_PixelFormat_PVRTC_RGBA4:                        //150
        case F_PixelFormat_PVRTC2_2BPP:                        //151
        case F_PixelFormat_PVRTC2_4BPP:                        //152

        // ETC
        case F_PixelFormat_ETC1_RGB8:                          //153
        case F_PixelFormat_ETC2_RGB8:                          //154
        case F_PixelFormat_ETC2_RGBA8:                         //155
        case F_PixelFormat_ETC2_RGB8A1:                        //156

        // ATC
        case F_PixelFormat_ATC_RGB:                            //157
        case F_PixelFormat_ATC_RGBA_EXPLICIT_ALPHA:            //158
        case F_PixelFormat_ATC_RGBA_INTERPOLATED_ALPHA:        //159

        // ASTC 
        case F_PixelFormat_ASTC_RGBA_4X4_LDR:                  //160
        case F_PixelFormat_ASTC_RGBA_5X4_LDR:                  //161
        case F_PixelFormat_ASTC_RGBA_5X5_LDR:                  //162
        case F_PixelFormat_ASTC_RGBA_6X5_LDR:                  //163
        case F_PixelFormat_ASTC_RGBA_6X6_LDR:                  //164
        case F_PixelFormat_ASTC_RGBA_8X5_LDR:                  //165
        case F_PixelFormat_ASTC_RGBA_8X6_LDR:                  //166
        case F_PixelFormat_ASTC_RGBA_8X8_LDR:                  //167
        case F_PixelFormat_ASTC_RGBA_10X5_LDR:                 //168
        case F_PixelFormat_ASTC_RGBA_10X6_LDR:                 //169
        case F_PixelFormat_ASTC_RGBA_10X8_LDR:                 //170
        case F_PixelFormat_ASTC_RGBA_10X10_LDR:                //171
        case F_PixelFormat_ASTC_RGBA_12X10_LDR:                //172
        case F_PixelFormat_ASTC_RGBA_12X12_LDR:                //173
            break;

        }

        return componentMapping;
    }

    VkImageAspectFlags VulkanConverter::Transform2VkImageAspectFlags(FPixelFormatType ePixelFormat, const bool bPreferDepthOverStencil /*= false*/)
    {
        const uint32 pfFlags = FPixelFormat::GetPixelFormatFlags(ePixelFormat);

        VkImageAspectFlags retVal = 0;
        if (pfFlags & F_PixelFormatFlag_IsDepth)
        {
            retVal = VK_IMAGE_ASPECT_DEPTH_BIT;
        }
        else
        {
            retVal = VK_IMAGE_ASPECT_COLOR_BIT;
        }
        return retVal;
    }

}; //LostPeter