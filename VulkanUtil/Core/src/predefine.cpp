#include "../include/preinclude.h"
#include "../include/predefine.h"

namespace LostPeter
{   

////Typedef
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


////Vulkan
    const VkVertexInputBindingDescriptionVector& Util_GetVkVertexInputBindingDescriptionVector(VulkanVertexType type)
    {
        switch ((int)type)
        {
            case Vulkan_VertexType_Pos2Color4:
                return Vertex_Pos2Color4::GetBindingDescriptions();
            case Vulkan_VertexType_Pos2Color4Tex2:
                return Vertex_Pos2Color4Tex2::GetBindingDescriptions();
            case Vulkan_VertexType_Pos3Color4Tex2:
                return Vertex_Pos3Color4Tex2::GetBindingDescriptions();
            case Vulkan_VertexType_Pos3Color4Normal3Tex2:
                return Vertex_Pos3Color4Normal3Tex2::GetBindingDescriptions();
            case Vulkan_VertexType_Pos3Color4Normal3Tangent3Tex2:
                return Vertex_Pos3Color4Normal3Tangent3Tex2::GetBindingDescriptions();
        }
        assert(false && "Util_GetVkVertexInputBindingDescriptionVector: Wrong type !");
        return Vertex_Pos3Color4Normal3Tangent3Tex2::GetBindingDescriptions();
    }
    const VkVertexInputAttributeDescriptionVector& Util_GetVkVertexInputAttributeDescriptionVector(VulkanVertexType type)
    {
        switch ((int)type)
        {
            case Vulkan_VertexType_Pos2Color4:
                return Vertex_Pos2Color4::GetAttributeDescriptions();
            case Vulkan_VertexType_Pos2Color4Tex2:
                return Vertex_Pos2Color4Tex2::GetAttributeDescriptions();
            case Vulkan_VertexType_Pos3Color4Tex2:
                return Vertex_Pos3Color4Tex2::GetAttributeDescriptions();
            case Vulkan_VertexType_Pos3Color4Normal3Tex2:
                return Vertex_Pos3Color4Normal3Tex2::GetAttributeDescriptions();
            case Vulkan_VertexType_Pos3Color4Normal3Tangent3Tex2:
                return Vertex_Pos3Color4Normal3Tangent3Tex2::GetAttributeDescriptions();
        }
        assert(false && "Util_GetVkVertexInputAttributeDescriptionVector: Wrong type !");
        return Vertex_Pos3Color4Normal3Tangent3Tex2::GetAttributeDescriptions();
    }
    VkVertexInputBindingDescriptionVector* Util_GetVkVertexInputBindingDescriptionVectorPtr(VulkanVertexType type)
    {
        switch ((int)type)
        {
            case Vulkan_VertexType_Pos2Color4:
                return Vertex_Pos2Color4::GetBindingDescriptionsPtr();
            case Vulkan_VertexType_Pos2Color4Tex2:
                return Vertex_Pos2Color4Tex2::GetBindingDescriptionsPtr();
            case Vulkan_VertexType_Pos3Color4Tex2:
                return Vertex_Pos3Color4Tex2::GetBindingDescriptionsPtr();
            case Vulkan_VertexType_Pos3Color4Normal3Tex2:
                return Vertex_Pos3Color4Normal3Tex2::GetBindingDescriptionsPtr();
            case Vulkan_VertexType_Pos3Color4Normal3Tangent3Tex2:
                return Vertex_Pos3Color4Normal3Tangent3Tex2::GetBindingDescriptionsPtr();
        }
        assert(false && "Util_GetVkVertexInputBindingDescriptionVectorPtr: Wrong type !");
        return Vertex_Pos3Color4Normal3Tangent3Tex2::GetBindingDescriptionsPtr();
    }   
    VkVertexInputAttributeDescriptionVector* Util_GetVkVertexInputAttributeDescriptionVectorPtr(VulkanVertexType type)
    {
        switch ((int)type)
        {
            case Vulkan_VertexType_Pos2Color4:
                return Vertex_Pos2Color4::GetAttributeDescriptionsPtr();
            case Vulkan_VertexType_Pos2Color4Tex2:
                return Vertex_Pos2Color4Tex2::GetAttributeDescriptionsPtr();
            case Vulkan_VertexType_Pos3Color4Tex2:
                return Vertex_Pos3Color4Tex2::GetAttributeDescriptionsPtr();
            case Vulkan_VertexType_Pos3Color4Normal3Tex2:
                return Vertex_Pos3Color4Normal3Tex2::GetAttributeDescriptionsPtr();
            case Vulkan_VertexType_Pos3Color4Normal3Tangent3Tex2:
                return Vertex_Pos3Color4Normal3Tangent3Tex2::GetAttributeDescriptionsPtr();
        }
        assert(false && "Util_GetVkVertexInputAttributeDescriptionVectorPtr: Wrong type !");
        return Vertex_Pos3Color4Normal3Tangent3Tex2::GetAttributeDescriptionsPtr();
    }


}; //LostPeter