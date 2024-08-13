/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-10-30
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/PreDefine.h"
#include "../include/VulkanDefine.h"

namespace LostPeterVulkan
{   
////////////////////////////// Typedef /////////////////////////////
    String Utile_VkResult2String(VkResult result)
    {   
        switch ((int)result)
        {
            case VK_SUCCESS:                        return "VK_SUCCESS";
            case VK_NOT_READY:                      return "VK_NOT_READY";
            case VK_TIMEOUT:                        return "VK_TIMEOUT";
            case VK_EVENT_SET:                      return "VK_EVENT_SET";
            case VK_EVENT_RESET:                    return "VK_EVENT_RESET";
            case VK_INCOMPLETE:                     return "VK_INCOMPLETE";
            case VK_ERROR_OUT_OF_HOST_MEMORY:       return "VK_ERROR_OUT_OF_HOST_MEMORY";
            case VK_ERROR_OUT_OF_DEVICE_MEMORY:     return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
            case VK_ERROR_INITIALIZATION_FAILED:    return "VK_ERROR_INITIALIZATION_FAILED";
            case VK_ERROR_DEVICE_LOST:              return "VK_ERROR_DEVICE_LOST";
            case VK_ERROR_MEMORY_MAP_FAILED:        return "VK_ERROR_MEMORY_MAP_FAILED";
            case VK_ERROR_LAYER_NOT_PRESENT:        return "VK_ERROR_LAYER_NOT_PRESENT";
            case VK_ERROR_EXTENSION_NOT_PRESENT:    return "VK_ERROR_EXTENSION_NOT_PRESENT";
            case VK_ERROR_FEATURE_NOT_PRESENT:      return "VK_ERROR_FEATURE_NOT_PRESENT";
            case VK_ERROR_INCOMPATIBLE_DRIVER:      return "VK_ERROR_INCOMPATIBLE_DRIVER";
            case VK_ERROR_TOO_MANY_OBJECTS:         return "VK_ERROR_TOO_MANY_OBJECTS";
            case VK_ERROR_FORMAT_NOT_SUPPORTED:     return "VK_ERROR_FORMAT_NOT_SUPPORTED";
            case VK_ERROR_FRAGMENTED_POOL:          return "VK_ERROR_FRAGMENTED_POOL";
            case VK_ERROR_OUT_OF_POOL_MEMORY:       return "VK_ERROR_OUT_OF_POOL_MEMORY";
            case VK_ERROR_INVALID_EXTERNAL_HANDLE:  return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
            case VK_ERROR_SURFACE_LOST_KHR:         return "VK_ERROR_SURFACE_LOST_KHR";
            case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
            case VK_SUBOPTIMAL_KHR:                 return "VK_SUBOPTIMAL_KHR";
            case VK_ERROR_OUT_OF_DATE_KHR:          return "VK_ERROR_OUT_OF_DATE_KHR";
            case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
            case VK_ERROR_VALIDATION_FAILED_EXT:    return "VK_ERROR_VALIDATION_FAILED_EXT";
            case VK_ERROR_INVALID_SHADER_NV:        return "VK_ERROR_INVALID_SHADER_NV";
            case VK_ERROR_NOT_PERMITTED_EXT:        return "VK_ERROR_NOT_PERMITTED_EXT";            
        }
        return String("Unknow !");
    }

    bool Util_CheckVkResult(VkResult result, const String& nameFunc)
    {
        if (result != VK_SUCCESS)
        {
            String msg = "*********************** UTIL_VK_CHECK: " + nameFunc + " failed with: " + Utile_VkResult2String(result);
            F_LogError("%s", msg.c_str());
            return false;
        }
        return true;
    }


////////////////////////////// Enum ////////////////////////////////
    //VulkanWindowType
    static String s_nameWindows[] = 
    {
        "Main",             //0:    Main
        "Game",             //1:    Game
        "Scene",            //2:    Scene
    };
    const String& Util_GetWindowTypeName(VulkanWindowType type)
    {
        return s_nameWindows[(int)type];
    }
    const String& Util_GetWindowTypeName(int type)
    {
        return s_nameWindows[(int)type];
    }
    VulkanWindowType Util_ParseWindowType(const String& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_Window_Count; i++)
        {
            if (s_nameWindows[i] == strName)
                return (VulkanWindowType)(i);
        }
        F_Assert(false && "Util_ParseWindowType: Wrong type name !")
        return Vulkan_Window_Main;
    }


    //VulkanPixelFormatComponentType
    static String s_namePixelFormatComponents[] = 
    {
        "ByteU",            //0: Byte unsigned
        "ByteS",            //1: Byte signed
        "ShortU",           //2: Short unsigned
        "ShortS",           //3: Short signed
        "IntU",             //4: Int unsigned
        "IntS",             //5: Int signed
        "LongU",            //6: Long unsigned
        "LongS",            //7: Long signed
        "Float16",          //8: Float 16
        "Float32",          //9: Float 32
        "Double"            //10: Double
    };
    const String& Util_GetPixelFormatComponentTypeName(VulkanPixelFormatComponentType type)
    {   
        return s_namePixelFormatComponents[(int)type];
    }
    const String& Util_GetPixelFormatComponentTypeName(int type)
    {
        return s_namePixelFormatComponents[type];
    }


    VkImageType Util_Transform2VkImageType(FTextureType type)
    {
        switch ((int)type)
        {
        case F_Texture_1D:          return VK_IMAGE_TYPE_1D;
        case F_Texture_2D:          return VK_IMAGE_TYPE_2D;
        case F_Texture_2DArray:     return VK_IMAGE_TYPE_2D;
        case F_Texture_3D:          return VK_IMAGE_TYPE_3D;
        case F_Texture_CubeMap:     return VK_IMAGE_TYPE_2D;
        }
        F_Assert(false && "Util_Transform2VkImageType: Wrong type !")
        return VK_IMAGE_TYPE_2D;
    }
    VkImageViewType Util_Transform2VkImageViewType(FTextureType type)
    {
        switch ((int)type)
        {
        case F_Texture_1D:          return VK_IMAGE_VIEW_TYPE_1D;
        case F_Texture_2D:          return VK_IMAGE_VIEW_TYPE_2D;
        case F_Texture_2DArray:     return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        case F_Texture_3D:          return VK_IMAGE_VIEW_TYPE_3D;
        case F_Texture_CubeMap:     return VK_IMAGE_VIEW_TYPE_CUBE;
        }
        F_Assert(false && "Util_Transform2VkImageViewType: Wrong type !")
        return VK_IMAGE_VIEW_TYPE_2D;
    }
    VkFormat Util_Transform2VkFormat(FTexturePixelFormatType type)
    {
        switch ((int32)type)
        {
            case F_TexturePixelFormat_R8_UNORM:          return VK_FORMAT_R8_UNORM;
            case F_TexturePixelFormat_R16_UNORM:         return VK_FORMAT_R16_UNORM;
            case F_TexturePixelFormat_R8G8B8A8_SRGB:     return VK_FORMAT_R8G8B8A8_SRGB;
            case F_TexturePixelFormat_R8G8B8A8_UNORM:    return VK_FORMAT_R8G8B8A8_UNORM;
        }
        F_Assert(false && "Util_Transform2VkFormat: Wrong type !")
        return VK_FORMAT_R8G8B8A8_SRGB;
    }
    VkComponentMapping Util_Transform2VkComponentMapping(FTexturePixelFormatType type)
    {
        VkComponentMapping componentMapping;
        componentMapping.r = VK_COMPONENT_SWIZZLE_R;
		componentMapping.g = VK_COMPONENT_SWIZZLE_G;
		componentMapping.b = VK_COMPONENT_SWIZZLE_B;
		componentMapping.a = VK_COMPONENT_SWIZZLE_A;
        
        switch ((int32)type)
        {
        case F_TexturePixelFormat_R8G8B8A8_SRGB:                
            break;
        }

        return componentMapping;
    }
    VkFilter Util_Transform2VkFilter(FTextureFilterPixelType type)
    {
        switch ((int)type)
        {
        case F_TextureFilterPixel_None:            return VK_FILTER_NEAREST;
        case F_TextureFilterPixel_Point:           return VK_FILTER_NEAREST;
        case F_TextureFilterPixel_Linear:          return VK_FILTER_LINEAR;
        case F_TextureFilterPixel_Anisotropic:     return VK_FILTER_LINEAR;
        }
        F_Assert(false && "Util_Transform2VkFilter: Wrong type !")
        return VK_FILTER_NEAREST;
    }
    VkSamplerMipmapMode Util_Transform2VkSamplerMipmapMode(FTextureFilterPixelType type)
    {
        switch ((int)type)
        {
        case F_TextureFilterPixel_None:            return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        case F_TextureFilterPixel_Point:           return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        case F_TextureFilterPixel_Linear:          return VK_SAMPLER_MIPMAP_MODE_LINEAR;
        case F_TextureFilterPixel_Anisotropic:     return VK_SAMPLER_MIPMAP_MODE_LINEAR;
        }
        F_Assert(false && "Util_Transform2VkSamplerMipmapMode: Wrong type !")
        return VK_SAMPLER_MIPMAP_MODE_NEAREST;
    }
    VkFilter Util_Transform2VkFilter(FTextureFilterType typeFilter, FTextureFilterSizeType typeFilterSize)
    {
        switch ((int)typeFilter)
        {
        case F_TextureFilter_None:
            {
                switch ((int)typeFilterSize)
                {
                    case F_TextureFilterSize_Min:        return VK_FILTER_NEAREST;
                    case F_TextureFilterSize_Mag:        return VK_FILTER_NEAREST;
                    F_Assert(false && "Util_Transform2VkFilter: Wrong FTextureFilterSizeType type !")
                }   
            }   
            break;
        case F_TextureFilter_Bilinear:
            {
                switch ((int)typeFilterSize)
                {
                    case F_TextureFilterSize_Min:        return VK_FILTER_LINEAR;
                    case F_TextureFilterSize_Mag:        return VK_FILTER_LINEAR;
                    F_Assert(false && "Util_Transform2VkFilter: Wrong FTextureFilterSizeType type !")
                }
            }
            break;
        case F_TextureFilter_Trilinear:
            {
                switch ((int)typeFilterSize)
                {
                    case F_TextureFilterSize_Min:        return VK_FILTER_LINEAR;
                    case F_TextureFilterSize_Mag:        return VK_FILTER_LINEAR;
                    F_Assert(false && "Util_Transform2VkFilter: Wrong FTextureFilterSizeType type !")
                }
            }
            break;
        case F_TextureFilter_Anisotropic:
            {
                switch ((int)typeFilterSize)
                {
                    case F_TextureFilterSize_Min:        return VK_FILTER_LINEAR;
                    case F_TextureFilterSize_Mag:        return VK_FILTER_LINEAR;
                    F_Assert(false && "Util_Transform2VkFilter: Wrong FTextureFilterSizeType type !")
                }
            }
            break;
        } 
        F_Assert(false && "Util_Transform2VkFilter: Wrong FTextureFilterType type !")
        return VK_FILTER_NEAREST;
    }
    VkSamplerMipmapMode Util_Transform2VkSamplerMipmapMode(FTextureFilterType typeFilter)
    {
        switch ((int)typeFilter)
        {
        case F_TextureFilter_None:         return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        case F_TextureFilter_Bilinear:     return VK_SAMPLER_MIPMAP_MODE_LINEAR;
        case F_TextureFilter_Trilinear:    return VK_SAMPLER_MIPMAP_MODE_LINEAR;
        case F_TextureFilter_Anisotropic:  return VK_SAMPLER_MIPMAP_MODE_LINEAR;
        } 
        F_Assert(false && "Util_Transform2VkFilter: Wrong FTextureFilterType type !")
        return VK_SAMPLER_MIPMAP_MODE_NEAREST;
    }
    VkSamplerAddressMode Util_Transform2VkSamplerAddressMode(FTextureAddressingType type)
    {
        switch((int)type)
        {
        case F_TextureAddressing_Wrap:      return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        case F_TextureAddressing_Mirror:    return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
        case F_TextureAddressing_Clamp:     return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        case F_TextureAddressing_Border:    return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        }
        F_Assert(false && "Util_Transform2VkSamplerAddressMode: Wrong type !")
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    }
    VkBorderColor Util_Transform2VkBorderColor(FTextureBorderColorType type)
    {
        switch((int)type)
        {
        case F_TextureBorderColor_OpaqueBlack:         return VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        case F_TextureBorderColor_OpaqueWhite:         return VK_BORDER_COLOR_INT_OPAQUE_WHITE;
        case F_TextureBorderColor_TransparentBlack:    return VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;
        }
        F_Assert(false && "Util_Transform2VkBorderColor: Wrong type !")
        return VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    }
    VkSampleCountFlagBits Util_Transform2VkSampleCountFlagBits(FMSAASampleCountType type)
    {
        switch((int)type)
        {
        case F_MSAASampleCount_1_Bit:      return VK_SAMPLE_COUNT_1_BIT;
        case F_MSAASampleCount_2_Bit:      return VK_SAMPLE_COUNT_2_BIT;
        case F_MSAASampleCount_4_Bit:      return VK_SAMPLE_COUNT_4_BIT;
        case F_MSAASampleCount_8_Bit:      return VK_SAMPLE_COUNT_8_BIT;
        case F_MSAASampleCount_16_Bit:     return VK_SAMPLE_COUNT_16_BIT;
        case F_MSAASampleCount_32_Bit:     return VK_SAMPLE_COUNT_32_BIT;
        case F_MSAASampleCount_64_Bit:     return VK_SAMPLE_COUNT_64_BIT;
        }
        F_Assert(false && "Util_Transform2VkSampleCountFlagBits: Wrong type !")
        return VK_SAMPLE_COUNT_1_BIT;
    }

    VkShaderStageFlagBits Util_Transform2VkShaderStageFlagBits(FShaderType type)
    {
        switch((int)type)
        {
        case F_Shader_Vertex:                  return VK_SHADER_STAGE_VERTEX_BIT;
        case F_Shader_TessellationControl:     return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        case F_Shader_TessellationEvaluation:  return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        case F_Shader_Geometry:                return VK_SHADER_STAGE_GEOMETRY_BIT;
        case F_Shader_Fragment:                return VK_SHADER_STAGE_FRAGMENT_BIT;
        case F_Shader_Compute:                 return VK_SHADER_STAGE_COMPUTE_BIT;
        }
        F_Assert(false && "Util_Transform2VkShaderStageFlagBits: Wrong type !")
        return VK_SHADER_STAGE_VERTEX_BIT;
    }
    VkShaderStageFlagBits Util_Transform2VkShaderStageFlagBits(const Uint32Vector& aShaderTypes)
    {
        int bits;
        size_t count = aShaderTypes.size();
        if (count > 0)
        {
            for (size_t i = 0; i < count; i++)
            {
                FShaderType typeShader = (FShaderType)aShaderTypes[i];
                if (i ==0)
                    bits = Util_Transform2VkShaderStageFlagBits(typeShader);
                else
                    bits = bits | Util_Transform2VkShaderStageFlagBits(typeShader);
            }
            return (VkShaderStageFlagBits)bits;
        }

        F_Assert(false && "Util_Transform2VkShaderStageFlagBits: Wrong count !")
        return VK_SHADER_STAGE_VERTEX_BIT; 
    }

    VkPrimitiveTopology Util_Transform2VkPrimitiveTopology(FRenderPrimitiveType type)
    {
        switch ((int32)type)
        {
        case F_RenderPrimitive_PointList:     return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
        case F_RenderPrimitive_LineList:      return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
        case F_RenderPrimitive_LineStrip:     return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
        case F_RenderPrimitive_TriangleList:  return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        case F_RenderPrimitive_TriangleStrip: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
        case F_RenderPrimitive_TriangleFan:   return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
        }
        F_Assert(false && "Util_Transform2VkPrimitiveTopology: Wrong type !")
        return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST; 
    }
    VkCullModeFlags Util_Transform2VkCullModeFlags(FCullingType type)
    {
        switch ((int32)type)
        {
        case F_Culling_None:             return VK_CULL_MODE_NONE;
        case F_Culling_ClockWise:        return VK_CULL_MODE_BACK_BIT;
        case F_Culling_CounterClockWise: return VK_CULL_MODE_FRONT_BIT;
        }
        F_Assert(false && "Util_Transform2VkCullModeFlags: Wrong type !")
        return VK_CULL_MODE_BACK_BIT;
    }
    VkPolygonMode Util_Transform2VkPolygonMode(FPolygonType type)
    {   
        switch ((int32)type)
        {
        case F_Polygon_Point:      return VK_POLYGON_MODE_POINT;
        case F_Polygon_WireFrame:  return VK_POLYGON_MODE_LINE;
        case F_Polygon_Solid:      return VK_POLYGON_MODE_FILL;
        }
        F_Assert(false && "Util_Transform2VkPolygonMode: Wrong type !")
        return VK_POLYGON_MODE_FILL;
    }
    VkStencilOp Util_Transform2VkStencilOp(FStencilOPType type)
    {
        switch ((int32)type)
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
        F_Assert(false && "Util_Transform2VkStencilOp: Wrong type !")
        return VK_STENCIL_OP_KEEP;
    }
    VkCompareOp Util_Transform2VkCompareOp(FCompareFuncType type)
    {
        switch ((int32)type)
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
        F_Assert(false && "Util_Transform2VkCompareOp: Wrong type !")
        return VK_COMPARE_OP_NEVER;
    }
    VkBlendOp Util_Transform2VkBlendOp(FSceneBlendingOPType type)
    {
        switch ((int32)type)
        {
        case F_SceneBlendingOP_Add:               return VK_BLEND_OP_ADD;
        case F_SceneBlendingOP_Subtract:          return VK_BLEND_OP_SUBTRACT;
        case F_SceneBlendingOP_SubtractReverse:   return VK_BLEND_OP_REVERSE_SUBTRACT;
        case F_SceneBlendingOP_Min:               return VK_BLEND_OP_MIN;
        case F_SceneBlendingOP_Max:               return VK_BLEND_OP_MAX;
        }
        F_Assert(false && "Util_Transform2VkBlendOp: Wrong type !")
        return VK_BLEND_OP_ADD;
    }
    VkBlendFactor Util_Transform2VkBlendFactor(FSceneBlendingFactorType type)
    {
        switch ((int32)type)
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
        F_Assert(false && "Util_Transform2VkBlendFactor: Wrong type !")
        return VK_BLEND_FACTOR_ONE;
    }


    //VulkanDescriptorSetType
    static String s_nameDescriptorSetTypes[] = 
    {
        "Pass",                     //0:   Pass
        "Object",                   //1:   Object
        "ObjectTerrain",            //2:   ObjectTerrain
        "ObjectGrid",               //3:   ObjectGrid
        "ObjectCameraAxis",         //4:   ObjectCameraAxis
        "ObjectCoordinateAxis",     //5:   ObjectCoordinateAxis
        "ObjectLineFlat2D",         //6:   ObjectLineFlat2D
        "ObjectLineFlat3D",         //7:   ObjectLineFlat3D
        "ObjectCopyBlit",           //8:   ObjectCopyBlit
        "Material",                 //9:   Material
        "Instance",                 //10:  Instance
        "TextureCopy",              //11:  TextureCopy
        "Tessellation",             //12:  Tessellation
        "Geometry",                 //13:  Geometry
        "TextureVS",                //14:  TextureVS
        "TextureTESC",              //15:  TextureTESC
        "TextureTESE",              //16:  TextureTESE
        "TextureFS",                //17:  TextureFS
        "TextureFrameColor",        //18:  TextureFrameColor
        "TextureCSR",               //19:  TextureCSR
        "TextureCSRW",              //20:  TextureCSRW
        "TextureDepthShadow",       //21:  TextureDepthShadow
        "InputAttachRed",           //22:  InputAttachRed
        "InputAttachGreen",         //23:  InputAttachGreen
        "InputAttachBlue",          //24:  InputAttachBlue
        "Terrain",                  //25:  Terrain
    };
    const String& Util_GetDescriptorSetTypeName(VulkanDescriptorSetType type)
    {
        return s_nameDescriptorSetTypes[(int)type];
    }
    const String& Util_GetDescriptorSetTypeName(int type)
    {
        return s_nameDescriptorSetTypes[type];
    }
    VulkanDescriptorSetType Util_ParseDescriptorSetType(const String& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_DescriptorSet_Count; i++)
        {
            if (s_nameDescriptorSetTypes[i] == strName)
                return (VulkanDescriptorSetType)(i);
        }
        F_Assert(false && "Util_ParseDescriptorSetType: Wrong type name !")
        return Vulkan_DescriptorSet_Pass;
    }


    //VulkanDescriptorSetUsageType
    static String s_nameDescriptorSetUsageTypes[] = 
    {
        "Sampler",                  //0:  Sampler                   VK_DESCRIPTOR_TYPE_SAMPLER
        "CombinedImageSampler",     //1:  CombinedImageSampler      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
        "SampledImage",             //2:  SampledImage              VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE
        "StorageImage",             //3:  StorageImage              VK_DESCRIPTOR_TYPE_STORAGE_IMAGE
        "UniformTexelBuffer",       //4:  UniformTexelBuffer        VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER
        "StorageTexelBuffer",       //5:  StorageTexelBuffer        VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER
        "UniformBuffer",            //6:  UniformBuffer             VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
        "StorageBuffer",            //7:  StorageBuffer             VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
        "UniformBufferDynamic",     //8:  UniformBufferDynamic      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC
        "StorageBufferDynamic",     //9:  StorageBufferDynamic      VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC
        "InputAttachment",          //10: InputAttachment           VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT
    };
    const String& Util_GetDescriptorSetUsageTypeName(VulkanDescriptorSetUsageType type)
    {
        return s_nameDescriptorSetUsageTypes[(int)type];
    }
    const String& Util_GetDescriptorSetUsageTypeName(int type)
    {
        return s_nameDescriptorSetUsageTypes[type];
    }
    VulkanDescriptorSetUsageType Util_ParseDescriptorSetUsageType(const String& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_DescriptorSetUsage_Count; i++)
        {
            if (s_nameDescriptorSetUsageTypes[i] == strName)
                return (VulkanDescriptorSetUsageType)(i);
        }
        F_Assert(false && "Util_ParseDescriptorSetUsageType: Wrong type name !")
        return Vulkan_DescriptorSetUsage_UniformBuffer;
    }
    VkDescriptorType Util_Transform2VkDescriptorType(VulkanDescriptorSetUsageType type)
    {
        switch((int)type)
        {
        case Vulkan_DescriptorSetUsage_Sampler:                     return VK_DESCRIPTOR_TYPE_SAMPLER;
        case Vulkan_DescriptorSetUsage_CombinedImageSampler:        return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        case Vulkan_DescriptorSetUsage_SampledImage:                return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        case Vulkan_DescriptorSetUsage_StorageImage:                return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        case Vulkan_DescriptorSetUsage_UniformTexelBuffer:          return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
        case Vulkan_DescriptorSetUsage_StorageTexelBuffer:          return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
        case Vulkan_DescriptorSetUsage_UniformBuffer:               return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        case Vulkan_DescriptorSetUsage_StorageBuffer:               return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        case Vulkan_DescriptorSetUsage_UniformBufferDynamic:        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        case Vulkan_DescriptorSetUsage_StorageBufferDynamic:        return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
        case Vulkan_DescriptorSetUsage_InputAttachment:             return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
        }
        F_Assert(false && "Util_Transform2VkDescriptorType: Wrong type !")
        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    }


    //VulkanObjectType
    static String s_nameObjectTypes[] = 
    {
        "Mesh",                 //0:    Mesh            
        "SkinMesh",             //1:    SkinMesh
        "Camera",               //2:    Camera
        "Light",                //3:    Light
        "Terrain",              //4:    Terrain
        "Water",                //5:    Water
        "Sky",                  //6:    Sky
        "Cloud",                //7:    Cloud
        "Particle",             //8:    Particle
    };
    const String& Util_GetObjectTypeName(VulkanObjectType type)
    {
        return s_nameObjectTypes[(int)type];
    }
    const String& Util_GetObjectTypeName(int type)
    {
        return s_nameObjectTypes[type];
    }
    VulkanObjectType Util_ParseObjectType(const String& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_Object_Count; i++)
        {
            if (s_nameObjectTypes[i] == strName)
                return (VulkanObjectType)(i);
        }
        F_Assert(false && "Util_ParseObjectType: Wrong type name !")
        return Vulkan_Object_Mesh;
    }


    //VulkanRenderQueueType
    static String s_nameRenderQueueTypes[] = 
    {
        "BackGround",               //0:    BackGround            
        "Opaque",                   //1:    Opaque
        "Terrain",                  //2:    Terrain
        "Sky",                      //3:    Sky
        "Transparent",              //4:    Transparent
        "UI",                       //5:    UI
    };
    const String& Util_GetRenderQueueTypeName(VulkanRenderQueueType type)
    {
        return s_nameRenderQueueTypes[(int)type];
    }
    const String& Util_GetRenderQueueTypeName(int type)
    {
        return s_nameRenderQueueTypes[type];
    }
    VulkanRenderQueueType Util_ParseRenderQueueType(const String& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_RenderQueue_Count; i++)
        {
            if (s_nameRenderQueueTypes[i] == strName)
                return (VulkanRenderQueueType)(i);
        }
        F_Assert(false && "Util_ParseRenderQueueType: Wrong type name !")
        return Vulkan_RenderQueue_Opaque;
    }


    //VulkanEngineConfigType
    static const String s_nameEngineConfigTypes[] = 
	{
    ////Common
		"MultiThread",					    //0: MultiThread

    ////Render


    ////Audio


    ////Physics
        

	};
    const String& Util_GetEngineConfigTypeName(VulkanEngineConfigType type)
    {
        return s_nameEngineConfigTypes[(int32)type];
    }
    const String& Util_GetEngineConfigTypeName(int32 type)
    {
        return s_nameEngineConfigTypes[type];
    }

////////////////////////////// Vulkan //////////////////////////////
    const VkVertexInputBindingDescriptionVector& Util_GetVkVertexInputBindingDescriptionVector(FMeshVertexType type)
    {
        switch ((int)type)
        {
            case F_MeshVertex_Pos2Color4:
                return Vertex_Pos2Color4::GetBindingDescriptions();
            case F_MeshVertex_Pos3Color4:
                return Vertex_Pos3Color4::GetBindingDescriptions();
            case F_MeshVertex_Pos3Normal3:
                return Vertex_Pos3Normal3::GetBindingDescriptions();
            case F_MeshVertex_Pos3Normal3Tex2:
                return Vertex_Pos3Normal3Tex2::GetBindingDescriptions();
            case F_MeshVertex_Pos2Color4Tex2:
                return Vertex_Pos2Color4Tex2::GetBindingDescriptions();
            case F_MeshVertex_Pos3Color4Tex2:
                return Vertex_Pos3Color4Tex2::GetBindingDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tex2:
                return Vertex_Pos3Color4Normal3Tex2::GetBindingDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tex4:
                return Vertex_Pos3Color4Normal3Tex4::GetBindingDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex2:
                return Vertex_Pos3Color4Normal3Tangent3Tex2::GetBindingDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex4:
                return Vertex_Pos3Color4Normal3Tangent3Tex4::GetBindingDescriptions();
        }
        F_Assert(false && "Util_GetVkVertexInputBindingDescriptionVector: Wrong type !")
        return Vertex_Pos3Color4Normal3Tangent3Tex2::GetBindingDescriptions();
    }
    const VkVertexInputAttributeDescriptionVector& Util_GetVkVertexInputAttributeDescriptionVector(FMeshVertexType type)
    {
        switch ((int)type)
        {
            case F_MeshVertex_Pos2Color4:
                return Vertex_Pos2Color4::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Color4:
                return Vertex_Pos3Color4::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Normal3:
                return Vertex_Pos3Normal3::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Normal3Tex2:
                return Vertex_Pos3Normal3Tex2::GetAttributeDescriptions();
            case F_MeshVertex_Pos2Color4Tex2:
                return Vertex_Pos2Color4Tex2::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Color4Tex2:
                return Vertex_Pos3Color4Tex2::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tex2:
                return Vertex_Pos3Color4Normal3Tex2::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tex4:
                return Vertex_Pos3Color4Normal3Tex4::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex2:
                return Vertex_Pos3Color4Normal3Tangent3Tex2::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex4:
                return Vertex_Pos3Color4Normal3Tangent3Tex4::GetAttributeDescriptions();
        }
        F_Assert(false && "Util_GetVkVertexInputAttributeDescriptionVector: Wrong type !")
        return Vertex_Pos3Color4Normal3Tangent3Tex2::GetAttributeDescriptions();
    }
    VkVertexInputBindingDescriptionVector* Util_GetVkVertexInputBindingDescriptionVectorPtr(FMeshVertexType type)
    {
        switch ((int)type)
        {
            case F_MeshVertex_Pos2Color4:
                return Vertex_Pos2Color4::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Color4:
                return Vertex_Pos3Color4::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Normal3:
                return Vertex_Pos3Normal3::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Normal3Tex2:
                return Vertex_Pos3Normal3Tex2::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos2Color4Tex2:
                return Vertex_Pos2Color4Tex2::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Tex2:
                return Vertex_Pos3Color4Tex2::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tex2:
                return Vertex_Pos3Color4Normal3Tex2::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tex4:
                return Vertex_Pos3Color4Normal3Tex4::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex2:
                return Vertex_Pos3Color4Normal3Tangent3Tex2::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex4:
                return Vertex_Pos3Color4Normal3Tangent3Tex4::GetBindingDescriptionsPtr();
        }
        F_Assert(false && "Util_GetVkVertexInputBindingDescriptionVectorPtr: Wrong type !")
        return Vertex_Pos3Color4Normal3Tangent3Tex2::GetBindingDescriptionsPtr();
    }   
    VkVertexInputAttributeDescriptionVector* Util_GetVkVertexInputAttributeDescriptionVectorPtr(FMeshVertexType type)
    {
        switch ((int)type)
        {
            case F_MeshVertex_Pos2Color4:
                return Vertex_Pos2Color4::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Color4:
                return Vertex_Pos3Color4::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Normal3:
                return Vertex_Pos3Normal3::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Normal3Tex2:
                return Vertex_Pos3Normal3Tex2::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos2Color4Tex2:
                return Vertex_Pos2Color4Tex2::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Tex2:
                return Vertex_Pos3Color4Tex2::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tex2:
                return Vertex_Pos3Color4Normal3Tex2::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tex4:
                return Vertex_Pos3Color4Normal3Tex4::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex2:
                return Vertex_Pos3Color4Normal3Tangent3Tex2::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex4:
                return Vertex_Pos3Color4Normal3Tangent3Tex4::GetAttributeDescriptionsPtr();
        }
        F_Assert(false && "Util_GetVkVertexInputAttributeDescriptionVectorPtr: Wrong type !")
        return Vertex_Pos3Color4Normal3Tangent3Tex2::GetAttributeDescriptionsPtr();
    }

    VkPipelineColorBlendAttachmentState Util_PipelineColorBlendAttachmentState(VkColorComponentFlags colorWriteMask, VkBool32 blendEnable)
    {
        VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState = {};
        pipelineColorBlendAttachmentState.colorWriteMask = colorWriteMask;
        pipelineColorBlendAttachmentState.blendEnable = blendEnable;
        return pipelineColorBlendAttachmentState;
    }

    ////////////////////////////// Class ///////////////////////////////
    MeshInfo::MeshInfo()
        : group(0)
        , nameMesh("")
        , pathMesh("")
        , typeMesh(F_Mesh_File)
        , typeVertex(F_MeshVertex_Pos3Color4Tex2)
        , typeGeometryType(F_MeshGeometry_EntityGrid)
        , pMeshCreateParam(nullptr)
        , isFlipY(false)
        , isTransformLocal(false)
        , matTransformLocal(FMath::ms_mat4Unit)
    {

    }
    MeshInfo::MeshInfo(const String& _nameMesh,
                       const String& _pathMesh,
                       FMeshType _typeMesh,
                       FMeshVertexType _typeVertex,
                       FMeshGeometryType _typeGeometryType,
                       FMeshCreateParam* _pMeshCreateParam,
                       bool _isFlipY,
                       bool _isTransformLocal,
                       const FMatrix4& _matTransformLocal)
        : group(0)
        , nameMesh(_nameMesh)
        , pathMesh(_pathMesh)
        , typeMesh(_typeMesh)
        , typeVertex(_typeVertex)
        , typeGeometryType(_typeGeometryType)
        , pMeshCreateParam(_pMeshCreateParam)
        , isFlipY(_isFlipY)
        , isTransformLocal(_isTransformLocal)
        , matTransformLocal(_matTransformLocal)
    {

    }
    MeshInfo::~MeshInfo()
    {
        F_DELETE(this->pMeshCreateParam)
    }


  


}; //LostPeterVulkan