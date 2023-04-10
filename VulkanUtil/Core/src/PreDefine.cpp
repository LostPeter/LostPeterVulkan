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

#include "../include/PreInclude.h"
#include "../include/PreDefine.h"

namespace LostPeter
{   

////////////////////////////// Typedef /////////////////////////////
    std::string Utile_VkResult2String(VkResult result)
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
        return std::string("Unknow !");
    }

    bool Util_CheckVkResult(VkResult result, const std::string& nameFunc)
    {
        if (result != VK_SUCCESS)
        {
            std::string msg = "UTIL_VK_CHECK: " + nameFunc + " failed with: " + Utile_VkResult2String(result);
            Util_LogError("%s", msg.c_str());
            return false;
        }
        return true;
    }


////////////////////////////// Enum ////////////////////////////////

    //VulkanVertexType
    static std::string s_nameVertices[] = 
    {
        "Pos2Color4",
        "Pos3Normal3",
        "Pos2Color4Tex2",
        "Pos3Color4Tex2",
        "Pos3Color4Normal3Tex2",
        "Pos3Color4Normal3Tangent3Tex2",
    };
    const std::string& Util_GetVertexTypeName(VulkanVertexType type)
    {
        return s_nameVertices[(int)type];
    }
    const std::string& Util_GetVertexTypeName(int type)
    {
        return s_nameVertices[(int)type];
    }
    VulkanVertexType Util_ParseVertexType(const std::string& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_Vertex_Count; i++)
        {
            if (s_nameVertices[i] == strName)
                return (VulkanVertexType)(i);
        }
        assert(false && "Util_ParseVertexType: Wrong type name !");
        return Vulkan_Vertex_Pos3Color4Normal3Tex2;
    }


    //VulkanMeshType
    static std::string s_nameMeshes[] = 
    {
        "file",
        "geometry",
    };
    const std::string& Util_GetMeshTypeName(VulkanMeshType type)
    {
        return s_nameMeshes[(int)type];
    }
    const std::string& Util_GetMeshTypeName(int type)
    {
        return s_nameMeshes[(int)type];
    }
    VulkanMeshType Util_ParseMeshType(const std::string& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_Mesh_Count; i++)
        {
            if (s_nameMeshes[i] == strName)
                return (VulkanMeshType)(i);
        }
        assert(false && "Util_ParseMeshType: Wrong type name !");
        return Vulkan_Mesh_File;
    }


    //VulkanMeshGeometryType
    static std::string s_nameMeshGeometries[] = 
    {
        "triangle",
        "quad",
        "grid",
        "circle",
        "aabb",
        "sphere",
        "geosphere",
        "cylinder",
        "capsule",
        "cone",
        "torus",
        "skybox",
        "skydome",
    };
    const std::string& Util_GetMeshGeometryTypeName(VulkanMeshGeometryType type)
    {
        return s_nameMeshGeometries[(int)type];
    }
    const std::string& Util_GetMeshGeometryTypeName(int type)
    {
        return s_nameMeshGeometries[type];
    }
    VulkanMeshGeometryType Util_ParseMeshGeometryType(const std::string& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_MeshGeometry_Count; i++)
        {
            if (s_nameMeshGeometries[i] == strName)
                return (VulkanMeshGeometryType)(i);
        }
        assert(false && "Util_ParseMeshGeometryType: Wrong type name !");
        return Vulkan_MeshGeometry_Triangle;
    }


    //VulkanTextureType
    static std::string s_nameTextures[] = 
    {
        "1d",
        "2d",
        "2darray",
        "3d",
        "cubemap"
    };
    const std::string& Util_GetTextureTypeName(VulkanTextureType type)
    {
        return s_nameTextures[(int)type];
    }
    const std::string& Util_GetTextureTypeName(int type)
    {
        return s_nameTextures[type];
    }
    VulkanTextureType Util_ParseTextureType(const std::string& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_Texture_Count; i++)
        {
            if (s_nameTextures[i] == strName)
                return (VulkanTextureType)(i);
        }
        assert(false && "Util_ParseTextureType: Wrong type name !");
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
        assert(false && "Util_Transform2VkImageType: Wrong type !");
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
        assert(false && "Util_Transform2VkImageViewType: Wrong type !");
        return VK_IMAGE_VIEW_TYPE_2D;
    }


    //VulkanTextureFilterSizeType
    static std::string s_nameTextureFilterSizes[] = 
    {
        "min",
        "mag",
        "mip",
    };
    const std::string& Util_GetTextureFilterSizeTypeName(VulkanTextureFilterSizeType type)
    {
        return s_nameTextureFilterSizes[(int)type];
    }
    const std::string& Util_GetTextureFilterSizeTypeName(int type)
    {
        return s_nameTextureFilterSizes[type];
    }
    VulkanTextureFilterSizeType Util_ParseTextureFilterSizeType(const std::string& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_TextureFilterSize_Count; i++)
        {
            if (s_nameTextureFilterSizes[i] == strName)
                return (VulkanTextureFilterSizeType)(i);
        }
        assert(false && "Util_ParseTextureFilterSizeType: Wrong type name !");
        return Vulkan_TextureFilterSize_Min;
    }


    //VulkanTextureFilterPixelType
    static std::string s_nameTextureFilterPixels[] = 
    {
        "none",
        "point",
        "linear",
        "anisotropic",
    };
    const std::string& Util_GetTextureFilterPixelTypeName(VulkanTextureFilterPixelType type)
    {
        return s_nameTextureFilterPixels[(int)type];
    }
    const std::string& Util_GetTextureFilterPixelTypeName(int type)
    {
        return s_nameTextureFilterPixels[type];
    }
    VulkanTextureFilterPixelType Util_ParseTextureFilterPixelType(const std::string& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_TextureFilterPixel_Count; i++)
        {
            if (s_nameTextureFilterPixels[i] == strName)
                return (VulkanTextureFilterPixelType)(i);
        }
        assert(false && "Util_ParseTextureFilterPixelType: Wrong type name !");
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
        assert(false && "Util_Transform2VkFilter: Wrong type !");
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
        assert(false && "Util_Transform2VkSamplerMipmapMode: Wrong type !");
        return VK_SAMPLER_MIPMAP_MODE_NEAREST;
    }


    //VulkanTextureFilterType
    static std::string s_nameTextureFilters[] = 
    {
        "none",
        "bilinear",
        "trilinear",
        "anisotropic",
    };
    const std::string& Util_GetTextureFilterTypeName(VulkanTextureFilterType type)
    {
        return s_nameTextureFilters[(int)type];
    }
    const std::string& Util_GetTextureFilterTypeName(int type)
    {
        return s_nameTextureFilters[type];
    }
    VulkanTextureFilterType Util_ParseTextureFilterType(const std::string& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_TextureFilter_Count; i++)
        {
            if (s_nameTextureFilters[i] == strName)
                return (VulkanTextureFilterType)(i);
        }
        assert(false && "Util_ParseTextureFilterType: Wrong type name !");
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
                    assert(false && "Util_Transform2VkFilter: Wrong VulkanTextureFilterSizeType type !");    
                }   
            }   
            break;
        case Vulkan_TextureFilter_Bilinear:
            {
                switch ((int)typeFilterSize)
                {
                    case Vulkan_TextureFilterSize_Min:        return VK_FILTER_LINEAR;
                    case Vulkan_TextureFilterSize_Mag:        return VK_FILTER_LINEAR;
                    assert(false && "Util_Transform2VkFilter: Wrong VulkanTextureFilterSizeType type !");    
                }
            }
            break;
        case Vulkan_TextureFilter_Trilinear:
            {
                switch ((int)typeFilterSize)
                {
                    case Vulkan_TextureFilterSize_Min:        return VK_FILTER_LINEAR;
                    case Vulkan_TextureFilterSize_Mag:        return VK_FILTER_LINEAR;
                    assert(false && "Util_Transform2VkFilter: Wrong VulkanTextureFilterSizeType type !");    
                }
            }
            break;
        case Vulkan_TextureFilter_Anisotropic:
            {
                switch ((int)typeFilterSize)
                {
                    case Vulkan_TextureFilterSize_Min:        return VK_FILTER_LINEAR;
                    case Vulkan_TextureFilterSize_Mag:        return VK_FILTER_LINEAR;
                    assert(false && "Util_Transform2VkFilter: Wrong VulkanTextureFilterSizeType type !");    
                }
            }
            break;
        } 
        assert(false && "Util_Transform2VkFilter: Wrong VulkanTextureFilterType type !");
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
        assert(false && "Util_Transform2VkFilter: Wrong VulkanTextureFilterType type !");
        return VK_SAMPLER_MIPMAP_MODE_NEAREST;
    }


    //VulkanTextureAddressingType
    static std::string s_nameTextureAddressings[] = 
    {
        "wrap",
        "mirror",
        "clamp",
        "border",
    };
    const std::string& Util_GetTextureAddressingTypeName(VulkanTextureAddressingType type)
    {
        return s_nameTextureAddressings[(int)type];
    }
    const std::string& Util_GetTextureAddressingTypeName(int type)
    {
        return s_nameTextureAddressings[type];
    }
    VulkanTextureAddressingType Util_ParseTextureAddressingType(const std::string& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_TextureAddressing_Count; i++)
        {
            if (s_nameTextureAddressings[i] == strName)
                return (VulkanTextureAddressingType)(i);
        }
        assert(false && "Util_ParseTextureAddressingType: Wrong type name !");
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
        assert(false && "Util_Transform2VkSamplerAddressMode: Wrong type !");
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    }


    //VulkanTextureBorderColorType
    static std::string s_nameTextureBorderColors[] = 
    {
        "opaque_black",
        "opaque_white",
        "transparent_black",
    };
    const std::string& Util_GetTextureBorderColorTypeName(VulkanTextureBorderColorType type)
    {
        return s_nameTextureBorderColors[(int)type];
    }
    const std::string& Util_GetTextureBorderColorTypeName(int type)
    {
        return s_nameTextureBorderColors[type];
    }
    VulkanTextureBorderColorType Util_ParseTextureBorderColorType(const std::string& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_TextureBorderColor_Count; i++)
        {
            if (s_nameTextureAddressings[i] == strName)
                return (VulkanTextureBorderColorType)(i);
        }
        assert(false && "Util_ParseTextureBorderColorType: Wrong type name !");
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
        assert(false && "Util_Transform2VkBorderColor: Wrong type !");
        return VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    }


    //VulkanMSAASampleCountType
    static std::string s_nameMSAASampleCounts[] = 
    {
        "1-Bit",
        "2-Bit",
        "4-Bit",
        "8-Bit",
        "16-Bit",
        "32-Bit",
        "64-Bit",
    };
    const std::string& Util_GetMSAASampleCountTypeName(VulkanMSAASampleCountType type)
    {
        return s_nameMSAASampleCounts[(int)type];
    }
    const std::string& Util_GetMSAASampleCountTypeName(int type)
    {
        return s_nameMSAASampleCounts[type];
    }
    VulkanMSAASampleCountType Util_ParseMSAASampleCountType(const std::string& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_MSAASampleCount_Count; i++)
        {
            if (s_nameMSAASampleCounts[i] == strName)
                return (VulkanMSAASampleCountType)(i);
        }
        assert(false && "Util_ParseMSAASampleCountType: Wrong type name !");
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
        assert(false && "Util_Transform2VkSampleCountFlagBits: Wrong type !");
        return VK_SAMPLE_COUNT_1_BIT;
    }


    //VulkanShaderType
    static std::string s_nameShaders[] = 
    {
        "vert",
        "tesc",
        "tese",
        "geom",
        "frag",
        "comp",
    };
    const std::string& Util_GetShaderTypeName(VulkanShaderType type)
    {
        return s_nameShaders[(int)type];
    }
    const std::string& Util_GetShaderTypeName(int type)
    {
        return s_nameShaders[type];
    }
    VulkanShaderType Util_ParseShaderType(const std::string& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_Shader_Count; i++)
        {
            if (s_nameShaders[i] == strName)
                return (VulkanShaderType)(i);
        }
        assert(false && "Util_ParseShaderType: Wrong type name !");
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
        assert(false && "Util_Transform2VkShaderStageFlagBits: Wrong type !");
        return VK_SHADER_STAGE_VERTEX_BIT;
    }


////////////////////////////// Vulkan //////////////////////////////
    const VkVertexInputBindingDescriptionVector& Util_GetVkVertexInputBindingDescriptionVector(VulkanVertexType type)
    {
        switch ((int)type)
        {
            case Vulkan_Vertex_Pos2Color4:
                return Vertex_Pos2Color4::GetBindingDescriptions();
            case Vulkan_Vertex_Pos3Normal3:
                return Vertex_Pos3Normal3::GetBindingDescriptions();
            case Vulkan_Vertex_Pos2Color4Tex2:
                return Vertex_Pos2Color4Tex2::GetBindingDescriptions();
            case Vulkan_Vertex_Pos3Color4Tex2:
                return Vertex_Pos3Color4Tex2::GetBindingDescriptions();
            case Vulkan_Vertex_Pos3Color4Normal3Tex2:
                return Vertex_Pos3Color4Normal3Tex2::GetBindingDescriptions();
            case Vulkan_Vertex_Pos3Color4Normal3Tangent3Tex2:
                return Vertex_Pos3Color4Normal3Tangent3Tex2::GetBindingDescriptions();
        }
        assert(false && "Util_GetVkVertexInputBindingDescriptionVector: Wrong type !");
        return Vertex_Pos3Color4Normal3Tangent3Tex2::GetBindingDescriptions();
    }
    const VkVertexInputAttributeDescriptionVector& Util_GetVkVertexInputAttributeDescriptionVector(VulkanVertexType type)
    {
        switch ((int)type)
        {
            case Vulkan_Vertex_Pos2Color4:
                return Vertex_Pos2Color4::GetAttributeDescriptions();
            case Vulkan_Vertex_Pos3Normal3:
                return Vertex_Pos3Normal3::GetAttributeDescriptions();
            case Vulkan_Vertex_Pos2Color4Tex2:
                return Vertex_Pos2Color4Tex2::GetAttributeDescriptions();
            case Vulkan_Vertex_Pos3Color4Tex2:
                return Vertex_Pos3Color4Tex2::GetAttributeDescriptions();
            case Vulkan_Vertex_Pos3Color4Normal3Tex2:
                return Vertex_Pos3Color4Normal3Tex2::GetAttributeDescriptions();
            case Vulkan_Vertex_Pos3Color4Normal3Tangent3Tex2:
                return Vertex_Pos3Color4Normal3Tangent3Tex2::GetAttributeDescriptions();
        }
        assert(false && "Util_GetVkVertexInputAttributeDescriptionVector: Wrong type !");
        return Vertex_Pos3Color4Normal3Tangent3Tex2::GetAttributeDescriptions();
    }
    VkVertexInputBindingDescriptionVector* Util_GetVkVertexInputBindingDescriptionVectorPtr(VulkanVertexType type)
    {
        switch ((int)type)
        {
            case Vulkan_Vertex_Pos2Color4:
                return Vertex_Pos2Color4::GetBindingDescriptionsPtr();
            case Vulkan_Vertex_Pos3Normal3:
                return Vertex_Pos3Normal3::GetBindingDescriptionsPtr();
            case Vulkan_Vertex_Pos2Color4Tex2:
                return Vertex_Pos2Color4Tex2::GetBindingDescriptionsPtr();
            case Vulkan_Vertex_Pos3Color4Tex2:
                return Vertex_Pos3Color4Tex2::GetBindingDescriptionsPtr();
            case Vulkan_Vertex_Pos3Color4Normal3Tex2:
                return Vertex_Pos3Color4Normal3Tex2::GetBindingDescriptionsPtr();
            case Vulkan_Vertex_Pos3Color4Normal3Tangent3Tex2:
                return Vertex_Pos3Color4Normal3Tangent3Tex2::GetBindingDescriptionsPtr();
        }
        assert(false && "Util_GetVkVertexInputBindingDescriptionVectorPtr: Wrong type !");
        return Vertex_Pos3Color4Normal3Tangent3Tex2::GetBindingDescriptionsPtr();
    }   
    VkVertexInputAttributeDescriptionVector* Util_GetVkVertexInputAttributeDescriptionVectorPtr(VulkanVertexType type)
    {
        switch ((int)type)
        {
            case Vulkan_Vertex_Pos2Color4:
                return Vertex_Pos2Color4::GetAttributeDescriptionsPtr();
            case Vulkan_Vertex_Pos3Normal3:
                return Vertex_Pos3Normal3::GetAttributeDescriptionsPtr();
            case Vulkan_Vertex_Pos2Color4Tex2:
                return Vertex_Pos2Color4Tex2::GetAttributeDescriptionsPtr();
            case Vulkan_Vertex_Pos3Color4Tex2:
                return Vertex_Pos3Color4Tex2::GetAttributeDescriptionsPtr();
            case Vulkan_Vertex_Pos3Color4Normal3Tex2:
                return Vertex_Pos3Color4Normal3Tex2::GetAttributeDescriptionsPtr();
            case Vulkan_Vertex_Pos3Color4Normal3Tangent3Tex2:
                return Vertex_Pos3Color4Normal3Tangent3Tex2::GetAttributeDescriptionsPtr();
        }
        assert(false && "Util_GetVkVertexInputAttributeDescriptionVectorPtr: Wrong type !");
        return Vertex_Pos3Color4Normal3Tangent3Tex2::GetAttributeDescriptionsPtr();
    }


}; //LostPeter