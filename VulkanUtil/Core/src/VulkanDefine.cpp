/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2022-12-05
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/PreInclude.h"
#include "../include/VulkanDefine.h"

namespace LostPeter
{
////Struct
    //////////////////////////////// Vertex_Pos2Color4 //////////////////////////////
    //0: Vulkan_VertexType_Pos2Color4
    VkVertexInputBindingDescriptionVector Vertex_Pos2Color4::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos2Color4::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos3Normal3 /////////////////////////////
    //1: Vulkan_VertexType_Pos3Normal3
    VkVertexInputBindingDescriptionVector Vertex_Pos3Normal3::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos3Normal3::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos2Color4Tex2 //////////////////////////
    //2: Vulkan_VertexType_Pos2Color4Tex2
    VkVertexInputBindingDescriptionVector Vertex_Pos2Color4Tex2::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos2Color4Tex2::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos3Color4Tex2 //////////////////////////
    //3: Vulkan_VertexType_Pos3Color4Tex2
    VkVertexInputBindingDescriptionVector Vertex_Pos3Color4Tex2::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos3Color4Tex2::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos3Color4Normal3Tex2 ///////////////////
    //4: Vulkan_VertexType_Pos3Color4Normal3Tex2
    VkVertexInputBindingDescriptionVector Vertex_Pos3Color4Normal3Tex2::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos3Color4Normal3Tex2::s_aAttributeDescriptions;

    //////////////////////////////// Vertex_Pos3Color4Normal3Tex2 ///////////////////
    //5: Vulkan_VertexType_Pos3Color4Normal3Tangent3Tex2
    VkVertexInputBindingDescriptionVector Vertex_Pos3Color4Normal3Tangent3Tex2::s_aBindingDescriptions;
    VkVertexInputAttributeDescriptionVector Vertex_Pos3Color4Normal3Tangent3Tex2::s_aAttributeDescriptions;
    

}; //LostPeter