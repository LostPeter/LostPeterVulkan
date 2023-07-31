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

namespace LostPeter
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
            String msg = "UTIL_VK_CHECK: " + nameFunc + " failed with: " + Utile_VkResult2String(result);
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


    //VulkanPixelFormatType
    VkFormat Util_Transform2VkFormat(VulkanPixelFormatType type)
    {
        switch ((int32)type)
        {
            case Vulkan_PixelFormat_BYTE_A8R8G8B8_UNORM:            return VK_FORMAT_B8G8R8A8_UNORM; 
        }

        return VK_FORMAT_UNDEFINED;
    }
    VkComponentMapping Util_Transform2VkComponentMapping(VulkanPixelFormatType type)
    {
        VkComponentMapping componentMapping;
        componentMapping.r = VK_COMPONENT_SWIZZLE_R;
		componentMapping.g = VK_COMPONENT_SWIZZLE_G;
		componentMapping.b = VK_COMPONENT_SWIZZLE_B;
		componentMapping.a = VK_COMPONENT_SWIZZLE_A;
        
        switch ((int32)type)
        {
        case Vulkan_PixelFormat_BYTE_A8R8G8B8_UNORM:                
            break;
        }

        return componentMapping;
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


    //VulkanTextureType
    static String s_nameTextures[] = 
    {
        "1d",                   //0: 1D
        "2d",                   //1: 2D
        "2darray",              //2: 2DArray        
        "3d",                   //3: 3D
        "cubemap"               //4: CubeMap
    };
    const String& Util_GetTextureTypeName(VulkanTextureType type)
    {
        return s_nameTextures[(int)type];
    }
    const String& Util_GetTextureTypeName(int type)
    {
        return s_nameTextures[type];
    }
    VulkanTextureType Util_ParseTextureType(const String& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_Texture_Count; i++)
        {
            if (s_nameTextures[i] == strName)
                return (VulkanTextureType)(i);
        }
        F_Assert(false && "Util_ParseTextureType: Wrong type name !")
        return Vulkan_Texture_2D;
    }
    VkImageType Util_Transform2VkImageType(VulkanTextureType type)
    {
        switch ((int)type)
        {
        case Vulkan_Texture_1D: return VK_IMAGE_TYPE_1D;
        case Vulkan_Texture_2D: return VK_IMAGE_TYPE_2D;
        case Vulkan_Texture_2DArray: return VK_IMAGE_TYPE_2D;
        case Vulkan_Texture_3D: return VK_IMAGE_TYPE_3D;
        case Vulkan_Texture_CubeMap: return VK_IMAGE_TYPE_2D;
        }
        F_Assert(false && "Util_Transform2VkImageType: Wrong type !")
        return VK_IMAGE_TYPE_2D;
    }
    VkImageViewType Util_Transform2VkImageViewType(VulkanTextureType type)
    {
        switch ((int)type)
        {
        case Vulkan_Texture_1D: return VK_IMAGE_VIEW_TYPE_1D;
        case Vulkan_Texture_2D: return VK_IMAGE_VIEW_TYPE_2D;
        case Vulkan_Texture_2DArray: return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        case Vulkan_Texture_3D: return VK_IMAGE_VIEW_TYPE_3D;
        case Vulkan_Texture_CubeMap: return VK_IMAGE_VIEW_TYPE_CUBE;
        }
        F_Assert(false && "Util_Transform2VkImageViewType: Wrong type !")
        return VK_IMAGE_VIEW_TYPE_2D;
    }


    //VulkanTextureFilterSizeType
    static String s_nameTextureFilterSizes[] = 
    {
        "min",          //0: Min		
        "mag",          //1: Mag
        "mip",          //2: Mip
    };
    const String& Util_GetTextureFilterSizeTypeName(VulkanTextureFilterSizeType type)
    {
        return s_nameTextureFilterSizes[(int)type];
    }
    const String& Util_GetTextureFilterSizeTypeName(int type)
    {
        return s_nameTextureFilterSizes[type];
    }
    VulkanTextureFilterSizeType Util_ParseTextureFilterSizeType(const String& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_TextureFilterSize_Count; i++)
        {
            if (s_nameTextureFilterSizes[i] == strName)
                return (VulkanTextureFilterSizeType)(i);
        }
        F_Assert(false && "Util_ParseTextureFilterSizeType: Wrong type name !")
        return Vulkan_TextureFilterSize_Min;
    }


    //VulkanTextureFilterPixelType
    static String s_nameTextureFilterPixels[] = 
    {
        "none",             //0: None
        "point",            //1: Point
        "linear",           //2: Linear
        "anisotropic",      //3: Anisotropic
    };
    const String& Util_GetTextureFilterPixelTypeName(VulkanTextureFilterPixelType type)
    {
        return s_nameTextureFilterPixels[(int)type];
    }
    const String& Util_GetTextureFilterPixelTypeName(int type)
    {
        return s_nameTextureFilterPixels[type];
    }
    VulkanTextureFilterPixelType Util_ParseTextureFilterPixelType(const String& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_TextureFilterPixel_Count; i++)
        {
            if (s_nameTextureFilterPixels[i] == strName)
                return (VulkanTextureFilterPixelType)(i);
        }
        F_Assert(false && "Util_ParseTextureFilterPixelType: Wrong type name !")
        return Vulkan_TextureFilterPixel_None;
    }
    VkFilter Util_Transform2VkFilter(VulkanTextureFilterPixelType type)
    {
        switch ((int)type)
        {
        case Vulkan_TextureFilterPixel_None:            return VK_FILTER_NEAREST;
        case Vulkan_TextureFilterPixel_Point:           return VK_FILTER_NEAREST;
        case Vulkan_TextureFilterPixel_Linear:          return VK_FILTER_LINEAR;
        case Vulkan_TextureFilterPixel_Anisotropic:     return VK_FILTER_LINEAR;
        }
        F_Assert(false && "Util_Transform2VkFilter: Wrong type !")
        return VK_FILTER_NEAREST;
    }
    VkSamplerMipmapMode Util_Transform2VkSamplerMipmapMode(VulkanTextureFilterPixelType type)
    {
        switch ((int)type)
        {
        case Vulkan_TextureFilterPixel_None:            return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        case Vulkan_TextureFilterPixel_Point:           return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        case Vulkan_TextureFilterPixel_Linear:          return VK_SAMPLER_MIPMAP_MODE_LINEAR;
        case Vulkan_TextureFilterPixel_Anisotropic:     return VK_SAMPLER_MIPMAP_MODE_LINEAR;
        }
        F_Assert(false && "Util_Transform2VkSamplerMipmapMode: Wrong type !")
        return VK_SAMPLER_MIPMAP_MODE_NEAREST;
    }


    //VulkanTextureFilterType
    static String s_nameTextureFilters[] = 
    {
        "none",             //0: None
        "bilinear",         //1: Point
        "trilinear",        //2: Linear
        "anisotropic",      //3: Anisotropic
    };
    const String& Util_GetTextureFilterTypeName(VulkanTextureFilterType type)
    {
        return s_nameTextureFilters[(int)type];
    }
    const String& Util_GetTextureFilterTypeName(int type)
    {
        return s_nameTextureFilters[type];
    }
    VulkanTextureFilterType Util_ParseTextureFilterType(const String& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_TextureFilter_Count; i++)
        {
            if (s_nameTextureFilters[i] == strName)
                return (VulkanTextureFilterType)(i);
        }
        F_Assert(false && "Util_ParseTextureFilterType: Wrong type name !")
        return Vulkan_TextureFilter_None;
    }
    VkFilter Util_Transform2VkFilter(VulkanTextureFilterType typeFilter, VulkanTextureFilterSizeType typeFilterSize)
    {
        switch ((int)typeFilter)
        {
        case Vulkan_TextureFilter_None:
            {
                switch ((int)typeFilterSize)
                {
                    case Vulkan_TextureFilterSize_Min:        return VK_FILTER_NEAREST;
                    case Vulkan_TextureFilterSize_Mag:        return VK_FILTER_NEAREST;
                    F_Assert(false && "Util_Transform2VkFilter: Wrong VulkanTextureFilterSizeType type !")
                }   
            }   
            break;
        case Vulkan_TextureFilter_Bilinear:
            {
                switch ((int)typeFilterSize)
                {
                    case Vulkan_TextureFilterSize_Min:        return VK_FILTER_LINEAR;
                    case Vulkan_TextureFilterSize_Mag:        return VK_FILTER_LINEAR;
                    F_Assert(false && "Util_Transform2VkFilter: Wrong VulkanTextureFilterSizeType type !")
                }
            }
            break;
        case Vulkan_TextureFilter_Trilinear:
            {
                switch ((int)typeFilterSize)
                {
                    case Vulkan_TextureFilterSize_Min:        return VK_FILTER_LINEAR;
                    case Vulkan_TextureFilterSize_Mag:        return VK_FILTER_LINEAR;
                    F_Assert(false && "Util_Transform2VkFilter: Wrong VulkanTextureFilterSizeType type !")
                }
            }
            break;
        case Vulkan_TextureFilter_Anisotropic:
            {
                switch ((int)typeFilterSize)
                {
                    case Vulkan_TextureFilterSize_Min:        return VK_FILTER_LINEAR;
                    case Vulkan_TextureFilterSize_Mag:        return VK_FILTER_LINEAR;
                    F_Assert(false && "Util_Transform2VkFilter: Wrong VulkanTextureFilterSizeType type !")
                }
            }
            break;
        } 
        F_Assert(false && "Util_Transform2VkFilter: Wrong VulkanTextureFilterType type !")
        return VK_FILTER_NEAREST;
    }
    VkSamplerMipmapMode Util_Transform2VkSamplerMipmapMode(VulkanTextureFilterType typeFilter)
    {
        switch ((int)typeFilter)
        {
        case Vulkan_TextureFilter_None:         return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        case Vulkan_TextureFilter_Bilinear:     return VK_SAMPLER_MIPMAP_MODE_LINEAR;
        case Vulkan_TextureFilter_Trilinear:    return VK_SAMPLER_MIPMAP_MODE_LINEAR;
        case Vulkan_TextureFilter_Anisotropic:  return VK_SAMPLER_MIPMAP_MODE_LINEAR;
        } 
        F_Assert(false && "Util_Transform2VkFilter: Wrong VulkanTextureFilterType type !")
        return VK_SAMPLER_MIPMAP_MODE_NEAREST;
    }


    //VulkanTextureAddressingType
    static String s_nameTextureAddressings[] = 
    {
        "wrap",             //0: Wrap
        "mirror",           //1: Mirror
        "clamp",            //2: Clamp
        "border",           //3: Border
    };
    const String& Util_GetTextureAddressingTypeName(VulkanTextureAddressingType type)
    {
        return s_nameTextureAddressings[(int)type];
    }
    const String& Util_GetTextureAddressingTypeName(int type)
    {
        return s_nameTextureAddressings[type];
    }
    VulkanTextureAddressingType Util_ParseTextureAddressingType(const String& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_TextureAddressing_Count; i++)
        {
            if (s_nameTextureAddressings[i] == strName)
                return (VulkanTextureAddressingType)(i);
        }
        F_Assert(false && "Util_ParseTextureAddressingType: Wrong type name !")
        return Vulkan_TextureAddressing_Wrap;
    }
    VkSamplerAddressMode Util_Transform2VkSamplerAddressMode(VulkanTextureAddressingType type)
    {
        switch((int)type)
        {
        case Vulkan_TextureAddressing_Wrap:      return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        case Vulkan_TextureAddressing_Mirror:    return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
        case Vulkan_TextureAddressing_Clamp:     return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        case Vulkan_TextureAddressing_Border:    return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        }
        F_Assert(false && "Util_Transform2VkSamplerAddressMode: Wrong type !")
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    }


    //VulkanTextureBorderColorType
    static String s_nameTextureBorderColors[] = 
    {
        "opaque_black",             //0: OpaqueBlack
        "opaque_white",             //1: OpaqueWhite
        "transparent_black",        //2: TransparentBlack
    };
    const String& Util_GetTextureBorderColorTypeName(VulkanTextureBorderColorType type)
    {
        return s_nameTextureBorderColors[(int)type];
    }
    const String& Util_GetTextureBorderColorTypeName(int type)
    {
        return s_nameTextureBorderColors[type];
    }
    VulkanTextureBorderColorType Util_ParseTextureBorderColorType(const String& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_TextureBorderColor_Count; i++)
        {
            if (s_nameTextureAddressings[i] == strName)
                return (VulkanTextureBorderColorType)(i);
        }
        F_Assert(false && "Util_ParseTextureBorderColorType: Wrong type name !")
        return Vulkan_TextureBorderColor_OpaqueBlack;
    }
    VkBorderColor Util_Transform2VkBorderColor(VulkanTextureBorderColorType type)
    {
        switch((int)type)
        {
        case Vulkan_TextureBorderColor_OpaqueBlack:         return VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        case Vulkan_TextureBorderColor_OpaqueWhite:         return VK_BORDER_COLOR_INT_OPAQUE_WHITE;
        case Vulkan_TextureBorderColor_TransparentBlack:    return VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;
        }
        F_Assert(false && "Util_Transform2VkBorderColor: Wrong type !")
        return VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    }


    //VulkanMSAASampleCountType
    static String s_nameMSAASampleCounts[] = 
    {
        "1-Bit",                //0: 1_Bit
        "2-Bit",                //1: 2_Bit
        "4-Bit",                //2: 4_Bit
        "8-Bit",                //3: 8_Bit
        "16-Bit",               //4: 16_Bit
        "32-Bit",               //5: 32_Bit
        "64-Bit",               //6: 64_Bit
    };
    const String& Util_GetMSAASampleCountTypeName(VulkanMSAASampleCountType type)
    {
        return s_nameMSAASampleCounts[(int)type];
    }
    const String& Util_GetMSAASampleCountTypeName(int type)
    {
        return s_nameMSAASampleCounts[type];
    }
    VulkanMSAASampleCountType Util_ParseMSAASampleCountType(const String& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_MSAASampleCount_Count; i++)
        {
            if (s_nameMSAASampleCounts[i] == strName)
                return (VulkanMSAASampleCountType)(i);
        }
        F_Assert(false && "Util_ParseMSAASampleCountType: Wrong type name !")
        return Vulkan_MSAASampleCount_1_Bit;
    }
    VkSampleCountFlagBits Util_Transform2VkSampleCountFlagBits(VulkanMSAASampleCountType type)
    {
        switch((int)type)
        {
        case Vulkan_MSAASampleCount_1_Bit:      return VK_SAMPLE_COUNT_1_BIT;
        case Vulkan_MSAASampleCount_2_Bit:      return VK_SAMPLE_COUNT_2_BIT;
        case Vulkan_MSAASampleCount_4_Bit:      return VK_SAMPLE_COUNT_4_BIT;
        case Vulkan_MSAASampleCount_8_Bit:      return VK_SAMPLE_COUNT_8_BIT;
        case Vulkan_MSAASampleCount_16_Bit:     return VK_SAMPLE_COUNT_16_BIT;
        case Vulkan_MSAASampleCount_32_Bit:     return VK_SAMPLE_COUNT_32_BIT;
        case Vulkan_MSAASampleCount_64_Bit:     return VK_SAMPLE_COUNT_64_BIT;
        }
        F_Assert(false && "Util_Transform2VkSampleCountFlagBits: Wrong type !")
        return VK_SAMPLE_COUNT_1_BIT;
    }


    //VulkanShaderType
    static String s_nameShaders[] = 
    {
        "vert",                 //0: Vertex
        "tesc",                 //1: TessellationControl
        "tese",                 //2: TessellationEvaluation
        "geom",                 //3: Geometry
        "frag",                 //4: Fragment
        "comp",                 //5: Compute
    };
    const String& Util_GetShaderTypeName(VulkanShaderType type)
    {
        return s_nameShaders[(int)type];
    }
    const String& Util_GetShaderTypeName(int type)
    {
        return s_nameShaders[type];
    }
    VulkanShaderType Util_ParseShaderType(const String& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_Shader_Count; i++)
        {
            if (s_nameShaders[i] == strName)
                return (VulkanShaderType)(i);
        }
        F_Assert(false && "Util_ParseShaderType: Wrong type name !")
        return Vulkan_Shader_Vertex;
    }
    VkShaderStageFlagBits Util_Transform2VkShaderStageFlagBits(VulkanShaderType type)
    {
        switch((int)type)
        {
        case Vulkan_Shader_Vertex:                  return VK_SHADER_STAGE_VERTEX_BIT;
        case Vulkan_Shader_TessellationControl:     return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        case Vulkan_Shader_TessellationEvaluation:  return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        case Vulkan_Shader_Geometry:                return VK_SHADER_STAGE_GEOMETRY_BIT;
        case Vulkan_Shader_Fragment:                return VK_SHADER_STAGE_FRAGMENT_BIT;
        case Vulkan_Shader_Compute:                 return VK_SHADER_STAGE_COMPUTE_BIT;
        }
        F_Assert(false && "Util_Transform2VkShaderStageFlagBits: Wrong type !")
        return VK_SHADER_STAGE_VERTEX_BIT;
    }


    //VulkanDescriptorSetType
    static String s_nameDescriptorSets[] = 
    {
        "Pass",                     //0:   Pass
        "Object",                   //1:   Object
        "ObjectTerrain",            //2:   ObjectTerrain
        "ObjectGrid",               //3:   ObjectGrid
        "ObjectCameraAxis",         //4:   ObjectCameraAxis
        "ObjectCoordinateAxis",     //5:   ObjectCoordinateAxis
        "ObjectLineFlat",           //6:   ObjectLineFlat
        "ObjectCopyBlit",           //7:   ObjectCopyBlit
        "Material",                 //8:   Material
        "Instance",                 //9:   Instance
        "TextureCopy",              //10:  TextureCopy
        "Tessellation",             //11:  Tessellation
        "Geometry",                 //12:  Geometry
        "TextureVS",                //13:  TextureVS
        "TextureTESC",              //14:  TextureTESC
        "TextureTESE",              //15:  TextureTESE
        "TextureFS",                //16:  TextureFS
        "TextureFrameColor",        //17:  TextureFrameColor
        "TextureCSR",               //18:  TextureCSR
        "TextureCSRW",              //19:  TextureCSRW
    };
    const String& Util_GetDescriptorSetTypeName(VulkanDescriptorSetType type)
    {
        return s_nameDescriptorSets[(int)type];
    }
    const String& Util_GetDescriptorSetTypeName(int type)
    {
        return s_nameDescriptorSets[type];
    }
    VulkanDescriptorSetType Util_ParseDescriptorSetType(const String& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_DescriptorSet_Count; i++)
        {
            if (s_nameDescriptorSets[i] == strName)
                return (VulkanDescriptorSetType)(i);
        }
        F_Assert(false && "Util_ParseDescriptorSetType: Wrong type name !")
        return Vulkan_DescriptorSet_Pass;
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


}; //LostPeter