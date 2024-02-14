/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-19
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    /////////////////////////////////////// Typedef //////////////////////////////////////
    String E_VkResult2String(VkResult result)
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

    bool E_CheckVkResult(VkResult result, const String& nameFunc)
    {
        if (result != VK_SUCCESS)
        {
            String msg = "*********************** E_VK_CHECK: " + nameFunc + " failed with: " + E_VkResult2String(result);
            F_LogError("%s", msg.c_str());
            return false;
        }
        return true;
    }

    void E_SetObjectName(VkDevice device, uint64_t object, VkDebugReportObjectTypeEXT objectType, const char* szName)
    {
        // Check for a valid function pointer
        // if (vkDebugMarkerSetObjectNameEXT)
        // {
        //     VkDebugMarkerObjectNameInfoEXT nameInfo = { };
        //     nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
        //     nameInfo.objectType = objectType;
        //     nameInfo.object = object;
        //     nameInfo.pObjectName = szName;
        //     vkDebugMarkerSetObjectNameEXT(device, &nameInfo);
        // }
    }


    //////////////////////////////// Vertex_Pos2Color4 //////////////////////////////
    //0: F_MeshVertex_Pos2Color4
    VkVertexInputBindingDescriptionVector Vertex_Pos2Color4::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos2Color4::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos3Color4 //////////////////////////////
    //1: F_MeshVertex_Pos3Color4
    VkVertexInputBindingDescriptionVector Vertex_Pos3Color4::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos3Color4::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos3Normal3 /////////////////////////////
    //2: F_MeshVertex_Pos3Normal3
    VkVertexInputBindingDescriptionVector Vertex_Pos3Normal3::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos3Normal3::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos3Normal3Tex2 /////////////////////////
    //3: F_MeshVertex_Pos3Normal3Tex2
    VkVertexInputBindingDescriptionVector Vertex_Pos3Normal3Tex2::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos3Normal3Tex2::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos2Color4Tex2 //////////////////////////
    //4: F_MeshVertex_Pos2Color4Tex2
    VkVertexInputBindingDescriptionVector Vertex_Pos2Color4Tex2::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos2Color4Tex2::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos3Color4Tex2 //////////////////////////
    //5: F_MeshVertex_Pos3Color4Tex2
    VkVertexInputBindingDescriptionVector Vertex_Pos3Color4Tex2::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos3Color4Tex2::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos3Color4Normal3Tex2 ///////////////////
    //6: F_MeshVertex_Pos3Color4Normal3Tex2
    VkVertexInputBindingDescriptionVector Vertex_Pos3Color4Normal3Tex2::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos3Color4Normal3Tex2::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos3Color4Normal3Tex4 ///////////////////
    //7: F_MeshVertex_Pos3Color4Normal3Tex4
    VkVertexInputBindingDescriptionVector Vertex_Pos3Color4Normal3Tex4::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos3Color4Normal3Tex4::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos3Color4Normal3Tangent3Tex2 ///////////
    //8: F_MeshVertex_Pos3Color4Normal3Tangent3Tex2
    VkVertexInputBindingDescriptionVector Vertex_Pos3Color4Normal3Tangent3Tex2::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos3Color4Normal3Tangent3Tex2::s_aAttributeDescriptions;
    
    //////////////////////////////// Vertex_Pos3Color4Normal3Tangent3Tex4 ///////////
    //9: F_MeshVertex_Pos3Color4Normal3Tangent3Tex4
    VkVertexInputBindingDescriptionVector Vertex_Pos3Color4Normal3Tangent3Tex4::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos3Color4Normal3Tangent3Tex4::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos3Normal3Tangent3BlendWI8Tex2 //////////
    //10: F_MeshVertex_Pos3Normal3Tangent3BlendWI8Tex2
    VkVertexInputBindingDescriptionVector Vertex_Pos3Normal3Tangent3BlendWI8Tex2::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos3Normal3Tangent3BlendWI8Tex2::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2 ////
    //11: F_MeshVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2
    VkVertexInputBindingDescriptionVector Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2::s_aAttributeDescriptions;

    const VkVertexInputBindingDescriptionVector& E_GetVkVertexInputBindingDescriptionVector(FMeshVertexType type)
    {
        switch ((int)type)
        {
            case F_MeshVertex_Pos2Color4:                                                   //0:    F_MeshVertex_Pos2Color4                                            
                return Vertex_Pos2Color4::GetBindingDescriptions();
            case F_MeshVertex_Pos3Color4:                                                   //1:    F_MeshVertex_Pos3Color4 (FMeshVertexPC)                                            
                return Vertex_Pos3Color4::GetBindingDescriptions();
            case F_MeshVertex_Pos3Normal3:                                                  //2:    F_MeshVertex_Pos3Normal3                                             
                return Vertex_Pos3Normal3::GetBindingDescriptions();
            case F_MeshVertex_Pos3Normal3Tex2:                                              //3:    F_MeshVertex_Pos3Normal3Tex2     
                return Vertex_Pos3Normal3Tex2::GetBindingDescriptions();
            case F_MeshVertex_Pos2Color4Tex2:                                               //4:    F_MeshVertex_Pos2Color4Tex2 
                return Vertex_Pos2Color4Tex2::GetBindingDescriptions();
            case F_MeshVertex_Pos3Color4Tex2:                                               //5:    F_MeshVertex_Pos3Color4Tex2 (FMeshVertexPCT)
                return Vertex_Pos3Color4Tex2::GetBindingDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tex2:                                        //6:    F_MeshVertex_Pos3Color4Normal3Tex2
                return Vertex_Pos3Color4Normal3Tex2::GetBindingDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tex4:                                        //7:    F_MeshVertex_Pos3Color4Normal3Tex4
                return Vertex_Pos3Color4Normal3Tex4::GetBindingDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex2:                                //8:    F_MeshVertex_Pos3Color4Normal3Tangent3Tex2 (FMeshVertex)
                return Vertex_Pos3Color4Normal3Tangent3Tex2::GetBindingDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex4:                                //9:    F_MeshVertex_Pos3Color4Normal3Tangent3Tex4 (FMeshVertexUV2)
                return Vertex_Pos3Color4Normal3Tangent3Tex4::GetBindingDescriptions();
            case F_MeshVertex_Pos3Normal3Tangent3BlendWI8Tex2:                              //10:   F_MeshVertex_Pos3Normal3Tangent3BlendWI8Tex2
                return Vertex_Pos3Normal3Tangent3BlendWI8Tex2::GetBindingDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2:                        //11:   F_MeshVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2 (FMeshVertexSkin)
                return Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2::GetBindingDescriptions();
        }
        F_Assert(false && "E_GetVkVertexInputBindingDescriptionVector: Wrong type !")
        return Vertex_Pos3Color4Normal3Tangent3Tex2::GetBindingDescriptions();
    }
    const VkVertexInputAttributeDescriptionVector& E_GetVkVertexInputAttributeDescriptionVector(FMeshVertexType type)
    {
        switch ((int)type)
        {
            case F_MeshVertex_Pos2Color4:                                                   //0:    F_MeshVertex_Pos2Color4  
                return Vertex_Pos2Color4::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Color4:                                                   //1:    F_MeshVertex_Pos3Color4 (FMeshVertexPC) 
                return Vertex_Pos3Color4::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Normal3:                                                  //2:    F_MeshVertex_Pos3Normal3        
                return Vertex_Pos3Normal3::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Normal3Tex2:                                              //3:    F_MeshVertex_Pos3Normal3Tex2 
                return Vertex_Pos3Normal3Tex2::GetAttributeDescriptions();
            case F_MeshVertex_Pos2Color4Tex2:                                               //4:    F_MeshVertex_Pos2Color4Tex2 
                return Vertex_Pos2Color4Tex2::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Color4Tex2:                                               //5:    F_MeshVertex_Pos3Color4Tex2 (FMeshVertexPCT)
                return Vertex_Pos3Color4Tex2::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tex2:                                        //6:    F_MeshVertex_Pos3Color4Normal3Tex2
                return Vertex_Pos3Color4Normal3Tex2::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tex4:                                        //7:    F_MeshVertex_Pos3Color4Normal3Tex4
                return Vertex_Pos3Color4Normal3Tex4::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex2:                                //8:    F_MeshVertex_Pos3Color4Normal3Tangent3Tex2 (FMeshVertex)
                return Vertex_Pos3Color4Normal3Tangent3Tex2::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex4:                                //9:    F_MeshVertex_Pos3Color4Normal3Tangent3Tex4 (FMeshVertexUV2)
                return Vertex_Pos3Color4Normal3Tangent3Tex4::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Normal3Tangent3BlendWI8Tex2:                              //10:   F_MeshVertex_Pos3Normal3Tangent3BlendWI8Tex2
                return Vertex_Pos3Normal3Tangent3BlendWI8Tex2::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2:                        //11:   F_MeshVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2 (FMeshVertexSkin)
                return Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2::GetAttributeDescriptions();
        }
        F_Assert(false && "E_GetVkVertexInputAttributeDescriptionVector: Wrong type !")
        return Vertex_Pos3Color4Normal3Tangent3Tex2::GetAttributeDescriptions();
    }
    VkVertexInputBindingDescriptionVector* E_GetVkVertexInputBindingDescriptionVectorPtr(FMeshVertexType type)
    {
        switch ((int)type)
        {
            case F_MeshVertex_Pos2Color4:                                                   //0:    F_MeshVertex_Pos2Color4  
                return Vertex_Pos2Color4::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Color4:                                                   //1:    F_MeshVertex_Pos3Color4 (FMeshVertexPC) 
                return Vertex_Pos3Color4::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Normal3:                                                  //2:    F_MeshVertex_Pos3Normal3        
                return Vertex_Pos3Normal3::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Normal3Tex2:                                              //3:    F_MeshVertex_Pos3Normal3Tex2 
                return Vertex_Pos3Normal3Tex2::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos2Color4Tex2:                                               //4:    F_MeshVertex_Pos2Color4Tex2 
                return Vertex_Pos2Color4Tex2::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Tex2:                                               //5:    F_MeshVertex_Pos3Color4Tex2 (FMeshVertexPCT)
                return Vertex_Pos3Color4Tex2::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tex2:                                        //6:    F_MeshVertex_Pos3Color4Normal3Tex2
                return Vertex_Pos3Color4Normal3Tex2::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tex4:                                        //7:    F_MeshVertex_Pos3Color4Normal3Tex4
                return Vertex_Pos3Color4Normal3Tex4::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex2:                                //8:    F_MeshVertex_Pos3Color4Normal3Tangent3Tex2 (FMeshVertex)
                return Vertex_Pos3Color4Normal3Tangent3Tex2::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex4:                                //9:    F_MeshVertex_Pos3Color4Normal3Tangent3Tex4 (FMeshVertexUV2)
                return Vertex_Pos3Color4Normal3Tangent3Tex4::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Normal3Tangent3BlendWI8Tex2:                              //10:   F_MeshVertex_Pos3Normal3Tangent3BlendWI8Tex2
                return Vertex_Pos3Normal3Tangent3BlendWI8Tex2::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2:                        //11:   F_MeshVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2 (FMeshVertexSkin)
                return Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2::GetBindingDescriptionsPtr();
        }
        F_Assert(false && "E_GetVkVertexInputBindingDescriptionVectorPtr: Wrong type !")
        return Vertex_Pos3Color4Normal3Tangent3Tex2::GetBindingDescriptionsPtr();
    }
    VkVertexInputAttributeDescriptionVector* E_GetVkVertexInputAttributeDescriptionVectorPtr(FMeshVertexType type)
    {
        switch ((int)type)
        {
            case F_MeshVertex_Pos2Color4:                                                   //0:    F_MeshVertex_Pos2Color4  
                return Vertex_Pos2Color4::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Color4:                                                   //1:    F_MeshVertex_Pos3Color4 (FMeshVertexPC) 
                return Vertex_Pos3Color4::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Normal3:                                                  //2:    F_MeshVertex_Pos3Normal3        
                return Vertex_Pos3Normal3::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Normal3Tex2:                                              //3:    F_MeshVertex_Pos3Normal3Tex2 
                return Vertex_Pos3Normal3Tex2::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos2Color4Tex2:                                               //4:    F_MeshVertex_Pos2Color4Tex2 
                return Vertex_Pos2Color4Tex2::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Tex2:                                               //5:    F_MeshVertex_Pos3Color4Tex2 (FMeshVertexPCT)
                return Vertex_Pos3Color4Tex2::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tex2:                                        //6:    F_MeshVertex_Pos3Color4Normal3Tex2
                return Vertex_Pos3Color4Normal3Tex2::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tex4:                                        //7:    F_MeshVertex_Pos3Color4Normal3Tex4
                return Vertex_Pos3Color4Normal3Tex4::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex2:                                //8:    F_MeshVertex_Pos3Color4Normal3Tangent3Tex2 (FMeshVertex)
                return Vertex_Pos3Color4Normal3Tangent3Tex2::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex4:                                //9:    F_MeshVertex_Pos3Color4Normal3Tangent3Tex4 (FMeshVertexUV2)
                return Vertex_Pos3Color4Normal3Tangent3Tex4::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Normal3Tangent3BlendWI8Tex2:                              //10:   F_MeshVertex_Pos3Normal3Tangent3BlendWI8Tex2
                return Vertex_Pos3Normal3Tangent3BlendWI8Tex2::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2:                        //11:   F_MeshVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2 (FMeshVertexSkin)
                return Vertex_Pos3Color4Normal3Tangent3BlendWI8Tex2::GetAttributeDescriptionsPtr();
        }
        F_Assert(false && "E_GetVkVertexInputAttributeDescriptionVectorPtr: Wrong type !")
        return Vertex_Pos3Color4Normal3Tangent3Tex2::GetAttributeDescriptionsPtr();
    }

    /////////////////////////////////////// Vulkan Define ////////////////////////////////
    
}; //LostPeterPluginRendererVulkan